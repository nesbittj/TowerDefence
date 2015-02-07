#include "cEngine.h"

cEngine* cEngine::mInstance = NULL;

cEngine* cEngine::Instance()
{
	if(!mInstance) mInstance = new cEngine;
	return mInstance;
}

int cEngine::Init()
{
	mWindow = NULL;
	mRen = NULL;
	mLog = NULL;
	mInput = NULL;
	mTexture = NULL;
	mQuit = false;
	mRender = true;
	mUpdate = true;

	mLog = cLogger::Instance();
	mInput = cInput::Instance();
	mInput->SetEventPtr(&mEvent);

	LoadConfigFromFile("assets/config.xml");

	/*
	//TODO: set timer resolution/granularity without windows.h/Winmm.lib
	//get timer resolution
	TIMECAPS tc;
	timeGetDevCaps(&tc,sizeof(TIMECAPS));
	//set timer resolution
	Uint32 DesiredSchedulerMS = 1;
	mSleepIsGranuler = (timeBeginPeriod(DesiredSchedulerMS) == TIMERR_NOERROR);
	*/

	mPerfCountFrequency = SDL_GetPerformanceFrequency();
	mLastCounter = SDL_GetPerformanceCounter();

	mFPSTimer = cTimer();
	mFPSTimer.Start();
	mFPSCap = cTimer();
	mFPSCap.Start();
	mCountedFrames = 1;
	mAvgFPS = 0;

	mUpdatesCap.Start();
	mCountedUpdates = 0;
	mAvgUpdates = 0;

	if(SDL_Init(SDL_INIT_EVERYTHING))
	{
		mLog->LogSDLError("SDL init error: ");
		return -1;
	}

	mWindow = SDL_CreateWindow("SDL Window", 100,100, SCREEN_WIDTH,SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if(!mWindow)
	{
		mLog->LogSDLError("SDL_CreateWindow");
		return -1;
	}

	mRen = cRenderer::Instance();
	if(mRen->Init(mWindow,&mEvent)) return -1;

	mArena = new cArena();
	mArena->Init();
	
	mRen->InitCamera(&mEvent,mInput,
		SCREEN_WIDTH,SCREEN_HEIGHT,mArena->GetArenaWidth(),mArena->GetArenaHeight());

	//TODO: remember to remove this debug texture
	mTexture = mRen->LoadBitmap("assets/arena/grid_bg.bmp");

	mPlayer = cPlayer(10,0);
	mTowerController = cTowerController();
	mTowerController.Init(mArena,&mCursorX,&mCursorY);

	mEnemyController = cEnemyController();
	mEnemyController.Init(mArena);

	return 0;
}

int cEngine::CleanUp()
{
	mArena->CleanUp();
	delete mArena;
	mArena = NULL;

	mEnemyController.CleanUp();
	mTowerController.CleanUp();

	mFPSTimer.Stop();

	if(mTexture) SDL_DestroyTexture(mTexture);
	mTexture = NULL;

	if(mRen) mRen->CleanUp();
	mRen = NULL;

	if(mWindow) SDL_DestroyWindow(mWindow);
	mWindow = NULL;

	SDL_Quit();

	mInput = NULL;
	mLog = NULL;

	return 0;
}

/*
loads config from xml file.
returns -1 on load file error.
returns 0 on success.
//TODO: if there is no config, generate one
*/
int cEngine::LoadConfigFromFile(const char* _filename)
{
	int l_result = 0;
	tinyxml2::XMLDocument doc;
	if(!doc.LoadFile(_filename))
	{
		XMLElement* l_elem = doc.FirstChildElement("config")->FirstChildElement("display");
		l_elem->QueryIntAttribute("width",&SCREEN_WIDTH);
		l_elem->QueryIntAttribute("height",&SCREEN_HEIGHT);
		l_elem->QueryIntAttribute("fps",&SCREEN_FPS);
	}
	else
	{
		SCREEN_WIDTH = 640;
		SCREEN_HEIGHT = 480;
		SCREEN_FPS = 60;
		mLog->LogError("cEngine::LoadConfig failed");
		l_result = -1;
	}
	SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;
	UPDATE_FREQ = SCREEN_FPS * 2;
	UPDATE_TICKS_PER_FRAME = 1000 / UPDATE_FREQ;

	
	mMonitorRefreshHz = 60;
	mGameUpdatesHz = mMonitorRefreshHz;
	mTargetSecondsPerFrame = 1.f / mGameUpdatesHz;

	return l_result;
}

/*
updates game events,inputs,movement etc
sets global quit value to true if close button is pressed
*/
void cEngine::Update()
{
	UpdateEvents();

	mAvgUpdates = CalcAvgUpdates();
	//CapFrameRate();
	//CapUpdateRate();

	if(mRender)
	{
		//TODO: find a better solution to getting cam pos, possibly another location to calc cursor.
		JVector2 camPos = mRen->mCamera->GetPos();
		mCursorX = cMaths::Round(mInput->GetMouseX() - camPos.x,LEVEL_GRID_SIZE);
		mCursorY = cMaths::Round(mInput->GetMouseY() - camPos.y,LEVEL_GRID_SIZE);

		mRen->Update();
		//mPlayer.Update();
		mTowerController.Update(mEnemyController.GetEnemies(),mEnemyController.GetMaxEnemies());
		mEnemyController.Update();
		mArena->Update();

		mCountedUpdates++;
	}
}

void cEngine::UpdateEvents()
{
	while(SDL_PollEvent(&mEvent) != 0)
	{
		if(mInput->UpdateInputEvents() == -1) mQuit = true;
		if(mRen->UpdateEvents() == -1) mQuit = true;
	}
	mInput->UpdateOldKeys();
}

/*
renders scene by calling cRenderer routines
*/
void cEngine::Render()
{	
	mAvgFPS = CalcAvgFPS();

	if(mRender)
	{
		//JVector2 cam = mRen->mCamera->GetPos();
		//mRen->RenderTexture(mTexture,0,0,NULL,SCREEN_SPACE);
		
		mArena->Draw();

		SDL_Color mouseColour = { 0,0,0,255 };
		mRen->DrawRect(mCursorX,mCursorY,30,30,mouseColour,0,WORLD_SPACE);

		std::stringstream FPS; FPS.str(""); FPS << "FPS: " << CalcFPS() << "\n";
		mRen->RenderText(FPS.str().c_str(),34,50,0,mouseColour,NULL,SCREEN_SPACE);
		mPrevFPS = mFPSTimer.GetTicks();

		std::stringstream AVGFPS; AVGFPS.str(""); AVGFPS << "Avg FPS: " << mAvgFPS;
		mRen->RenderText(AVGFPS.str().c_str(),34,80,0,mouseColour,NULL,SCREEN_SPACE);
		
		std::stringstream AVGUPDS; AVGUPDS.str(""); AVGUPDS << "Avg Ups: " << mAvgUpdates;
		mRen->RenderText(AVGUPDS.str().c_str(),34,110,0,mouseColour,NULL,SCREEN_SPACE);

		mTowerController.DrawTowersInUse();
		mTowerController.DrawTower(mCursorX,mCursorY,mTowerController.GetTowerSelected(),WORLD_SPACE);
		mTowerController.DrawTowerText(mCursorX,mCursorY - 15,mTowerController.GetTowerSelected(),mouseColour,WORLD_SPACE);
		mEnemyController.DrawEnemies();

		SleepFPS();

		mRen->Present(NULL);
		mCountedFrames++;
	}
}

bool cEngine::GetQuit() { return mQuit; }
Uint32 cEngine::CalcAvgFPS() { return mCountedFrames / ( mFPSTimer.GetTicks() / 1000.f ); }
Uint32 cEngine::CalcAvgUpdates() { return mCountedUpdates / ( mFPSTimer.GetTicks() / 1000.f ); }
int cEngine::CalcFPS() { return 1000 / (mFPSTimer.GetTicks() - mPrevFPS); }

/*
caps framerate to SCREEN_FPS using SCREEN_TICKS_PER_FRAME
and mFPSCap timer.
if enough time per frame has not passed (SCREEN_TICKS_PER_FRAME),
do not render. if enough time has passed, do render.
*/
void cEngine::CapFrameRate()
{
	if( mFPSCap.GetTicks() < SCREEN_TICKS_PER_FRAME ) mRender = false;
	else { mRender = true; mFPSCap.Start(); }
}

void cEngine::CapUpdateRate()
{
	if( mUpdatesCap.GetTicks() < UPDATE_TICKS_PER_FRAME ) mUpdate = false;
	else { mUpdate = true; mUpdatesCap.Start(); }
}

void cEngine::SleepFPS()
{
	float SecondsElapsedForFrame = GetSecondsElapsed(mLastCounter,SDL_GetPerformanceCounter());;
	if(SecondsElapsedForFrame < mTargetSecondsPerFrame)
	{
		if(mSleepIsGranuler)
		{
			Uint32 SleepMS = (Uint32)(1000.f * (mTargetSecondsPerFrame - SecondsElapsedForFrame));
			if(SleepMS > 0) SDL_Delay(SleepMS);
		}
		while(SecondsElapsedForFrame < mTargetSecondsPerFrame)
		{
			SecondsElapsedForFrame = GetSecondsElapsed(mLastCounter,SDL_GetPerformanceCounter());
		}
	}
	else
	{
	}		

	Uint64 EndCounter = SDL_GetPerformanceCounter();
	mLastCounter = EndCounter;

	/*
	float MSPerFrame = 1000*GetSecondsElapsed(mLastCounter,EndCounter);
	float FPS = 0.f; //PerfCountFrequency / CounterElapsed;
	float MCPF = 0.f; //(INT32)(CyclesElapsed / (1000 * 1000));

	char buffer[256];
	sprintf(buffer,"%fms/f, %ff/s, %fmc/f\n",MSPerFrame,FPS,MCPF);
	OutputDebugStringA(buffer);
	*/
}

inline float cEngine::GetSecondsElapsed(Uint64 Start, Uint64 End)
{
	float result = ((float)(End - Start) / (float)mPerfCountFrequency);
	return result;
}
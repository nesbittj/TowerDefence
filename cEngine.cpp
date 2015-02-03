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

	mFPSTimer = cTimer();
	mFPSTimer.start();
	mFPSCap = cTimer();
	mFPSCap.start();
	mCountedFrames = 0;
	mAvgFPS = 0;

	mUpdatesCap.start();
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
	
	mRen->mCamera->Init(&mEvent,mInput,
		SCREEN_WIDTH,SCREEN_HEIGHT,mArena->GetArenaWidth(),mArena->GetArenaHeight());

	//TODO: remember to remove this debug texture
	mTexture = mRen->LoadBitmap("assets/arena/grid_bg.bmp");

	mPlayer = cPlayer(10,0,LEVEL_GRID_SIZE,mRen->mCamera);
	mTowerController = cTowerController();
	mTowerController.Init(mArena,&mPlayer);

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

	mFPSTimer.stop();

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
*/
int cEngine::LoadConfigFromFile(const char* _filename)
{
	int l_result = 0;
	XMLDocument doc;
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
	CapFrameRate();
	CapUpdateRate();

	if(mUpdate)
	{
		mRen->Update();
		mPlayer.Update();
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
		mRen->DrawRect(mPlayer.GetCurserX(),mPlayer.GetCurserY(),30,30,mouseColour,0,SCREEN_SPACE);
		mRen->RenderText(mAvgFPS,34,50,0,mouseColour,NULL,SCREEN_SPACE);
		mRen->RenderText(mAvgUpdates,34,80,0,mouseColour,NULL,SCREEN_SPACE);

		mTowerController.DrawTowersInUse();
		mTowerController.DrawTower(mPlayer.GetCurserX(),mPlayer.GetCurserY(),mTowerController.GetTowerSelected(),SCREEN_SPACE);
		mTowerController.DrawTowerText(mPlayer.GetCurserX(),mPlayer.GetCurserY() - 15,mTowerController.GetTowerSelected(),mouseColour,SCREEN_SPACE);
		mEnemyController.DrawEnemies();

		mRen->Present(NULL);
		mCountedFrames++;
	}
}

bool cEngine::GetQuit() { return mQuit; }
Uint32 cEngine::CalcAvgFPS() { return mCountedFrames / ( mFPSTimer.getTicks() / 1000.f ); }
Uint32 cEngine::CalcAvgUpdates() { return mCountedUpdates / ( mFPSTimer.getTicks() / 1000.f ); }

/*
caps framerate to SCREEN_FPS using SCREEN_TICKS_PER_FRAME
and mFPSCap timer.
if enough time per frame has not passed (SCREEN_TICKS_PER_FRAME),
do not render. if enough time has passed, do render.
*/
void cEngine::CapFrameRate()
{
	if( mFPSCap.getTicks() < SCREEN_TICKS_PER_FRAME ) mRender = false;
	else { mRender = true; mFPSCap.start(); }
}

void cEngine::CapUpdateRate()
{
	if( mUpdatesCap.getTicks() < UPDATE_TICKS_PER_FRAME ) mUpdate = false;
	else { mUpdate = true; mUpdatesCap.start(); }
}
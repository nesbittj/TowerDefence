#include "cEngine.h"

cEngine* cEngine::mInstance = NULL;

cEngine* cEngine::Instance()
{
	if(!mInstance) mInstance = new cEngine;
	return mInstance;
}

/*
initialises SDL, window and game objects
sets game window width and height
creates pointer to renderer and logger singleton instance
*/
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

	mWindow = SDL_CreateWindow("SDL Window", 100,100, SCREEN_WIDTH,SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if(!mWindow)
	{
		mLog->LogSDLError("SDL_CreateWindow");
		return -1;
	}

	mRen = cRenderer::Instance();
	if(mRen->Init(mWindow)) return -1;
	SDL_Rect cam = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
	mRen->SetCamera(new cCamera(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,LEVEL_WIDTH,LEVEL_HIEGHT));

	mTexture = mRen->LoadBitmap("assets/grid_bg.bmp");

	mNodeRoot = new cSceneNode();
	mPlayer = cPlayer(10,0,LEVEL_GRID_SIZE,mRen->GetCamera());
	mTowerController = cTowerController();
	mTowerController.Init(LEVEL_GRID_SIZE,&mPlayer);
	// TODO: pass level grid size by pointer

	mEnemyController = cEnemyController();
	mEnemyController.Init(32);

	mCore = new cCore(SCREEN_WIDTH,SCREEN_HEIGHT,LEVEL_GRID_SIZE);
	mCore->Init(0);

	return 0;
}

int cEngine::CleanUp()
{
	mCore->CleanUp();
	delete mCore; mCore = NULL;
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
updates game events,inputs,movement etc
sets global quit value to true if close button is pressed
*/
void cEngine::Update()
{
	if(mInput->UpdateInputEvents() != 0) mQuit = true;

	mAvgUpdates = CalcAvgUpdates();
	CapFrameRate();
	CapUpdateRate();

	if(mUpdate)
	{
		UpdateCamera();
		mPlayer.Update();
		mTowerController.Update(mEnemyController.GetEnemies(),mEnemyController.GetMaxEnemies());
		mEnemyController.Update(mCore->GetPos());
		mCore->Update(mEnemyController.GetEnemies(),mEnemyController.GetMaxEnemies());
		mCountedUpdates++;
	}
}

/*
moves camera by user input
*/
void cEngine::UpdateCamera()
{
	JVector3 new_cam(0,0,mRen->GetCamera()->GetMoveSpeed());
	if(mInput->GetKeyDown(SDLK_w)) new_cam.y += new_cam.z;
	if(mInput->GetKeyDown(SDLK_s)) new_cam.y -= new_cam.z;
	if(mInput->GetKeyDown(SDLK_a)) new_cam.x += new_cam.z;
	if(mInput->GetKeyDown(SDLK_d)) new_cam.x -= new_cam.z;
	if(mInput->GetKeyDownRelease(SDLK_BACKSPACE)) new_cam.Zero();
	if(new_cam.x != 0 || new_cam.y != 0)
		mRen->GetCamera()->UpdateRelative(new_cam.x,new_cam.y);
}


/*
renders scene by calling cRenderer routines
*/
void cEngine::Render()
{	
	mAvgFPS = CalcAvgFPS();

	if(mRender)
	{
		float2 cam = mRen->GetCamera()->GetPos();

		mRen->RenderTexture(mTexture,0,0,NULL);

		SDL_Color mouseColour = { 0,0,0,255 };
		mRen->DrawRect(mPlayer.GetCurserX(),mPlayer.GetCurserY(),30,30,mouseColour,SCREEN_SPACE);
		mRen->RenderText(mAvgFPS,34,50,0,mouseColour,NULL,SCREEN_SPACE);
		mRen->RenderText(mAvgUpdates,34,80,0,mouseColour,NULL,SCREEN_SPACE);

		mCore->Draw();
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
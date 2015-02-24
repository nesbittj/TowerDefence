#include "cEngine.h"

cEngine* cEngine::mInstance = NULL;

cEngine* cEngine::Instance()
{
	if(!mInstance) mInstance = new cEngine;
	return mInstance;
}

int cEngine::Init()
{
	mRen = NULL;
	mLog = NULL;
	mInput = NULL;
	mQuit = false;
	mRender = true;
	mUpdate = true;

	mLog = cLogger::Instance();
	mInput = cInput::Instance();
	mInput->SetEventPtr(&mEvent);

	if(SDL_Init(SDL_INIT_EVERYTHING))
	{
		mLog->LogSDLError("SDL init error: ");
		return -1;
	}

	mRen = cRenderer::Instance();
	if(mRen->Init(&mEvent)) return -1;
	//TODO: init logging

	 //Initialize SDL_mixer
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
	{
		//TODO: log sound open properly
		printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
	}

	mArena = new cArena();
	mArena->Init();
	
	mRen->InitCamera(&mEvent,mInput,mArena->GetArenaWidth(),mArena->GetArenaHeight());

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

	Mix_Quit();

	if(mRen) mRen->CleanUp();
	mRen = NULL;

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
	UpdateEvents();

	if(mRender)
	{
		//TODO: find a better solution to getting cam pos, possibly another location to calc cursor.
		JVector2 camPos = mRen->mCamera->GetPos();
		mCursorX = cMaths::Round(mInput->GetMouseX() - (Uint32)camPos.x,32);
		mCursorY = cMaths::Round(mInput->GetMouseY() - (Uint32)camPos.y,32);

		mRen->Update();
		//mPlayer.Update();
		mTowerController.Update(mEnemyController.GetEnemies(),mEnemyController.GetMaxEnemies());
		mEnemyController.Update();
		mArena->Update();
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
	if(mRender)
	{		
		mArena->Draw();

		SDL_Color mouseColour = { 0,0,0,255 };
		mRen->DrawRect((float)mCursorX,(float)mCursorY,30,30,mouseColour,0,WORLD_SPACE);

		mTowerController.DrawTowersInUse();
		mTowerController.DrawTower((float)mCursorX,(float)mCursorY,mTowerController.GetTowerSelected(),WORLD_SPACE);
		mTowerController.DrawTowerText((float)mCursorX,(float)(mCursorY - 15),mTowerController.GetTowerSelected(),mouseColour,WORLD_SPACE);
		mEnemyController.DrawEnemies();

		mRen->Present(NULL);
	}
}

bool cEngine::GetQuit() { return mQuit; }
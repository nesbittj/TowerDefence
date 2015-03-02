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
	mPaused = false;
	mQuit = false;

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
	mTowerController.Init(mArena);

	mEnemyController = cEnemyController();
	mEnemyController.Init(mArena);

	mGUI = new cGUInamepsace::cGUI();
	SDL_Color theme_colour = { 255, 255, 0, 255 };
	mGUI->Init(theme_colour);
	cGUInamepsace::panel* l_panel = mGUI->AddElementPanel(200,200,200,200,"Menu",0);
	l_panel->AddElementButton(10,30,100,20,"BUTTON!!",l_panel);

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

	if(mPaused)	//TODO: NEED TO PAUSE TIMERS!!
	{
		//update menu events
		if(mInput->GetKeyDownRelease(SDLK_p))
		{
			mPaused = false;
			mTowerController.UnPause();
			mEnemyController.UnPause();
		}
	}
	else
	{
		if(mInput->GetKeyDownRelease(SDLK_p))
		{
			mPaused = true;
			mRen->TakeSnapshot(0,0);
			mTowerController.Pause();
			mEnemyController.Pause();
		}

		mRen->mCamera->Update();
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

void cEngine::Render()
{
	mRen->ClearToColour(0);

	if(mPaused)
	{
		mRen->RenderSnapshot(0,0,0);
		mGUI->Render();
	}
	else
	{
		mArena->Draw();

		SDL_Color mouseColour = { 0,0,0,255 };
		mRen->DrawRect(
			(float)mRen->mCamera->GetCursorX(),(float)mRen->mCamera->GetCursorY(),
			30,30,mouseColour,0,WORLD_SPACE);

		mTowerController.DrawTowersInUse();
		mTowerController.DrawTower(
			(float)mRen->mCamera->GetCursorX(),(float)mRen->mCamera->GetCursorY(),
			mTowerController.GetTowerSelected(),WORLD_SPACE);
		mTowerController.DrawTowerText(
			(float)mRen->mCamera->GetCursorX(),((float)mRen->mCamera->GetCursorY() - 15),
			mTowerController.GetTowerSelected(),mouseColour,WORLD_SPACE);
		mEnemyController.DrawEnemies();
	}

	mRen->Present(NULL);
}

bool cEngine::GetQuit() { return mQuit; }
#include "cEngine.h"

cEngine* cEngine::mInstance = NULL;

cEngine* cEngine::Instance()
{
	if(!mInstance) mInstance = new cEngine;
	return mInstance;
}

//TODO: consider more secure approach
bool* gQuit;
void RunQuit(void) { printf("cEngine:: RUN QUit!\n"); *gQuit = true; }

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

	mPlayer = cPlayer(10,0,1000);
	mTowerController = cTowerController();
	mTowerController.Init(mArena,&mPlayer);

	mEnemyController = cEnemyController();
	mEnemyController.Init(mArena,&mPlayer);

	SetGUILayout();

	return 0;
}

void cEngine::SetGUILayout()
{
	mGUI = cGUInamepsace::cGUI::Instance();
	SDL_Color theme_colour = { 255, 255, 0, 255 };
	mGUI->Init(theme_colour);

	mPausePanel = mGUI->AddElementPanel(200,200,200,200);	
	gQuit = &mQuit;
	mPausePanel->AddElementButton(10,30,160,20,"Quit Game To Desktop",RunQuit);

	mHUD = mGUI->AddElementPanel(0,0,150,100);
	mHUD->AddElementTextfield(15,15,96,15,"Enemies Alive: ");
	mHUD->AddElementTextfield(115,15,15,15,(Sint32*)mEnemyController.GetNumEnemiesAlive());
	mHUD->AddElementTextfield(15,32,96,15,"Money: £");
	mHUD->AddElementTextfield(115,32,15,15,(Sint32*)mPlayer.GetMoney());
	mHUD->AddElementTextfield(15,49,96,15,"Score: ");
	mHUD->AddElementTextfield(115,49,15,15,(Sint32*)mPlayer.GetScore());
	mHUD->SetFocus(cGUInamepsace::GUI_FOCUS_NO_MOUSE);
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
	mGUI->Update();

	if(mPaused)
	{
		//update menu events
		if(mInput->GetKeyDownRelease(SDLK_p))
		{
			mPaused = false;
			mTowerController.UnPause();
			mEnemyController.UnPause();
			mPausePanel->SetFocus(cGUInamepsace::GUI_FOCUS_NONE);
			mHUD->SetFocus(cGUInamepsace::GUI_FOCUS_NO_MOUSE);
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
			mPausePanel->SetFocus(cGUInamepsace::GUI_FOCUS_NO_MOUSE);
			mHUD->SetFocus(cGUInamepsace::GUI_FOCUS_NONE);
		}

		mRen->mCamera->Update();
		mPlayer.Update();
		mTowerController.Update(mEnemyController.GetEnemies(),mEnemyController.GetMaxEnemies());
		mEnemyController.Update();
		mArena->Update();
	}
}

void cEngine::UpdateEvents()
{
	mInput->UpdateOldKeys();
	while(SDL_PollEvent(&mEvent) != 0)
	{
		if(mInput->UpdateInputEvents() == -1) mQuit = true;
		if(mRen->UpdateEvents() == -1) mQuit = true;
	}
}

void cEngine::Render()
{
	mRen->ClearToColour(0);

	if(mPaused)
	{
		mRen->RenderSnapshot(0,0,0);
	}
	else
	{
		mArena->Draw();

		SDL_Color mouseColour = { 0,0,0,255 };
		mRen->DrawRect(
			(float)mRen->mCamera->GetCursorX(),(float)mRen->mCamera->GetCursorY(),
			30,30,mouseColour,0,WORLD_SPACE);

		mTowerController.DrawTowersInUse();
		if(mTowerController.mTowerEditMode)
		{
			mTowerController.DrawTower(
				(float)mRen->mCamera->GetCursorX(),(float)mRen->mCamera->GetCursorY(),
				mTowerController.GetTowerSelected(),WORLD_SPACE);
			mTowerController.DrawTowerText(
				(float)mRen->mCamera->GetCursorX(),((float)mRen->mCamera->GetCursorY() - 15),
				mTowerController.GetTowerSelected(),mouseColour,WORLD_SPACE);
		}
		mEnemyController.DrawEnemies();
	}

	mGUI->Render();

	mRen->Present(NULL);
}

bool cEngine::GetQuit() { return mQuit; }
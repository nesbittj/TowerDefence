/*
jonathan nesbitt
20/05/14

engine class 
initialises and handels engine
singleton class
*/

#pragma once

//#include <Windows.h>
//#undef LoadBitmap //stops windows.h defining as LoadBitmapA

#include <SDL.h>
#include <SDL_mixer.h>
#include "tinyxml2\tinyxml2.h"

#include "cRenderer.h"
#include "cLogger.h"
#include "cInput.h"
#include "cGUI.h"
#include "cTimer.h"

#include "cPlayer.h"
#include "cEntity.h"
#include "cTowerController.h"
#include "cEnemyController.h"
#include "cArena.h"

class cEngine
{
private:
	static cEngine* mInstance;
	cEngine() {};
	cRenderer* mRen;
	cLogger* mLog;
	cInput* mInput;
	cGUInamepsace::cGUI* mGUI;
	cGUInamepsace::panel* mPausePanel;
	cGUInamepsace::panel* mHUD;

	SDL_Event mEvent;
	bool mPaused;
	bool mQuit;

	cPlayer mPlayer;
	cTowerController mTowerController;
	cEnemyController mEnemyController;
	cArena* mArena;

public:
	static cEngine* Instance();
	int Init();
	int CleanUp();

	void Update();
	void UpdateEvents();
	void Render();

	bool GetQuit();
};


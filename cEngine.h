/*
jonathan nesbitt
20/05/14

engine class 
initialises and handels engine
singleton class
*/

#pragma once

#include <SDL.h>
#include "tinyxml2.h"

#include "cRenderer.h"
#include "cLogger.h"
#include "cInput.h"
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
	SDL_Window* mWindow;
	cRenderer* mRen;
	cLogger* mLog;
	cInput* mInput;

	SDL_Texture* mTexture;
	SDL_Event mEvent;
	bool mQuit;
	bool mRender;
	bool mUpdate;

	cPlayer mPlayer;
	cTowerController mTowerController;
	cEnemyController mEnemyController;
	cArena* mArena;
	
	void CapFrameRate();
	Uint32 CalcAvgFPS();
	void CapUpdateRate();
	Uint32 CalcAvgUpdates();

public:
	static cEngine* Instance();
	int Init();
	int CleanUp();
	int LoadConfigFromFile(const char* _filename);

	void Update();
	void Render();

	bool GetQuit();
	
	cTimer mFPSTimer;
	cTimer mFPSCap;
	Uint32 mCountedFrames;
	Uint32 mAvgFPS;
	int SCREEN_FPS;
	int SCREEN_TICKS_PER_FRAME;// = 1000 / SCREEN_FPS;

	cTimer mUpdatesCap;
	int UPDATE_FREQ;// = SCREEN_FPS * 2;
	int UPDATE_TICKS_PER_FRAME;// = 1000 / UPDATE_FREQ;
	Uint32 mCountedUpdates;
	Uint32 mAvgUpdates;
	
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	static const short LEVEL_WIDTH = 1280;
	static const short LEVEL_HIEGHT = 960;
	static const Uint32 LEVEL_GRID_SIZE = 32;

	void UpdateCamera();


};


/*
jonathan nesbitt
20/05/14

engine class 
initialises and handels engine
singleton class
*/

#pragma once

#include <SDL.h>
#include <iostream>
#include "cRenderer.h"
#include "cLogger.h"
#include "cInput.h"
#include "cSceneNode.h"
#include "cTimer.h"
#include "cCamera.h"
#include "cPlayer.h"
#include "cTowerController.h"
#include "cEnemyController.h"
#include "cCore.h"

#include <sstream>

using namespace std;

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

	cSceneNode* mNodeRoot;

	cPlayer mPlayer;
	cTowerController mTowerController;
	cEnemyController mEnemyController;
	cCore* mCore;
	
	void CapFrameRate();
	Uint32 CalcAvgFPS();
	void CapUpdateRate();
	Uint32 CalcAvgUpdates();

public:
	static cEngine* Instance();
	int Init();
	int CleanUp();

	void Update();
	void Render();

	bool GetQuit();
	
	cTimer mFPSTimer;
	cTimer mFPSCap;
	Uint32 mCountedFrames;
	Uint32 mAvgFPS;
	static const Uint32 SCREEN_FPS = 60;
	static const Uint32 SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

	cTimer mUpdatesCap;
	static const Uint32 UPDATE_FREQ = SCREEN_FPS * 2;
	static const Uint32 UPDATE_TICKS_PER_FRAME = 1000 / UPDATE_FREQ;
	Uint32 mCountedUpdates;
	Uint32 mAvgUpdates;
	
	static const short SCREEN_WIDTH = 640;
	static const short SCREEN_HEIGHT = 480;
	static const short LEVEL_WIDTH = 1280;
	static const short LEVEL_HIEGHT = 960;
	static const Uint32 LEVEL_GRID_SIZE = 32;

	void UpdateCamera();


};


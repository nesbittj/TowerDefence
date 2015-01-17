#pragma once

#include <SDL.h>
#include "cCamera.h"
#include "cRenderer.h"
#include "cMaths.h"
#include "cLogger.h"
#include "tinyxml2.h"
using namespace tinyxml2;

class cLevel
{
private:
	static const unsigned int NUM_TILES = 2;
	static const unsigned int GRID_SIZE = 32;

	float x,y;
	string mLevelTiles[GRID_SIZE]; //TODO: grid size should be set in this class
	SDL_Texture* mTiles[NUM_TILES];
	cRenderer* mRen;
	cLogger* mLog;

	bool UnloadBitmaps();

public:
	cLevel(float _x, float _y);
	~cLevel();

	bool Init();
	bool CleanUp();

	bool LoadLevelData(const char* _filename);
	const char* GetTyleType(float2 _pos);
	void Draw();

	inline float2 GetPos() const { float2 l_pos = { x,y }; return l_pos; }
	void SetPos(float _x, float _y);
	//inline void SetPos(float2 _pos) { SetPos(_pos.x,_pos.y); }
};
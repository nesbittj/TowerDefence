#pragma once

#include <SDL.h>
#include "cCamera.h"
#include "cRenderer.h"
#include "cMaths.h"

class cLevel
{
private:
	static const unsigned int NUM_TILES = 2;
	static const unsigned int GRID_SIZE = 32;

	float x,y;
	const char* mLevelTiles[GRID_SIZE][GRID_SIZE]; //TODO: grid size should be set oin this class
	SDL_Texture* mTiles[NUM_TILES];
	cCamera* mCamera;
	cRenderer* mRen;

	bool LoadBitmaps(const char* _data);
	bool UnloadBitmaps();

public:
	cLevel(float _x, float _y);
	~cLevel();

	bool Init(cCamera* _camera);
	bool CleanUp();

	bool LoadLevel(const char* _filename);
	const char* GetTyleType(float2 _pos);
	void Draw();

	inline float2 GetPos() const { float2 l_pos = { x,y }; return l_pos; }
	inline void SetPos(float _x, float _y);
	inline void SetPos(float2 _pos) { SetPos(_pos.x,_pos.y); }
};
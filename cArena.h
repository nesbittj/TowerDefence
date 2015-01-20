#pragma once

#include <SDL.h>
#include "cCamera.h"
#include "cRenderer.h"
#include "cMaths.h"
#include "cLogger.h"
#include "tinyxml2.h"
using namespace tinyxml2;

class cArena
{
private:
	static const unsigned int NUM_TILES = 2;
	static const unsigned int GRID_SIZE = 32;

	float x,y;
	string mArenaTiles[GRID_SIZE]; //TODO: grid size should be set in this class
	SDL_Texture* mTiles[NUM_TILES];
	cRenderer* mRen;
	cLogger* mLog;

	bool UnloadBitmaps();

	//Breadth First Search Variables
	vector<float2> mOpenList;
	vector<float2> mClosedList;
	float2 mNeighbours[4];

public:
	cArena(float _x, float _y);
	~cArena();

	bool Init();
	bool CleanUp();

	bool LoadArenaData(const char* _filename);
	const char* GetTyleType(float2 _pos);
	void Draw();
	vector<float2> BreadthFirst(const float2 _start, const float2 _target);
	void GraphNeighbours(float2 _current);
	bool CheckBounds(float2 _pos);

	static int Contains(vector<float2>* _vect, float2 _val);
	inline float2 GetPos() const { float2 l_pos = { x,y }; return l_pos; }
	void SetPos(float _x, float _y);
};
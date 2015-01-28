#pragma once

#include <SDL.h>

#include <map>
#include <stack>

#include "cCamera.h"
#include "cRenderer.h"

#include "cMaths.h"
#include "cLogger.h"

#include "cCore.h"

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
	cCore* mCore;

	bool UnloadBitmaps();

	//Breadth First Search Variables
	vector<JVector2> mOpenList;
	vector<JVector2> mClosedList;
	JVector2 mNeighbours[4];
	pair<int,int> mAdj[4];

	JVector2 mEnemyStartPos;
	JVector2 mEnemyTargetPos;
	JVector2 mEnemyExitPos;

public:
	cArena(float _x, float _y);
	~cArena();

	bool Init();
	bool CleanUp();

	bool LoadArenaData(const char* _filename);
	const char* GetTyleType(JVector2 _pos);
	void Update();
	void Draw();
	stack<pair<int,int>> BreadthFirst(const pair<int,int> _start, const pair<int,int> _target);
	void GraphNeighbours(pair<int,int> _u);
	bool CheckBounds(JVector2 _pos);
	bool CheckBounds(pair<int,int> _u);
	
	inline JVector2 GetPos() const { return JVector2(x,y); }
	void SetPos(float _x, float _y);
	cCore* GetCore() { return mCore; }
	const JVector2& const GetEnemyStartPos() const { return mEnemyStartPos; }
	const JVector2& const GetEnemyTargetPos() const { return mEnemyTargetPos; }
	const JVector2& const GetEnemyExitPos() const { return mEnemyExitPos; }
};
/*
jonathan nesbitt
01/02/15
cArena class
*/

#pragma once

#include <SDL.h>
#include <map>
#include <stack>
#include "tinyxml2.h"

#include "cCamera.h"
#include "cRenderer.h"
#include "cMaths.h"
#include "cLogger.h"

#include "cCore.h"


using namespace tinyxml2;

class cArena
{
private:
	static const unsigned int NUM_TILES = 2;
	static const unsigned int GRID_SIZE = 32;

	int mArenaWidth, mArenaHeight, mArenaWidthProdGrid, mArenaHeightProdGrid;
	string mArenaTiles[GRID_SIZE]; //TODO: grid size should be set in this class
	SDL_Texture* mTiles[NUM_TILES];
	cRenderer* mRen;
	cLogger* mLog;
	cCore* mCore;

	//Breadth First Search Variables
	vector<JVector2> mOpenList;
	vector<JVector2> mClosedList;
	JVector2 mNeighbours[4];
	pair<int,int> mAdj[4];

	JVector2 mEnemyStartPos;
	JVector2 mEnemyTargetPos;
	JVector2 mEnemyExitPos;

	bool LoadArenaData(const char* _filename);
public:
	cArena();
	~cArena();

	bool Init();
	bool CleanUp();

	void Update();
	void Draw();

	stack<pair<int,int>> BreadthFirst(const pair<int,int> _start, const pair<int,int> _target);	
	void GraphNeighbours(pair<int,int> _u);

	void CheckBounds(float* _x, float* _y);
	bool CheckBounds(pair<int,int> _u) const;

	inline int GetGridSize() const { return GRID_SIZE; }
	//arena width * grid size
	inline int GetArenaWidth() const { return mArenaWidthProdGrid; }
	//arena height * grid size
	inline int GetArenaHeight() const { return mArenaHeightProdGrid; }
	const char* GetTileType(JVector2 _pos);
	cCore* GetCore() { return mCore; }
	const JVector2& const GetEnemyStartPos() const { return mEnemyStartPos; }
	const JVector2& const GetEnemyTargetPos() const { return mEnemyTargetPos; }
	const JVector2& const GetEnemyExitPos() const { return mEnemyExitPos; }
};
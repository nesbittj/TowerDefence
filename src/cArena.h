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
#include "tmxparser.h"

#include "cCamera.h"
#include "cRenderer.h"
#include "cMaths.h"
#include "cLogger.h"

#include "cCore.h"

using namespace tinyxml2;
using namespace tmxparser;

class cArena
{
private:

	enum { TILE_EMPTY, TILE_WALL, TILE_CORE, TILE_START, TILE_EXIT } ;

	cRenderer* mRen;
	cLogger* mLog;
	cCore* mCore;

	TmxMap mArena;
	SDL_Texture* mTilesSpriteSheet;

	JVector2 mEnemyStartPos;
	JVector2 mEnemyTargetPos;
	JVector2 mEnemyExitPos;

	//Breadth First Search Variables
	vector<JVector2> mOpenList;
	vector<JVector2> mClosedList;
	JVector2 mNeighbours[4];
	pair<int,int> mAdj[4];
	
	int LoadArenaData(const char* _filename);

public:
	cArena();
	~cArena();

	bool Init();
	bool CleanUp();

	void Update();
	void Draw();
	void DrawTile(int _x, int _y, int _tile_type, int _space);

	stack<pair<int,int>> BreadthFirst(const pair<int,int> _start, const pair<int,int> _target);	
	void GraphNeighbours(pair<int,int> _u);

	void CheckBounds(float* _x, float* _y);
	bool CheckBounds(pair<int,int> _u) const;

	//TODO: game only supports square tiles
	inline int GetGridSize() const { return mArena.tileWidth; }
	//arena width * grid size
	inline int GetArenaWidth() const { return mArena.width*mArena.tileWidth; }
	//arena height * grid size
	inline int GetArenaHeight() const { return mArena.height*mArena.tileHeight; }
	int GetTileType(JVector2 _pos);
	cCore* GetCore() { return mCore; }
	const JVector2& const GetEnemyStartPos() const { return mEnemyStartPos; }
	const JVector2& const GetEnemyTargetPos() const { return mEnemyTargetPos; }
	const JVector2& const GetEnemyExitPos() const { return mEnemyExitPos; }
};
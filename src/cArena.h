/*
jonathan nesbitt
01/02/15
cArena class
*/

#pragma once

#include <SDL.h>
#include <map>
#include <queue>
#include <stack>

#include "tinyxml2\tinyxml2.h"
#include "tmxparser\tmxparser.h"

#include "cCamera.h"
#include "cRenderer.h"
#include "cInput.h"
#include "cMaths.h"
#include "cLogger.h"

using namespace tinyxml2;
using namespace tmxparser;
	
typedef enum { TILE_EMPTY, TILE_WALL, TILE_TOWER, TILE_START, TILE_EXIT, TILE_TYPE_RESET } ARENA_TILE_TYPE;

class cArena
{
private:
	cRenderer* mRen;
	cLogger* mLog;
	cInput* mInput;

	TmxMap mArena;
	SDL_Texture* mTilesSpriteSheet;

	JVector2 mEnemyStartPos;
	JVector2 mEnemyExitPos;

	int LoadArenaData(const char* _filename);
	
	map<pair<int,int>,pair<int,int>> mParent;
	stack<pair<int,int>> mPath;
	pair<int,int> mAdj[4];
	void GraphNeighbours(pair<int,int> _u, int _vanila = 0);

	Uint32 mArenaEditMode;
	bool mUpdateBFS;

public:
	bool Init();
	bool CleanUp();

	void Update();
	void Draw();
	void DrawTile(int _x, int _y, int _tile_type, int _space);

	bool BreadthFirst(const pair<int,int> _start, const pair<int,int> _target);
	JVector2 GetPathParent(int _x, int _y);
	JVector2 GetPathParent(JVector2 _pos) { return GetPathParent((int)_pos.x,(int)_pos.y); }
	/*not in use, use GetPathParent() - returns pointer to path data structure*/
	stack<pair<int,int>> GetPath() { return mPath; }

	void CheckWorldBounds(float* _x, float* _y);
	bool CheckIndexBounds(pair<int,int> _u) const;

	//TODO: game only supports square tiles
	inline int GetGridSize() const { return mArena.tileWidth; }
	//arena width * grid size
	inline int GetArenaWidth() const { return mArena.width*mArena.tileWidth+mArena.tileWidth; }
	//arena height * grid size
	inline int GetArenaHeight() const { return mArena.height*mArena.tileHeight+mArena.tileHeight; }
	inline JVector2 GetEnemyStartPos() const { return mEnemyStartPos; }
	inline JVector2 GetEnemyExitPos() const { return mEnemyExitPos; }

	ARENA_TILE_TYPE GetTileType(int _x, int _y, int _vanila = 0);
	ARENA_TILE_TYPE GetTileType(JVector2 _pos, int _vanila = 0) { return GetTileType((int)_pos.x,(int)_pos.y,_vanila); }
	void SetTileType(int _x, int _y, int _tile_type);
	void SetTileType(JVector2 _pos, int _tile_type) { SetTileType((int)_pos.x,(int)_pos.y,_tile_type); }
};
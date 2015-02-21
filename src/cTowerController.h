#pragma once

#include <SDL.h>
#include <SDL_mixer.h>
#include "cEntity.h"
#include "cInput.h"
#include "cRenderer.h"
#include "cTower.h"
#include "cLogger.h"
#include "cEnemy.h"
#include "cArena.h"

#include "tinyxml2.h"

using namespace tinyxml2;

class cTowerController
{
private:
	static const unsigned int mMaxTowerTypes = 10;
	static const unsigned int mMaxTowersInUse = 30;
	TowerData mTowersData[mMaxTowerTypes];
	cTower* mTowersInUse[mMaxTowersInUse];

	string mTowersFileLocation;
	Uint32 mTowerSelected;

	cInput* mInput;
	cRenderer* mRen;
	cLogger* mLog;
	cArena* mArena;

	int *mCursorX, *mCursorY;

public:
		
	Uint32 GetTowerSelected() const { return mTowerSelected; }
	void SetTowerSelected(Uint32 _tower);

	cTowerController();
	~cTowerController();

	bool Init(cArena* _arena, int* _cursor_x, int* _cursor_y);
	bool CleanUp();

	void Update(cEnemy** const _enemies_hit, int size_of_array);
	void DrawTowersInUse();
	void DrawTower(Uint32 _x, Uint32 _y, Uint32 _tower, Uint32 _space = WORLD_SPACE);
	void DrawTowerText(Uint32 _x, Uint32 _y, Uint32 _tower, SDL_Color _col, Uint32 _space = WORLD_SPACE);
	void AddTower(Uint32 _x, Uint32 _y, Uint32 _tower);
	void RemoveTower(Uint32 _x, Uint32 _y);
	bool LoadTowersData();
};

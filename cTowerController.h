#pragma once

#include <SDL.h>
#include "cEntity.h"
#include "cInput.h"
#include "cRenderer.h"
#include "cPlayer.h"
#include "cTower.h"
#include "cLogger.h"

#include "tinyxml2.h"

using namespace tinyxml2;

class cTowerController
{
private:
	TowerData mTowersData[10];
	cTower* mTowersInUse[30];

	string mTowersFileLocation;
	Uint32 mTowerSelected;
	const Uint32* mGridSize;

	cInput* mInput;
	cRenderer* mRen;
	cLogger* mLog;
	const cPlayer* mPlayer;

public:
		
	Uint32 GetTowerSelected() const { return mTowerSelected; }
	void SetTowerSelected(Uint32 _tower);

	cTowerController();
	~cTowerController();

	bool Init(const Uint32* _grid_size, const cPlayer* _player);
	void Update();
	void DrawTowersInUse();
	void DrawTower(Uint32 _x, Uint32 _y, Uint32 _tower, Uint32 _space = WORLD_SPACE);
	void DrawTowerText(Uint32 _x, Uint32 _y, Uint32 _tower, SDL_Color _col, Uint32 _space = WORLD_SPACE);
	void AddTower(Uint32 _x, Uint32 _y, Uint32 _tower);
	bool LoadTowersData();
};

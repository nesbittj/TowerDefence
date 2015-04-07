#pragma once

#include <SDL.h>
#include <SDL_mixer.h>
#include "cEntity.h"
#include "cInput.h"
#include "cRenderer.h"
#include "cTower.h"
#include "cLogger.h"
#include "cEnemy.h"
#include "cPlayer.h"
#include "cGUI.h"

#include "tinyxml2\tinyxml2.h"

using namespace tinyxml2;

class cTower;

class cTowerController
{
private:
	static const unsigned int mMaxTowerTypes = 10;
	static const unsigned int mMaxTowersInUse = 30;
	TowerData mTowersData[mMaxTowerTypes];
	cTower* mTowersInUse[mMaxTowersInUse]; //TODO: use a more appropriate container, to be able to search by key

	string mTowersFileLocation;
	//Uint32 mTowerSelected;

	cInput* mInput;
	cRenderer* mRen;
	cLogger* mLog;
	cGUInamepsace::cGUI* mGUI;
	cGUInamepsace::panel* mEditTowerPanel;
	cGUInamepsace::panel* mAddTowerPanel;
	cArena* mArena;
	cPlayer* mPlayer;

	bool cTowerController::IsTileClear(Uint32 _x, Uint32 _y);

	Sint32 mAddTowerX;
	Sint32 mAddTowerY;

public:
	bool mTowerEditMode;
		
	//Uint32 GetTowerSelected() const { return mTowerSelected; }
	//void SetTowerSelected(Uint32 _tower);

	cTowerController();
	~cTowerController();

	bool Init(cArena* _arena, cPlayer* _player);
	bool CleanUp();

	void Update(cEnemy** const _enemies, int size_of_array);
	void DrawTowersInUse();
	void DrawTower(float _x, float _y, Uint32 _tower, Uint32 _space = WORLD_SPACE);
	void DrawTowerText(float _x, float _y, Uint32 _tower, SDL_Color _col, Uint32 _space = WORLD_SPACE);
	bool AddTower(Uint32 _x, Uint32 _y, Uint32 _tower);
	bool AddTowerCallback(int _tower);
	bool RemoveTower(Uint32 _x, Uint32 _y);
	cTower* GetTower(JVector2 _pos);
	cTower* GetTower(Uint32 _x, Uint32 _y) { return GetTower(JVector2((float)_x,(float)_y)); }
	bool LoadTowersData();
	
	void Pause();
	void UnPause();
};

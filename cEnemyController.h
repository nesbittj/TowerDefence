#pragma once

#include <SDL.h>
#include "cEntity.h"
#include "cInput.h"
#include "cRenderer.h"
#include "cPlayer.h"
#include "cLogger.h"
#include "cEnemy.h"
#include "cTimer.h"

#include "tinyxml2.h"

using namespace tinyxml2;

class cEnemyController
{
private:
	EnemyData mEnemiesData[10];
	cEnemy* mEnemiesAlive[30];
	
	cTimer mEnemySpawnTimer;
	string mEnemyFileLocation;
	Uint32 mGridSize;

	cInput* mInput;
	cRenderer* mRen;
	cLogger* mLog;

public:

	cEnemyController();
	~cEnemyController();

	bool Init(const Uint32 _grid_size);
	bool CleanUp();

	void Update();
	void DrawEnemies();
	void DrawEnemy(Uint32 _x, Uint32 _y, Uint32 _enemy, Uint32 _space = WORLD_SPACE);
	void DrawEnemyText(Uint32 _x, Uint32 _y, Uint32 _enemy, SDL_Color _col, Uint32 _space = WORLD_SPACE);
	void AddEnemy(Uint32 _x, Uint32 _y, Uint32 _enemy);
	void RemoveEnemy(Uint32 _enemy);
	bool LoadEnemyData();

	cEnemy** const GetEnemies() { return mEnemiesAlive; }
};

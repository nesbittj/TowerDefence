#pragma once

#include <SDL.h>
#include "cEntity.h"
#include "cInput.h"
#include "cRenderer.h"
#include "cPlayer.h"
#include "cLogger.h"
#include "cEnemy.h"
#include "cTimer.h"
#include "cArena.h"
#include "tinyxml2.h"

using namespace tinyxml2;

class cEnemyController
{
private:
	static const unsigned int mMaxEnemiesAlive = 30;
	static const unsigned int mMaxEnemyTypes = 10;
	EnemyData mEnemiesData[mMaxEnemyTypes];
	cEnemy* mEnemiesAlive[mMaxEnemiesAlive];
	
	cTimer mEnemySpawnTimer;
	string mEnemyFileLocation;
	Uint32 mGridSize;
	JVector2 mEnemyStartPos;
	JVector2 mEnemyExitPos;
	vector<JVector2> mEnemyPath;

	cInput* mInput;
	cRenderer* mRen;
	cLogger* mLog;
	cArena* mArena;

public:

	cEnemyController();
	~cEnemyController();

	bool Init(const Uint32 _grid_size, cArena* _arena);
	bool CleanUp();

	void Update(JVector2 _target);
	void DrawEnemies();
	void DrawEnemy(Uint32 _x, Uint32 _y, Uint32 _enemy, Uint32 _space = WORLD_SPACE);
	void DrawEnemyText(Uint32 _x, Uint32 _y, Uint32 _enemy, SDL_Color _col, Uint32 _space = WORLD_SPACE);
	void AddEnemy(Uint32 _x, Uint32 _y, Uint32 _enemy);
	void RemoveEnemy(Uint32 _enemy);
	bool LoadEnemyData();

	cEnemy** const GetEnemies() { return mEnemiesAlive; }
	inline int GetMaxEnemies() const { return mMaxEnemiesAlive; }
};

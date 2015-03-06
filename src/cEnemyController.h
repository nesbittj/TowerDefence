#pragma once

#include <SDL.h>

#include "cRenderer.h"
#include "cPlayer.h"

#include "cInput.h"
#include "cLogger.h"
#include "cTimer.h"
#include "cMaths.h"

#include "cEntity.h"
#include "cEnemy.h"
#include "cArena.h"

#include "tinyxml2\tinyxml2.h"

using namespace tinyxml2;

class cEnemyController
{
private:
	static const unsigned int mMaxEnemiesAlive = 100;
	static const unsigned int mMaxEnemyTypes = 10;
	EnemyData mEnemiesData[mMaxEnemyTypes];
	cEnemy* mEnemiesAlive[mMaxEnemiesAlive];
	Uint32 mNumEnemiesAlive;
	
	cTimer mEnemySpawnTimer;
	string mEnemyFileLocation;
	JVector2 mEnemyStartPos;

	cInput* mInput;
	cRenderer* mRen;
	cLogger* mLog;
	cArena* mArena;

public:
	cEnemyController();
	~cEnemyController();

	bool Init(cArena* _arena);
	bool CleanUp();

	void Update();
	void DrawEnemies();
	void AddEnemy(Uint32 _x, Uint32 _y, Uint32 _enemy_type);
	void RemoveEnemy(Uint32 _enemy);
	bool LoadEnemyData();

	cEnemy** const GetEnemies() { return mEnemiesAlive; }
	inline int GetMaxEnemies() const { return mMaxEnemiesAlive; }
	inline Uint32* GetNumEnemiesAlive() { return &mNumEnemiesAlive; }
	
	void cEnemyController::Pause() { mEnemySpawnTimer.Pause(); }
	void cEnemyController::UnPause() { mEnemySpawnTimer.Unpause(); }
};
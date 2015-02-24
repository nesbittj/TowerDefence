#pragma once

#include "cEntity.h"
#include "cTimer.h"
#include "cEnemy.h"

struct TowerData
{
	string mName;
	SDL_Texture* mBitmap;
	Mix_Chunk* mFireSound;
	float mDamage;
	int mRange;
	int mFireFreq;
	int mFireDuration;
};

class cTower : public cEntity
{
private:
	TowerData* mTowerData;
	cTimer mFireFreqTimer;
	cTimer mFireDurTimer;
	cTimer mFireTimer;
	bool mTimersOn;
	vector<JVector3> mFiringVerts;

	void Init(TowerData* _data);

	bool mFiring;
	int mCurrentTargetIndex;

public:
	cTower(Uint32 _x, Uint32 _y);
	~cTower();

	bool Init(SDL_Texture* _bitmap, TowerData* _data);
	bool CleanUp();

	void Update(cEnemy** const _enemies, int size_of_array);
	void Draw();

};


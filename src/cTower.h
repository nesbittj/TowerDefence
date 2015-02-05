#pragma once

#include "cEntity.h"
#include "cTimer.h"
#include "cEnemy.h"

struct TowerData
{
	string mName;
	SDL_Texture* mBitmap;
	float mDamage;
	int mRange;
	int mFireFreq;
	int mFireDuration;
};

class cTower : public cEntity
{
private:
	bool mFiring;
	TowerData* mTowerData;
	cTimer mFireFreqTimer;
	cTimer mFireDurTimer;
	vector<JVector3> mFiringVerts;
	float mFireDamage;

	void Init(TowerData* _data);

public:
	cTower(Uint32 _x, Uint32 _y);
	~cTower();

	bool Init(SDL_Texture* _bitmap, TowerData* _data);
	bool CleanUp();

	void Update(cEnemy** const _enemies, int size_of_array);
	void Draw();

	bool GetFiring() { return mFiring; }
};


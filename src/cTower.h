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
	Uint32 mRange;
	Uint32 mFireFreq;
	Uint32 mFireDuration;
	Uint32 mCost;
};

class cTower : public cEntity
{
private:
	TowerData* mTowerData;
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
	
	void Pause() { mFireTimer.Pause(); }
	void UnPause() { mFireTimer.Unpause(); }

};


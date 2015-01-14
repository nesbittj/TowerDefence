#pragma once

#include "centity.h"
#include "cTimer.h"

struct TowerData
{
	string mName;
	SDL_Texture* mBitmap;
	int mDamage;
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

	void Init(TowerData* _data);

public:
	cTower(Uint32 _x, Uint32 _y, Uint32 _grid_size);
	~cTower();

	bool Init(SDL_Texture* _bitmap, TowerData* _data);
	bool CleanUp();

	void Update();
	void Draw();
	bool TargetInRange(Uint32 _target[2]);
};


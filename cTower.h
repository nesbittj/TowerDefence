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
	cTower(Uint32 _x, Uint32 _y, char* _bitmap_filename, Uint32 _grid_size, TowerData* _data);
	cTower(Uint32 _x, Uint32 _y, SDL_Texture* _bitmap, Uint32 _grid_sizem, TowerData* _data);
	~cTower();

	void Update();
	void Draw();
	bool TargetInRange(Uint32 _target[2]);
};


#include "cTower.h"


cTower::cTower(Uint32 _x, Uint32 _y, Uint32 _grid_size) : cEntity(_x,_y,_grid_size)
{
	mFiring = false;
	mFiringVerts.push_back(JVector3());
	mFiringVerts.push_back(JVector3());
}

cTower::~cTower()
{
}

bool cTower::Init(SDL_Texture* _bitmap, TowerData* _data)
{
	mTowerData = _data;
	mFireFreqTimer = cTimer();
	mFireFreqTimer.start();
	mFireDurTimer = cTimer();
	if(!cEntity::Init(_bitmap)) return false;
	return true;
}

bool cTower::CleanUp()
{
	mFireDurTimer.stop();
	mFireFreqTimer.stop();
	mTowerData = NULL;
	if(!cEntity::CleanUp()) return false;
	return true;
}

/*
updates towers.
takes constant pointer to array of enemies,
enemies in range are fired appon.
second parameter is the number of elements
in the array passed in the fist param.
damage is done to enemy once every frame
*/
//TODO: timer should start when an enemy comes into range,
//TODO: however this would cause many more loops through _enemies
void cTower::Update(cEnemy** const _enemies, int size_of_array)
{
	bool l_freq = true;
	bool l_dur = true;
	
	if(mFireFreqTimer.getTicks() > mTowerData->mFireFreq)
	{
		l_freq = true;
		mFireFreqTimer.start();
	}
	else l_freq = false;

	if(mFireDurTimer.getTicks() < mTowerData->mFireDuration)
	{
		l_dur = true;
		if(!mFiring) mFireDurTimer.start();
	}
	else
	{
		l_dur = false;
		mFireDurTimer.stop();
	}

	if((!mFiring && l_freq)	|| (mFiring && l_dur))
	{
		mFiring = false;
		//TODO: make more efficient, could reduce number of calls/searches
		//TODO: consider reporting enemies hit then damaging them inside enemiesConstroller::Update()
		for(int i = 0; i < size_of_array; i++)
		{
			if(_enemies[i] != NULL)
			{
				JVector2 l_target(_enemies[i]->GetX(),_enemies[i]->GetY());
				JVector2 l_this_pos(x,y);
				if(cMaths::InRange(l_this_pos,l_target,mTowerData->mRange))
				{
					mFiringVerts[0] = JVector3(0,0,1);
					mFiringVerts[1] = JVector3(l_target.x,l_target.y,WORLD_SPACE);
					mFiring = true;
					_enemies[i]->Damage(mTowerData->mDamage);
					break;
				}
			}
		}
	}
	else mFiring = false;
}

void cTower::Draw()
{
	mRen->RenderTexture(mBitmap,x,y,NULL);
	if(mFiring)	
		mRen->RenderVerts(x,y,mFiringVerts);
	
	//firing timer text
	SDL_Color col = { 0,0,0 };
	mRen->RenderText(mFireFreqTimer.getTicks(),x+42,y,0,col,NULL);
	mRen->RenderText(mFireDurTimer.getTicks(),x+42,y+15,0,col,NULL);
	
}

#include "cTower.h"


cTower::cTower(Uint32 _x, Uint32 _y) : cEntity(_x,_y)
{
	mCurrentTargetIndex = -1;
	mFiring = false;
	mFiringVerts.push_back(JVector3());
	mFiringVerts.push_back(JVector3());
	mFiringVerts[0] = JVector3(0,0,1);
}

cTower::~cTower()
{
}

bool cTower::Init(SDL_Texture* _bitmap, TowerData* _data)
{
	mTowerData = _data;
	mFireFreqTimer = cTimer();
	mFireDurTimer = cTimer();
	mFireTimer = cTimer();
	mFireTimer.Start();
	//if(!mTowerData->mFireFreq && !mTowerData->mFireDuration) mTimersOn = false;
	//else
	//{
		//mTimersOn = true;
		//mFireFreqTimer.Start();
	//}
	if(!cEntity::Init(_bitmap)) return false;
	return true;
}

bool cTower::CleanUp()
{
	mFireDurTimer.Stop();
	mFireFreqTimer.Stop();
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
//TODO: dps calc still not correct
void cTower::Update(cEnemy** const _enemies, int size_of_array)
{
	//TODO: if statements could be more robust
	if((mFiring && mCurrentTargetIndex < 0) || (!mFiring && mFireTimer.GetTicks() > mTowerData->mFireFreq))
	{
		if(mCurrentTargetIndex < 0)
		{
			for(int i = 0; i < size_of_array; i++)
			{
				if(_enemies[i])
				{
					if(JVector2::Distance(JVector2(x,y),_enemies[i]->GetPos()) < mTowerData->mRange)
					{
						mCurrentTargetIndex = i;
						mFiringVerts[1] = JVector3(
							_enemies[mCurrentTargetIndex]->GetX(),
							_enemies[mCurrentTargetIndex]->GetY(),WORLD_SPACE);
						mFiring = true;
						mFireTimer.Start(); //maybe if firing only
						printf("search\n");
						break;
					}
				}
			}
			mFiring = false;
		}
		else
		{
			mFiring = true;
			mFireTimer.Start();
		}

		//TODO: may not want sound to play every fire, eg machine gun could be one long(er) track
		//if(mFiring && mTowerData->mFireSound) Mix_PlayChannel(-1,mTowerData->mFireSound,0);
	}

	if(mFiring)
	{
		if(mFireTimer.GetTicks() > mTowerData->mFireDuration)
		{
			mFireTimer.Start();
			mFiring = false;
		}
		else if(mCurrentTargetIndex < 0 || !_enemies[mCurrentTargetIndex]
		|| JVector2::Distance(JVector2(x,y),_enemies[mCurrentTargetIndex]->GetPos()) > mTowerData->mRange)
		{
			mCurrentTargetIndex = -1;
		}
		else
		{
			mFiringVerts[1] = JVector3(
				_enemies[mCurrentTargetIndex]->GetX(),
				_enemies[mCurrentTargetIndex]->GetY(),WORLD_SPACE);
			_enemies[mCurrentTargetIndex]->Damage(mTowerData->mDamage * mRen->GetDeltaTime());
		}
	}
}

void cTower::Draw()
{
	mRen->RenderTexture(mBitmap,x,y,NULL);
	if(mFiring && mCurrentTargetIndex >= 0)
	{
		mRen->RenderVerts(x,y,mFiringVerts);
	}
	
	//firing timer text
	SDL_Color col = { 0,0,0 };
	//mRen->RenderText(mFireFreqTimer.GetTicks(),x+42,y,0,col,NULL);
	//mRen->RenderText(mFireDurTimer.GetTicks(),x+42,y+15,0,col,NULL);
	mRen->RenderText(mFireTimer.GetTicks(),x+42,y,0,col,0);
	
}

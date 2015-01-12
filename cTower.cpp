#include "cTower.h"


cTower::cTower(Uint32 _x, Uint32 _y, char* _bitmap_filename, Uint32 _grid_size, TowerData* _data) : cEntity(_x,_y,_bitmap_filename,_grid_size)
{
	cTower(_x,_y,mBitmap,_grid_size,_data);
}

cTower::cTower(Uint32 _x, Uint32 _y, SDL_Texture* _bitmap, Uint32 _grid_size, TowerData* _data) : cEntity(_x,_y,_bitmap,_grid_size)
{
	mFiring = false;
	mTowerData = _data;
	mFireFreqTimer = cTimer();
	mFireFreqTimer.start();
	mFireDurTimer = cTimer();
	//mFireDurTimer.start();
	
	mFiringVerts.push_back(JVector3());
	mFiringVerts.push_back(JVector3());
}

cTower::~cTower()
{
	mFireDurTimer.stop();
	mFireFreqTimer.stop();
	mTowerData = NULL;
}

void cTower::Update()
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
		Uint32 target[2] = { mInput->GetMouseX(),mInput->GetMouseY() };
		if(TargetInRange(target))
		{
			mFiringVerts[0] = JVector3(0,0,1);
			mFiringVerts[1] = JVector3(target[0],target[1],WORLD_SPACE);
			mFiring = true;
		}
		else mFiring = false;
	}
	else mFiring = false;
}

void cTower::Draw()
{
	mRen->RenderTexture(mBitmap,x,y,NULL);
	if(mFiring) mRen->RenderVerts(x,y,mFiringVerts);
	/*
	//firing timer text
	SDL_Color col = { 0,0,0 };
	mRen->RenderText(mFireFreqTimer.getTicks(),x+42,y,0,col,NULL,SCREEN_SPACE);
	mRen->RenderText(mFireDurTimer.getTicks(),x+42,y+15,0,col,NULL,SCREEN_SPACE);
	*/
}

bool cTower::TargetInRange(Uint32 _target[2])
{
	if(abs((int)(x + mRen->GetCamera()->GetPos().x - _target[0])) > mTowerData->mRange) return false;
	if(abs((int)(y + mRen->GetCamera()->GetPos().y - _target[1])) > mTowerData->mRange) return false;
	return true;
}

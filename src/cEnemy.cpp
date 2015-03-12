#include "cEnemy.h"


cEnemy::cEnemy(Uint32 _x, Uint32 _y) : cEntity(_x,_y)
{
	mLives = 10;
}

cEnemy::~cEnemy()
{
}

bool cEnemy::Init(SDL_Texture* _bitmap, cArena* _arena, EnemyData* _data)
{
	if(!cEntity::Init(_bitmap)) return false;
	mArena = _arena;
	mEnemyData = _data;
	mLives = (float)_data->mStartingLives;
	mTransformStart = mTransformMid = mTransformTarget = JVector2(x,y);
	mTransformProgress = 0.f;
	return true;
}

bool cEnemy::CleanUp()
{
	mEnemyData = NULL;
	mArena = NULL;
	if(!cEntity::CleanUp()) return false;
	return true;
}

void cEnemy::Update()
{
	mTransformMid = JVector2::Lerp(mTransformStart,mTransformTarget,mEnemyData->mSpeed*mTransformProgress);
	if(mTransformMid == mTransformTarget) // could this target be missed???
	{
		if(mTransformTarget == mArena->GetEnemyExitPos() * (float)mArena->GetGridSize()) mLives = -1.f;
		mTransformStart = mTransformMid;
		mTransformTarget =  mArena->GetPathParent(mTransformMid) * (float)mArena->GetGridSize();		
		mTransformProgress = mRen->GetDeltaTime();
	}
	else
		mTransformProgress = cMaths::Clamp(mTransformProgress + mRen->GetDeltaTime(),0.f,1.f);
	
	SetPos(mTransformMid);
	mArena->CheckWorldBounds(&x,&y);
}

void cEnemy::Draw()
{
	SDL_Color col = { 0,0,0 };
	mRen->RenderTexture(mBitmap,x,y,NULL,WORLD_SPACE);
	mRen->RenderText((float)cMaths::Truncatef(mLives),x+5,y+8,0,col,NULL,WORLD_SPACE);
}

void cEnemy::Damage(float _value)
{
	mLives -= _value;
	if(mLives < 0) mLives = 0;
}
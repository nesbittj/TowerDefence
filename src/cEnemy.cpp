#include "cEnemy.h"


cEnemy::cEnemy(Uint32 _x, Uint32 _y) : cEntity(_x,_y)
{
	mLives = 10;
	x = _x; y = _y;
}

cEnemy::~cEnemy()
{
}

bool cEnemy::Init(SDL_Texture* _bitmap, cArena* _arena, EnemyData* _data, stack<pair<int,int>> _enemy_path)
{
	if(!cEntity::Init(_bitmap)) return false;
	mArena = _arena;
	mEnemyData = _data;
	mEnemyPath = _enemy_path;
	mLives = _data->mStartingLives;
	mTransformStart = mTransformTarget = JVector2(x,y);
	mTransformProgress = 0.f;
	mTransformPrecision = 0.01f;
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
	//TODO: profiling, when enemy reached end of path, CPU usage goes up!
	if(!mEnemyPath.empty())
	{
		float l_speed = 2*mRen->GetDeltaTime();
		JVector2 local_v(x,y);
		if((mTransformProgress < 1))//JVector2::Distance(local_v,mTransformTarget) > mTransformPrecision)
		{
			local_v = JVector2::Lerp(mTransformStart,mTransformTarget,mTransformProgress);
			mTransformProgress += l_speed;
			x = local_v.x; y = local_v.y;
			mArena->CheckBounds(&x,&y);
		}
		else
		{
			mEnemyPath.pop();
			mTransformProgress = l_speed;
			mTransformStart = local_v;
			if(!mEnemyPath.empty())
				mTransformTarget = JVector2(mEnemyPath.top().first,mEnemyPath.top().second)
				* mArena->GetGridSize();
		}
	}
}

void cEnemy::Draw()
{
	cEntity::Draw();
	
	SDL_Color col = { 0,0,0 };
	mRen->RenderText(x,x+34,y,0,col,0);
	mRen->RenderText(y,x+34,y+10,0,col,0);
}

void cEnemy::Damage(float _value)
{
	mLives -= _value;
	if(mLives < 0) mLives = 0;
}
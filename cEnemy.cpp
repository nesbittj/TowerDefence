#include "cEnemy.h"


cEnemy::cEnemy(Uint32 _x, Uint32 _y, Uint32 _grid_size) : cEntity(_x,_y,_grid_size)
{
	mLives = 10;
	x = _x; y = _y;
}

cEnemy::~cEnemy()
{
}

bool cEnemy::Init(SDL_Texture* _bitmap, EnemyData* _data)
{
	if(!cEntity::Init(_bitmap)) return false;
	mEnemyData = _data;
	mLives = _data->mStartingLives;
	return true;
}

bool cEnemy::CleanUp()
{
	if(!cEntity::CleanUp()) return false;
	return true;
}

void cEnemy::Update(float2 _target)
{
	if(_target.x < x) x -= mEnemyData->mSpeed;
	if(_target.x > x) x += mEnemyData->mSpeed;
	if(_target.y < y) y -= mEnemyData->mSpeed;
	if(_target.y > y) y += mEnemyData->mSpeed;
	mRen->GetCamera()->CheckLevelBounds(x,y);

	if(x > mRen->GetCamera()->level_w || mLives < 0)
		mLives = 0;
}

void cEnemy::Draw()
{
	cEntity::Draw();
	
	SDL_Color col = { 0,0,0 };
	mRen->RenderText(mLives,x+42,y,0,col,0);
}

void cEnemy::Damage(int _value)
{
	mLives -= _value;
	if(mLives < 0) mLives = 0;
	printf("%i\n",mLives);
}

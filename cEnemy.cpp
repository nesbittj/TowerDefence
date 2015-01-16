#include "cEnemy.h"


cEnemy::cEnemy(Uint32 _x, Uint32 _y, Uint32 _grid_size) : cEntity(_x,_y,_grid_size)
{
	mLives = 10;
}

cEnemy::~cEnemy()
{
}

bool cEnemy::Init(SDL_Texture* _bitmap, EnemyData* _data)
{
	if(!cEntity::Init(_bitmap)) return false;
	int l_y_pos = rand() % mRen->GetCamera()->screen_h;
	x = 0; y = l_y_pos;
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
	if(_target.x < x) x -= mEnemyData->mSpeed*0.05;
	if(_target.x > x) x += mEnemyData->mSpeed*0.05;
	if(_target.y < y) y -= mEnemyData->mSpeed*0.05;
	if(_target.y > y) y += mEnemyData->mSpeed*0.05;
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

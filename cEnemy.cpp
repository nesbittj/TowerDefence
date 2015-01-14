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
	return true;
}

bool cEnemy::CleanUp()
{
	if(!cEntity::CleanUp()) return false;
	return true;
}

void cEnemy::Update()
{
	if(++x > mRen->GetCamera()->screen_w)
		mLives = 0;
}

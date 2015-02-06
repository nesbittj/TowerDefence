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
	if(!mEnemyPath.empty())
	{
		JVector2 l_0(x,y);
		JVector2 l_t(mEnemyPath.top().first,mEnemyPath.top().second);
		l_t *= mArena->GetGridSize();
		if(l_0 == l_t) mEnemyPath.pop();
		JVector2 l_r = JVector2::Lerp(l_0,l_t,1);
		x += l_r.x; y += l_r.y;
		mArena->CheckBounds(&x,&y);
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
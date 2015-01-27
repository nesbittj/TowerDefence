#include "cEnemy.h"


cEnemy::cEnemy(Uint32 _x, Uint32 _y, Uint32 _grid_size) : cEntity(_x,_y,_grid_size)
{
	mLives = 10;
	x = _x; y = _y;
}

cEnemy::~cEnemy()
{
}

bool cEnemy::Init(SDL_Texture* _bitmap, EnemyData* _data, stack<pair<int,int>> _enemy_path)
{
	if(!cEntity::Init(_bitmap)) return false;
	mEnemyData = _data;
	mEnemyPath = _enemy_path;
	mLives = _data->mStartingLives;
	return true;
}

bool cEnemy::CleanUp()
{
	if(!cEntity::CleanUp()) return false;
	return true;
}

void cEnemy::Update()
{
	JVector2 l_0 = JVector2(x,y);
	JVector2 l_t = l_0;
	if(!mEnemyPath.empty())
	{
		l_t = JVector2(mEnemyPath.top().first,mEnemyPath.top().second);
		l_t *= mGridSize;
		if(l_0 == l_t) 
			mEnemyPath.pop();
		JVector2 l_r = JVector2::Lerp(l_0,l_t,1);
		printf("lerp x: %f, y: %f\n\n", l_r.x,l_r.y);
		x += l_r.x; y += l_r.y;
		//mRen->GetCamera()->CheckCameraBounds(x,y);
	}

	if(x > mRen->GetCamera()->level_w || mLives < 0)
		mLives = 0;
}

void cEnemy::Draw()
{
	cEntity::Draw();
	
	SDL_Color col = { 0,0,0 };
	mRen->RenderText(x,x+34,y,0,col,0);
	mRen->RenderText(y,x+34,y+10,0,col,0);
}

void cEnemy::Damage(int _value)
{
	mLives -= _value;
	if(mLives < 0) mLives = 0;
}

JVector2 cEnemy::PathFind()
{	
	/*
	JVector2 l_current_pos(x,y);
	int current_index = cArena::Contains(mEnemyPath,l_current_pos);
	printf("%i\n",current_index);
	if(current_index < 0)
	{
		JVector2 not_found(-1,-1);
		return not_found;
	}

	//if(current_index+1 >= mEnemyPath->GetSize()) return mEnemyPath)[0];
	return (*mEnemyPath)[current_index+1];
	*/

	JVector2 l_current_pos(x,y);


	return JVector2();
}
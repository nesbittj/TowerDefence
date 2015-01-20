#include "cEnemy.h"


cEnemy::cEnemy(Uint32 _x, Uint32 _y, Uint32 _grid_size) : cEntity(_x,_y,_grid_size)
{
	mLives = 10;
	x = _x; y = _y;
}

cEnemy::~cEnemy()
{
}

bool cEnemy::Init(SDL_Texture* _bitmap, EnemyData* _data, vector<float2>* _enemy_path)
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
	float2 l_t = PathFind();
	JVector3 l_0 = JVector3(x,y,0);
	JVector3 l_1 = JVector3(l_t.x*32,l_t.y*32,0);
	JVector3 l_r = JVector3::Lerp(l_0,l_1,1);
	printf("x: %f\ny: %f\n",l_1.x,l_1.y);
	printf("x: %f\ny: %f\n\n",l_r.x,l_r.y);
	x += l_r.x; y += l_r.y;
	//mRen->GetCamera()->CheckCameraBounds(x,y);

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

float2 cEnemy::PathFind()
{
	/*
	if(_target.x < x) x -= mEnemyData->mSpeed;
	if(_target.x > x) x += mEnemyData->mSpeed;
	if(_target.y < y) y -= mEnemyData->mSpeed;
	if(_target.y > y) y += mEnemyData->mSpeed;
	*/
	
	float2 l_current_pos = { x,y };
	int current_index = cArena::Contains(mEnemyPath,l_current_pos);
	if(current_index+1 >= mEnemyPath->size()) return (*mEnemyPath)[0];
	return (*mEnemyPath)[current_index+1];
}
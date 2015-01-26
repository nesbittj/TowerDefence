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
	if(l_t.x >= 0 || l_t.y >= 0)
		mTargetPos = JVector3(l_t.x,l_t.y,0);
	JVector3 l_r = JVector3::Lerp(l_0,mTargetPos,1);
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
	float2 l_current_pos = { x,y };
	int current_index = cArena::Contains(mEnemyPath,l_current_pos);
	printf("%i\n",current_index);
	if(current_index < 0)
	{
		float2 not_found = { -1,-1 };
		return not_found;
	}

	if(current_index+1 >= mEnemyPath->size()) return (*mEnemyPath)[0];
	return (*mEnemyPath)[current_index+1];
}
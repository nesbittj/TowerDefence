#pragma once
#include "cEntity.h"
#include "cArena.h"
#include "stack"

struct EnemyData
{
	string mName;
	SDL_Texture* mBitmap;
	float mSpeed;
	int mStartingLives;
};

class cEnemy : public cEntity
{
public:

	cEnemy(Uint32 _x, Uint32 _y);
	~cEnemy();

	bool Init(SDL_Texture* _bitmap, cArena* _arena, EnemyData* _data, stack<pair<int,int>> _enemy_path);
	bool CleanUp();

	void Update();
	void Draw();
	void Damage(float _value);

private:
	JVector3 mTargetPos;
	cArena* mArena;
	EnemyData* mEnemyData;
	stack<pair<int,int>> mEnemyPath;
};


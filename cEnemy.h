#pragma once
#include "cEntity.h"
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
	EnemyData* mEnemyData;
	stack<pair<int,int>> mEnemyPath;

	cEnemy(Uint32 _x, Uint32 _y, Uint32 _grid_size);
	~cEnemy();

	bool Init(SDL_Texture* _bitmap, EnemyData* _data, stack<pair<int,int>> _enemy_path);
	bool CleanUp();

	void Update();
	void Draw();
	void Damage(float _value);
	JVector2 PathFind();

private:
	JVector3 mTargetPos;
};


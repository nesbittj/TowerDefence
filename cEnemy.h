#pragma once
#include "centity.h"

struct EnemyData
{
	string mName;
	SDL_Texture* mBitmap;
	int mSpeed;
	int mStartingLives;
};

class cEnemy : public cEntity
{
public:
	EnemyData* mEnemyData;

	cEnemy(Uint32 _x, Uint32 _y, Uint32 _grid_size);
	~cEnemy();

	bool Init(SDL_Texture* _bitmap, EnemyData* _data);
	bool CleanUp();

	void Update();
	void Draw();
	void Damage(int _value);
};


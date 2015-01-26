#pragma once
#include "centity.h"
#include "cQueue.h"

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
	cQueue<JVector2>* mEnemyPath;

	cEnemy(Uint32 _x, Uint32 _y, Uint32 _grid_size);
	~cEnemy();

	bool Init(SDL_Texture* _bitmap, EnemyData* _data, cQueue<JVector2>* _enemy_path);
	bool CleanUp();

	void Update();
	void Draw();
	void Damage(int _value);
	JVector2 PathFind();

private:
	JVector3 mTargetPos;
};


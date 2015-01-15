#pragma once

#include <SDL.h>
#include "cEntity.h"
#include "cEnemy.h"
#include "cMaths.h"
#include "tinyxml2.h"
using namespace tinyxml2;

class cCore : public cEntity
{
private:
	int mMaxLives;
public:
	cCore(Uint32 _x, Uint32 _y,Uint32 _grid_size);
	~cCore();

	bool virtual Init(SDL_Texture* _bitmap);
	bool virtual CleanUp();

	void Update(cEnemy** const _enemies, int size_of_array);
	bool LoadCoreData();
	void Heal();
	void Damage(int _damage);
};
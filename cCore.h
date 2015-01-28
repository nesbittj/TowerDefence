#pragma once

#include <SDL.h>

#include "cMaths.h"

#include "cEntity.h"

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

	void Update();
	bool LoadCoreData();
	void Heal();
	void Damage(float _damage);
	void Damage(JVector2 _origin, int _range, float _damage);
};
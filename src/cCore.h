#pragma once

#include <SDL.h>
#include "tinyxml2\tinyxml2.h"

#include "cMaths.h"

#include "cEntity.h"

using namespace tinyxml2;

class cCore : public cEntity
{
private:
	int mMaxLives;
public:
	cCore(Uint32 _x, Uint32 _y);
	~cCore();

	bool virtual Init(SDL_Texture* _bitmap);
	bool virtual CleanUp();

	void Update();
	bool LoadCoreData();
	void Heal();
	void Damage(float _damage);
	void Damage(JVector2 _origin, int _range, float _damage);
};
#pragma once
#include "centity.h"
class cEnemy : public cEntity
{
public:
	cEnemy(Uint32 _x, Uint32 _y, Uint32 _grid_size);
	~cEnemy();

	bool Init(SDL_Texture* _bitmap);
	bool CleanUp();
};


#pragma once

#include <SDL.h>
#include "cRenderer.h"
#include "cLogger.h"
#include "cInput.h"

class cEntity
{
protected:
	float x, y;
	Uint32 mLevel;
	float mLives;

	SDL_Texture* mBitmap;
	Uint32 mGridSize;
	cRenderer* mRen;
	cLogger* mLog;
	cInput* mInput;

public:
	float GetX() const { return x; }
	float GetY() const { return y; }
	JVector2 GetPos() const { return JVector2(x,y); }
	Uint32 GetLevel() const { return mLevel; }
	int GetLives() const { return mLives; }
	SDL_Texture* GetBitmap() const { return mBitmap; }

	void SetPos(Uint32 _x, Uint32 _y);
	void SetLevel(Uint32 l) { mLevel = l; }
	void SetBitmap(SDL_Texture* _bitmap) { mBitmap = _bitmap; }
	
	cEntity(Uint32 _x, Uint32 _y,Uint32 _grid_size);
	~cEntity();
	
	bool virtual Init(SDL_Texture* _bitmap);
	bool virtual CleanUp();

	void virtual Update();
	void virtual Draw(int _space = 0);
};
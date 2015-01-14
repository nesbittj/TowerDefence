#pragma once

#include <SDL.h>
#include "cRenderer.h"
#include "cLogger.h"
#include "cInput.h"

class cEntity
{
protected:
	Uint32 x, y;
	Uint32 mLevel;
	Uint32 mLives;

	SDL_Texture* mBitmap;
	Uint32 mGridSize;
	cRenderer* mRen;
	cLogger* mLog;
	cInput* mInput;

private:
	void Init(Uint32 _x, Uint32 _y, Uint32 _grid_size);

public:
	Uint32 GetX() const { return x; }
	Uint32 GetY() const { return y; }
	Uint32 GetLevel() const { return mLevel; }
	Uint32 GetLives() const { return mLives; }
	SDL_Texture* GetBitmap() const { return mBitmap; }

	void SetPos(Uint32 _x, Uint32 _y);
	void SetLevel(Uint32 l) { mLevel = l; }
	void SetBitmap(SDL_Texture* _bitmap) { mBitmap = _bitmap; }
	
	cEntity(Uint32 _x, Uint32 _y,Uint32 _grid_size);
	~cEntity();
	
	bool virtual Init(SDL_Texture* _bitmap);
	bool virtual CleanUp();

	void virtual Update();
	void virtual Draw();
};
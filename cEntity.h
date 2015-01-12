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
	SDL_Texture* GetBitmap() const { return mBitmap; }

	void SetPos(Uint32 _x, Uint32 _y);
	void SetLevel(Uint32 l) { mLevel = l; }
	bool LoadBitmap(char* filename);
	void SetBitmap(SDL_Texture* _bitmap) { mBitmap = _bitmap; }
	void UnloadBitmap();
	
	cEntity(Uint32 _x, Uint32 _y, char* _bitmap_filename, Uint32 _grid_size);
	cEntity(Uint32 _x, Uint32 _y, SDL_Texture* _bitmap, Uint32 _grid_size);
	~cEntity();

	void Update();
	void Draw();
};
#pragma once

#include <SDL.h>
#include <SDL_mixer.h>
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
	cRenderer* mRen;
	cLogger* mLog;
	cInput* mInput;

public:
	inline float GetX() const { return x; }
	inline float GetY() const { return y; }
	inline JVector2 GetPos() const { return JVector2(x,y); }
	Uint32 GetLevel() const { return mLevel; }
	int GetLives() const { return (int)mLives; }
	SDL_Texture* GetBitmap() const { return mBitmap; }

	inline void SetPos(float _x, float _y) { x = _x; y = _y; }
	inline void SetPos(JVector2 _pos) { SetPos(_pos.x,_pos.y); }
	void SetLevel(Uint32 l) { mLevel = l; }
	void SetBitmap(SDL_Texture* _bitmap) { mBitmap = _bitmap; }
	
	cEntity(Uint32 _x, Uint32 _y);
	~cEntity();
	
	bool virtual Init(SDL_Texture* _bitmap);
	bool virtual CleanUp();

	void virtual Update();
	void virtual Draw(int _space = 0);
};
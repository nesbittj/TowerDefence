#pragma once

#include <SDL.h>
#include "cMaths.h"
//#include "cArena.h"

class cCamera
{
private:
	float x, y;
	int screen_w, screen_h;
	float mMoveSpeed;
	//cArena* mArena;

public:
	cCamera(float _x, float _y, int _screen_w, int _screen_h);//, cArena* _arena);

	void UpdateRelative(float _x, float _y);
	void UpdateAbsolute(float _x, float _y);
	void CheckCameraBounds(float _oldX, float _oldY);
	void CheckLevelBounds(float _oldX, float _oldY);
	inline JVector2 GetPos() const { return JVector2(x,y); } 
	inline float GetMoveSpeed() const { return mMoveSpeed; }
};
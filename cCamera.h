#pragma once

#include <SDL.h>

struct float2 { float x; float y; };

class cCamera
{
public:
	float x, y;
	int screen_w, screen_h;
	int level_w, level_h;
	float mMoveSpeed;

	cCamera(float _x, float _y, int _screen_w, int _screen_h, int _level_w, int _level_h);

	void UpdateRelative(float _x, float _y);
	void UpdateAbsolute(float _x, float _y);
	void CheckBounds(float _oldX, float _oldY);
	inline float2 GetPos() const { float2 pos = {x,y}; return pos; } 
	inline float GetMoveSpeed() const { return mMoveSpeed; }
};
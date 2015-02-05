#pragma once

#include <SDL.h>
#include "cMaths.h"
#include "cInput.h"

class cCamera
{
private:
	float x, y;
	int screen_w_bound, screen_h_bound, mArena_w, mArena_h;
	float mMoveSpeed;
	SDL_Event* mEvent;
	cInput* mInput;

public:
	cCamera(float _x, float _y);

	int Init(SDL_Event* _event, cInput* _input,
		int _screen_w, int _screen_h, int _arena_w, int _arena_h);
	void CleanUp();

	void Update();
	//TODO: consolidate into one method
	void UpdateRelative(float _x, float _y);
	void UpdateAbsolute(float _x, float _y);
	void CheckCameraBounds(float _oldX, float _oldY);

	void SetArenaSize(int _arena_w, int _arena_h);
	inline JVector2 GetPos() const { return JVector2(x,y); } 
	inline float GetMoveSpeed() const { return mMoveSpeed; }
};
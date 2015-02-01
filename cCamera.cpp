#include "cCamera.h"

cCamera::cCamera(float _x, float _y, int _screen_w, int _screen_h)//, cArena* _arena)
{
	x = _x; y = _y;
	screen_w = _screen_w; screen_h = _screen_h;
	//mArena = _arena;
	mMoveSpeed = 1.0;//0.7;
}

/*
update camera position
values are added to current position,
movement is relative
*/
void cCamera::UpdateRelative(float _x, float _y)
{
	float oldX = _x; float oldY = _y;
	x += _x; y += _y;
	CheckCameraBounds(oldX,oldY);
}

/*
sets x and y position of camera
sets camera to an absolute position
*/
void cCamera::UpdateAbsolute(float _x, float _y)
{
	float oldX = _x; float oldY = _y;
	x = _x; y = _y;
	CheckCameraBounds(oldX,oldY);
}

/*
restrics camera to level height and width
TODO: consider: does cCamera need a pointer to cArena just for camera bounds?
*/
void cCamera::CheckCameraBounds(float _oldX, float _oldY)
{
	if(x > screen_w * 0.5) x = screen_w * 0.5;
	if(y > screen_h * 0.5) y = screen_h * 0.5;
	//if(x < screen_w - mArena->GetArenaWidth()) x = screen_w - mArena->GetArenaWidth();
	//if(y < screen_h - mArena->GetArenaHeight()) y = screen_h - mArena->GetArenaHeight();
	if(x < screen_w - 1280) x = screen_w - 1280;
	if(y < screen_h - 720) y = screen_h - 720;
}
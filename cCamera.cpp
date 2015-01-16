#include "cCamera.h"

cCamera::cCamera(float _x, float _y, int _screen_w, int _screen_h, int _level_w, int _level_h)
{
	x = _x; y = _y;
	screen_w = _screen_w; screen_h = _screen_h;
	level_w = _level_w; level_h = _level_h;
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
*/
void cCamera::CheckCameraBounds(float _oldX, float _oldY)
{
	if(x > 0.f) x = 0.f;
	if(y > 0.f) y = 0.f;
	if(x < screen_w - level_w) x = screen_w - level_w;
	if(y < screen_h - level_h) y = screen_h - level_h;
}

/*
restrics x,y to level height and width
*/
void cCamera::CheckLevelBounds(float _oldX, float _oldY)
{
	if(_oldX < 0.f) 
		_oldX = 0.f;
	if(_oldY < 0.f) 
		_oldY = 0.f;
	if(_oldX > level_w)
		_oldX = level_w;
	if(_oldY > level_h)
		_oldY = level_h;
}
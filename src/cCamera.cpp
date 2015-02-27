#include "cCamera.h"

/*
construct camera.
_x,_y startposition.
*/
cCamera::cCamera(float _x, float _y)
{
	x = _x; y = _y;
	mMoveSpeed = 1.0;
	mCursorX = mCursorY = 0;
}

int cCamera::Init(SDL_Event* _event, cInput* _input,
		int _screen_w, int _screen_h, int _arena_w, int _arena_h)
{
	mEvent = _event;
	mInput = _input;
	screen_w_bound = _screen_w / 2; screen_h_bound = _screen_h / 2;
	mArena_w = _arena_w; mArena_h = _arena_h;
	return 0;
}

void cCamera::CleanUp()
{
	mEvent = NULL;
	mInput = NULL;
}

void cCamera::Update()
{
	JVector2 new_cam(0,0);

	//calc new cam from wasd key presses
	if(mInput->GetKeyDown(SDLK_w)) new_cam.y += mMoveSpeed;
	if(mInput->GetKeyDown(SDLK_s)) new_cam.y -= mMoveSpeed;
	if(mInput->GetKeyDown(SDLK_a)) new_cam.x += mMoveSpeed;
	if(mInput->GetKeyDown(SDLK_d)) new_cam.x -= mMoveSpeed;
	
	//calc new cam with middle mouse + mouse move
	static JVector2 l_mouse_to_cam = JVector2();
	if(mInput->GetMouseButtonDown(CENTRE_MOUSE_BUTTON))
	{
		if(l_mouse_to_cam == JVector2())
		{
			l_mouse_to_cam = JVector2(GetPos() - JVector2((float)mInput->GetMouseX(),(float)mInput->GetMouseY()));;
		}

		JVector2 l_d = JVector2(GetPos() - JVector2((float)mInput->GetMouseX(),(float)mInput->GetMouseY()));
		new_cam.x = l_mouse_to_cam.x - l_d.x;
		new_cam.y = l_mouse_to_cam.y - l_d.y;
	}
	else
		l_mouse_to_cam.SetZero();
	
	//reset cam position to zero with backspace
	if(mInput->GetKeyDownRelease(SDLK_BACKSPACE))
	{
		new_cam.SetZero();
		UpdateAbsolute(new_cam.x,new_cam.y);
		return;
	}

	//update cam with new pos
	if(new_cam.x != 0 || new_cam.y != 0)
		UpdateRelative(new_cam.x,new_cam.y);
	
	mCursorX = cMaths::Round(mInput->GetMouseX() - (Uint32)x,32);
	mCursorY = cMaths::Round(mInput->GetMouseY() - (Uint32)y,32);
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
	if(x > (float)screen_w_bound) x = (float)screen_w_bound;
	if(y > (float)screen_h_bound) y = (float)screen_h_bound;
	if(x < (float)screen_w_bound - (float)mArena_w) x = (float)screen_w_bound - (float)mArena_w;
	if(y < (float)screen_h_bound - (float)mArena_h) y = (float)screen_h_bound - (float)mArena_h;
}

void cCamera::SetArenaSize(int _arena_w, int _arena_h)
{
	mArena_w = _arena_h; mArena_h = _arena_h;
}
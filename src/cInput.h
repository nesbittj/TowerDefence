/*
jonathan nesbitt
21/05/14

input handeler class
recieves and records human interface inputs
singleton class
*/

#pragma once

#include <SDL.h>
#include "cLogger.h"
#include "XInputButtons.h"

#define KEYCODE mEvent->key.keysym.sym
#define JOYCODE mEvent->cbutton.button
#define MOUSECODE mEvent->button.button

enum { LEFT_MOUSE_BUTTON , CENTRE_MOUSE_BUTTON, RIGHT_MOUSE_BUTTON
, MOUSE_BUTTON_FIVE, MOUSE_BUTTON_FOUR, SIZE_OF_MOUSE_BUTTON_ENUM };

class cInput
{
private:
	static cInput* mInstance;
	cInput(void);
	~cInput(void);

	cLogger* mLog;

	SDL_Event* mEvent;
	SDL_Joystick* mJoy;

	bool mKeysDown[256];
	bool mPrevKeysDown[256];
	bool mJoyButtonsDown[14];
	bool mPrevJoyButtonsDown[14];
	bool mMouseButtonDown[SIZE_OF_MOUSE_BUTTON_ENUM];
	bool mPrevMouseButtonDown[SIZE_OF_MOUSE_BUTTON_ENUM];

	int mJoyAxis[6];
	int mMouseX, mMouseY;
	float mJoyAxisDeadZone;
	
	int ProcessInputEvents();

public:
	static cInput* Instance();
	void SetEventPtr(SDL_Event* ev);

	bool GetKeyDown(int key);
	bool GetKeyDownRelease(int key);
	bool GetKeyDownNotRepeat(int key);
	bool GetKeyReleased(int key);

	bool GetJoyButtonDown(int button);
	bool GetJoyButtonDownRelease(int button);
	bool GetJoyButtonDownNotRepeat(int _button);
	bool GetJoyButtonReleased(int button);

	bool GetMouseButtonDown(int button);
	bool GetMouseButtonDownNotRepeat(int _button);
	bool GetMouseButtonDownRelease(int button);
	bool GetMouseButtonReleased(int button);

	inline int GetMouseX() { return mMouseX; }
	inline int GetMouseY() { return mMouseY; }
	int GetJoyAxis(int axis);

	int UpdateInputEvents();
	void UpdateOldKeys();
};


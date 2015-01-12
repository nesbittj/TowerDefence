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
	bool mMouseButtonDown[5];
	bool mPrevMouseButtonDown[5];

	int mJoyAxis[6];
	int mMouseX, mMouseY;
	float mJoyAxisDeadZone;
	
	int ProcessInputEvents();

public:
	static cInput* Instance();
	void SetEventPtr(SDL_Event* ev);

	bool GetKeyDown(int key);
	bool GetKeyDownRelease(int key);
	bool GetKeyReleased(int key);
	bool GetJoyButtonDown(int button);
	bool GetJoyButtonDownRelease(int button);
	bool GetJoyButtonReleased(int button);
	bool GetMouseButtonDown(int button);
	bool GetMouseButtonDownRelease(int button);
	bool GetMouseButtonReleased(int button);

	int GetMouseX();
	int GetMouseY();
	int GetJoyAxis(int axis);

	int UpdateInputEvents();
	void UpdateOldKeys();
};


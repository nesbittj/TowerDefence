#include "cInput.h"

cInput* cInput::mInstance = NULL;

cInput* cInput::Instance()
{
	if(!mInstance) mInstance = new cInput();
	return mInstance;
}

/*
initialises input class
sets all input values to false and zero
*/
cInput::cInput()
{
	mLog = cLogger::Instance();

	for(int i = 0; i < sizeof(mKeysDown); i++)
	{
		mKeysDown[i] = false;
		mPrevKeysDown[i] = false;
	}
	for(int i = 0; i < sizeof(mMouseButtonDown); i++)
	{
		mMouseButtonDown[i] = false;
		mPrevMouseButtonDown[i] = false;
	}

	for(int i = 0; i < sizeof(mJoyAxis)/sizeof(mJoyAxis[0]); i++) mJoyAxis[i] = 0;

	mMouseX = mMouseY = 0;

	mJoyAxisDeadZone = 10000.f;

	SDL_InitSubSystem(SDL_INIT_JOYSTICK);
	mJoy = NULL;
	if(SDL_NumJoysticks() > 0)
	{
		mJoy = SDL_JoystickOpen(0);
		if(!mJoy) mLog->LogSDLError("joystick init");
		
	}
	for(int i = 0; i < sizeof(mJoyButtonsDown); i++)
	{
		mJoyButtonsDown[i] = false;
		mPrevJoyButtonsDown[i] = false;
	}
}

cInput::~cInput()
{
	if(SDL_JoystickGetAttached(mJoy)) SDL_JoystickClose(mJoy);
	mJoy = NULL;

	mEvent = NULL;
	mLog = NULL;
}

void cInput::SetEventPtr(SDL_Event* ev) { mEvent = ev; }

/*
returns status of specific key
returns true if key is down
*/
bool cInput::GetKeyDown(int key)
{
	return mKeysDown[key];
}

/*
returns status of specific key
returns true if key is down
sets key to no longer be down
*/
bool cInput::GetKeyDownRelease(int key)
{
	if(mKeysDown[key])
	{
		mKeysDown[key] = false;
		return true;
	}
	return false;
}

/*
returns status of specific key
returns true only when the specified key
was down last tick but is now not down
*/
bool cInput::GetKeyReleased(int key)
{
	if(!mKeysDown[key] && mPrevKeysDown[key])
		return true;
	return false;
}

bool cInput::GetJoyButtonDown(int button)
{
	return mJoyButtonsDown[button];
}

bool cInput::GetJoyButtonDownRelease(int button)
{
	if(mJoyButtonsDown[button])
	{
		mJoyButtonsDown[button] = false;
		return true;
	}
	return false;
}

bool cInput::GetJoyButtonReleased(int button)
{
	if(!mJoyButtonsDown[button] && mPrevJoyButtonsDown[button])
		return true;
	return false;
}


bool cInput::GetMouseButtonDown(int button)
{
	return mMouseButtonDown[button];
}

bool cInput::GetMouseButtonDownRelease(int button)
{
	if(mMouseButtonDown[button])
	{
		mMouseButtonDown[button] = false;
		return true;
	}
	return false;
}

bool cInput::GetMouseButtonReleased(int button)
{
	if(!mMouseButtonDown[button] && mPrevMouseButtonDown[button])
		return true;
	return false;
}

/*
updates current key and button presses,
also updates old key presses.
called once per frame by engine->update
returns -1 is exit key is pressed.
*/
int cInput::UpdateInputEvents()
{
	while(SDL_PollEvent(mEvent) != 0)
	{
		if(ProcessInputEvents() == -1) return -1;
		if(GetKeyDownRelease(SDLK_ESCAPE)) return -1;
		if(GetJoyButtonDownRelease(JOY_START)) return -1;	
	}
	UpdateOldKeys();

	return 0;
}

/*
processes input events
updates local device status'
returns -1 on quit event
*/
int cInput::ProcessInputEvents()
{
	if(KEYCODE < 0 || KEYCODE > sizeof(mKeysDown)
		|| mEvent->key.repeat != 0) KEYCODE = 0;

	switch(mEvent->type)
	{
	case SDL_QUIT:
		return -1;
	case SDL_KEYDOWN:
		mKeysDown[KEYCODE] = true;
		break;
	case SDL_KEYUP:
		mKeysDown[KEYCODE] = false;
		break;
	case SDL_JOYBUTTONDOWN:
		mJoyButtonsDown[JOYCODE] = true;
		break;
	case SDL_JOYBUTTONUP:
		mJoyButtonsDown[JOYCODE] = false;
		break;
	case SDL_MOUSEBUTTONDOWN:
		mMouseButtonDown[MOUSECODE] = true;
		break;
	case SDL_MOUSEBUTTONUP:
		mMouseButtonDown[MOUSECODE] = false;
		break;
	}
	

	if(mJoy != NULL)
	{
		/*mJoyAxis[JOY_AXIS_LX] = SDL_JoystickGetAxis(mJoy,JOY_AXIS_LX);
		mJoyAxis[JOY_AXIS_LY] = SDL_JoystickGetAxis(mJoy,JOY_AXIS_LY);
		mJoyAxis[JOY_AXIS_RX] = SDL_JoystickGetAxis(mJoy,JOY_AXIS_RX);
		mJoyAxis[JOY_AXIS_RY] = SDL_JoystickGetAxis(mJoy,JOY_AXIS_RY);
		
		mJoyAxis[JOY_AXIS_LT] = SDL_JoystickGetAxis(mJoy,JOY_AXIS_LT);
		mJoyAxis[JOY_AXIS_RT] = SDL_JoystickGetAxis(mJoy,JOY_AXIS_RT);*/

		for(int i = JOY_AXIS_LX; i <= JOY_AXIS_RT; i++)
		{
			mJoyAxis[i] = SDL_JoystickGetAxis(mJoy,i);
			if(abs(mJoyAxis[i]) < mJoyAxisDeadZone) mJoyAxis[i] = 0.f;
		}
	}

	SDL_GetMouseState(&mMouseX,&mMouseY);

	return 0;
}

/*
copys current key status into
previouse key status
*/
void cInput::UpdateOldKeys()
{
	memcpy(mPrevKeysDown,mKeysDown,sizeof(mPrevKeysDown));
	memcpy(mPrevJoyButtonsDown,mJoyButtonsDown,sizeof(mPrevJoyButtonsDown));
	memcpy(mPrevMouseButtonDown,mMouseButtonDown,sizeof(mPrevMouseButtonDown));
}

/*
return mouse x and y position
*/
int cInput::GetMouseX() { return mMouseX; }
int cInput::GetMouseY() { return mMouseY; }

int cInput::GetJoyAxis(int axis) { return mJoyAxis[axis]; }
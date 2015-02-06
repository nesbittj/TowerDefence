#include "cTimer.h"

cTimer::cTimer()
{
	mStartTicks = 0;
	mPausedTicks = 0;
	mPaused = false;
	mStarted = false;
}

void cTimer::Start()
{
	mStarted = true;
	mPaused = false;
	mStartTicks = SDL_GetTicks();
	mPausedTicks = 0;
}

void cTimer::Stop()
{
	mStarted = false;
	mPaused = false;
	mStartTicks = 0;
	mPausedTicks = 0;
}

void cTimer::Pause()
{
	if( mStarted && !mPaused )
	{
		mPaused = true;
		mPausedTicks = SDL_GetTicks() - mStartTicks;
		mStartTicks = 0;
	}
}

void cTimer::Unpause()
{
	if( mStarted && mPaused )
	{
		mPaused = false;
		mStartTicks = SDL_GetTicks() - mPausedTicks;
		mPausedTicks = 0;
	}
}

unsigned int cTimer::GetTicks()
{
	unsigned int time = 0;
	if(mStarted)
	{
		if(mPaused) time = mPausedTicks;
		else time = SDL_GetTicks() - mStartTicks;
	}
	return time;
}
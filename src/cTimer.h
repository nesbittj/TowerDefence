#pragma once

#include <SDL.h>

class cTimer
{
    public:
        cTimer();

        void Start();
        void Stop();
        void Pause();
        void Unpause();

        unsigned int GetTicks();
        bool GetIsStarted() { return mStarted; }
        bool GetIsPaused() { return mPaused && mStarted; }

    private:
        unsigned int mStartTicks;
        unsigned int mPausedTicks;
        bool mPaused;
        bool mStarted;
};
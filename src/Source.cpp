/*
jonathan nesbitt
10/07/14

source file containing main()
game loop runs until quit value is set to true
*/

#include <windows.h>

#include <SDL.h>
#include "cEngine.h"

using namespace std;

INT64 PerfCountFrequency;
inline float GetSecondsElapsed(LARGE_INTEGER Start, LARGE_INTEGER End);
inline LARGE_INTEGER GetWallClock();

int main(int argc, char *argv[])
{
	cEngine* Engine = cEngine::Instance();
	if(!Engine->Init())
	{
		int MonitorRefreshHz = 60;
		int GameUpdatesHz = MonitorRefreshHz;
		float TargetSecondsPerFrame = 1.f / GameUpdatesHz;

		UINT DesiredSchedulerMS = 1;
		bool SleepIsGranuler = (timeBeginPeriod(DesiredSchedulerMS) == TIMERR_NOERROR);

		LARGE_INTEGER PerfCountFrequencyResult;
		QueryPerformanceFrequency(&PerfCountFrequencyResult);
		PerfCountFrequency = PerfCountFrequencyResult.QuadPart;
		LARGE_INTEGER LastCounter = GetWallClock();

		UINT64 LastCycleCount = __rdtsc();

		while(!Engine->GetQuit())
		{
			Engine->Update();
			Engine->Render();

			UINT64 EndCycleCount = __rdtsc();

			LARGE_INTEGER WorkCounter = GetWallClock();
			float WorkSecondsElased = GetSecondsElapsed(LastCounter,WorkCounter);

			float SecondsElapsedForFrame = WorkSecondsElased;
			if(SecondsElapsedForFrame < TargetSecondsPerFrame)
			{
				if(SleepIsGranuler)
				{
					DWORD SleepMS = (DWORD)(1000.f * (TargetSecondsPerFrame - SecondsElapsedForFrame));
					if(Sleep > 0)
						Sleep(SleepMS);
					printf("%d\n",SleepMS);
				}
				while(SecondsElapsedForFrame < TargetSecondsPerFrame)
				{
					SecondsElapsedForFrame = GetSecondsElapsed(LastCounter,GetWallClock());
				}
			}
			else
			{
			}			

			Engine->RenderPresent();

			LARGE_INTEGER EndCounter = GetWallClock();
			LastCounter = EndCounter;
			LastCycleCount = EndCycleCount;

			float MSPerFrame = 1000*GetSecondsElapsed(LastCounter,EndCounter);
			float FPS = 0.f; //PerfCountFrequency / CounterElapsed;
			float MCPF = 0.f; //(INT32)(CyclesElapsed / (1000 * 1000));

			char buffer[256];
			sprintf(buffer,"%fms/f, %ff/s, %fmc/f\n",MSPerFrame,FPS,MCPF);
			OutputDebugStringA(buffer);
		}
	}
	int l_result = Engine->CleanUp();
	Engine = NULL;

	return l_result;
}

inline float GetSecondsElapsed(LARGE_INTEGER Start, LARGE_INTEGER End)
{
	float result = ((float)(End.QuadPart - Start.QuadPart) / (float)PerfCountFrequency);
	return result;
}

inline LARGE_INTEGER GetWallClock()
{
	LARGE_INTEGER result;
	QueryPerformanceCounter(&result);
	return result;
}
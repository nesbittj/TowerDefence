/*
jonathan nesbitt
20/05/14

logger class
used to log errors in engine
singleton class
*/

#pragma once

#include <SDL.h>
#include <iostream>

using namespace std;

#define os cout

class cLogger
{
private:
	static cLogger* mInstance;
	cLogger(){};  // Private so that it can  not be called
public:
	static cLogger* Instance();

	void LogSDLError(const char* msg);
	void LogError(const char* msg);
};


#include "cLogger.h"

// Global static pointer used to ensure a single instance of the class.
cLogger* cLogger::mInstance = NULL;
 
/** This function is called to create an instance of the class.
Calling the constructor publicly is not allowed. The constructor
 is private and is only called by this Instance function.
 Only allow one instance of class to be generated.
*/
cLogger* cLogger::Instance()
{
	if (!mInstance) mInstance = new cLogger;
	return mInstance;
}

/*
outputs error using cout
appends recieved error string with SDL error message
*/
void cLogger::LogSDLError(const char* msg)
{
	os << msg << " error: " << SDL_GetError() << endl;
}

/*
outputs error using output steam
*/
void cLogger::LogError(const char* msg)
{
	os << msg << endl;
}

//TODO: use SDL_Log("text %d text", value);
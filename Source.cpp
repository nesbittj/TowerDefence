/*
jonathan nesbitt
10/07/14

source file containing main()
game loop runs until quit value is set to true
*/

#include <iostream>
#include <SDL.h>
#include <conio.h>
#include "cEngine.h"
#include "cLogger.h"

using namespace std;

int main(int argc, char *argv[])
{
	if(!cEngine::Instance()->Init())
	{
		while(!cEngine::Instance()->GetQuit())
		{
			cEngine::Instance()->Update();
			cEngine::Instance()->Render();
		}
	}
	cEngine::Instance()->CleanUp();

	return 0;
}

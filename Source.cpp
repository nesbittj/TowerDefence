/*
jonathan nesbitt
10/07/14

source file containing main()
game loop runs until quit value is set to true
*/

#include <SDL.h>
#include "cEngine.h"

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

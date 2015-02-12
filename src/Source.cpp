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

int main(int argc, char *argv[])
{
	cEngine* Engine = cEngine::Instance();
	if(!Engine->Init())
	{
		while(!Engine->GetQuit())
		{
			Engine->Update();
			Engine->Render();			
		}
	}
	int l_result = Engine->CleanUp();
	Engine = NULL;
	return l_result;
}
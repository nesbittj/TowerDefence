/*
jonathan nesbitt
20/05/14

renderer class
controles rendereing routines
singleton class
*/

#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <sstream>
#include <vector>
#include "cLogger.h"
#include "cMaths.h"
#include "cCamera.h"

using namespace std;

enum { WORLD_SPACE, SCREEN_SPACE };

class cRenderer
{
private:
	SDL_Renderer* mRenderer;
	static cRenderer* mInstance;
	cRenderer() {};

	cLogger* mLog;
	cCamera* mCamera;
	TTF_Font* mFont;
	SDL_Surface* mFontSurface;
	SDL_Texture* mFontTexture;

public:
	static cRenderer* Instance();
	int Init(SDL_Window* window);
	int CleanUp();

	cCamera* GetCamera() { return mCamera; }
	void SetCamera(cCamera* cam) { mCamera = cam; }
	void DrawRect(int x, int y, int w, int h, SDL_Color colour, int space = WORLD_SPACE);
	void DrawCircleFill(int _x, int _y, int _rad, SDL_Color _col, int _space = WORLD_SPACE);
	SDL_Texture* LoadTextureFromBMP(const char* bitmap, SDL_Renderer* ren);
	void RenderTexture(SDL_Texture* tex, int x, int y, SDL_Renderer* ren, int space = WORLD_SPACE);
	void RenderTexture(SDL_Texture* tex, int x, int y, SDL_Renderer* ren, int w, int h, int space = WORLD_SPACE);
	void RenderText(const char* string, int x, int y, int font, SDL_Color colour, SDL_Renderer* ren, int space = WORLD_SPACE);
	void RenderText(const int value, int x, int y, int font, SDL_Color colour, SDL_Renderer* ren, int space = WORLD_SPACE);
	void RenderVerts(int x, int y, const vector<JVector3>& verts, int space = WORLD_SPACE);
	void Present(SDL_Renderer* ren) const;
};


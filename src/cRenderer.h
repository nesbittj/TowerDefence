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

#include <sstream>
#include <vector>

#include "cLogger.h"
#include "cMaths.h"
#include "cCamera.h"

enum { WORLD_SPACE, SCREEN_SPACE };

class cRenderer
{
private:
	static cRenderer* mInstance;
	cRenderer() {};

	cLogger* mLog;
	SDL_Event* mEvent;

	SDL_Renderer* mRenderer;
	SDL_Color mColourDef;
	SDL_Color mColourBlack;
	TTF_Font* mFont;
	SDL_Surface* mFontSurface;
	SDL_Texture* mFontTexture;

	void SetDrawColour(SDL_Color _col, SDL_Renderer* _ren = NULL);
	void SetDrawColour(Uint8 _r, Uint8 _g, Uint8 _b, Uint8 _a, SDL_Renderer* _ren = NULL);
	SDL_Color GetDrawColour(SDL_Renderer* _ren = NULL) const;

public:
	cCamera* mCamera;

	static cRenderer* Instance();
	int Init(SDL_Window* _window, SDL_Event* _event);
	int CleanUp();
	int InitCamera(SDL_Event* _event, cInput* _input,
		int _screen_w, int _screen_h, int _arena_w, int _arena_h)
	{ return mCamera->Init(_event,_input,_screen_w,_screen_h,_arena_w,_arena_h); }
		
	void Update();
	int UpdateEvents();

	void DrawRect(float _x, float _y, int _w, int _h, SDL_Color _col, SDL_Renderer* _ren, int _space = WORLD_SPACE);
	//void DrawCircleFill(float _x, float _y, int _rad, SDL_Color _col, int _space = WORLD_SPACE);
	void RenderTexture(SDL_Texture* _tex, float _x, float _y, SDL_Renderer* _ren, int _space = WORLD_SPACE);
	void RenderTexture(SDL_Texture* _tex, float _x, float _y, SDL_Renderer* _ren, int _w, int _h, int _space = WORLD_SPACE);
	int RenderText(const char* _string, float _x, float _y, int _font, SDL_Color _col, SDL_Renderer* _ren, int _space = WORLD_SPACE);
	int RenderText(const int _value, float _x, float _y, int _font, SDL_Color _col, SDL_Renderer* _ren, int _space = WORLD_SPACE);
	void RenderVerts(float _x, float _y, const vector<JVector3>& _verts, bool _2D = true, int _space = WORLD_SPACE);
	void Present(SDL_Renderer* _ren);

	SDL_Texture* LoadBitmap(const char* _filename, SDL_Renderer* _ren = NULL);
	void UnloadBitmap(SDL_Texture* _bitmap);
};


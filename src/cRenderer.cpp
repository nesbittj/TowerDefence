#include "cRenderer.h"

cRenderer* cRenderer::mInstance = NULL;
cRenderer* cRenderer::Instance()
{
	if(!mInstance) mInstance = new cRenderer;
	return mInstance;
}

/*
initialises default renderer
renderer creation error returns -1.
TTF font init error returns -2.
TTF open font error returns -3.
success returns 0.
*/
int cRenderer::Init(SDL_Window* _window, SDL_Event* _event)
{
	mRenderer = NULL;
	mFont = NULL;
	mFontSurface = NULL;
	mFontTexture = NULL;
	mCamera = NULL;

	mLog = cLogger::Instance();
	mEvent = _event;

	mRenderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
	if(!mRenderer)
	{
		mLog->LogSDLError("SDL_CreateRenderer");
		return -1;
	}

	if(TTF_Init())
	{
		mLog->LogSDLError("TTF_Init");
		return -2;
	}

	mFont = TTF_OpenFont("assets/DejaVuSans.ttf",12);
	if(!mFont)
	{
		mLog->LogSDLError("TTF_Font_OpenFont");
		return -3;
	}

	//set defalt render colours
	mColourDef.r = mColourDef.g = mColourDef.b = 102; mColourDef.a = 255;
	mColourBlack.r = mColourBlack.g = mColourBlack.b = 0; mColourBlack.a = 255;
	SetDrawColour(mColourDef,mRenderer);

	mCamera = new cCamera(0,0);

	return 0;
}

/*
success returns 0.
*/
int cRenderer::CleanUp()
{
	if(mCamera) mCamera->CleanUp();
	mCamera = NULL;
	if(mFont) TTF_CloseFont(mFont);
	mFont = NULL;
	if(mFontSurface) SDL_FreeSurface(mFontSurface);
	mFontSurface = NULL;
	if(mFontTexture) SDL_DestroyTexture(mFontTexture);
	mFontTexture = NULL;

	if(mRenderer) SDL_DestroyRenderer(mRenderer);
	mRenderer = NULL;

	mEvent = NULL;
	mLog = NULL;

	return 0;
}

void cRenderer::Update()
{
	mCamera->Update();
}

int cRenderer::UpdateEvents()
{
	if (mEvent->type == SDL_WINDOWEVENT)
	{
		switch (mEvent->window.event)
		{
			//TODO: handel window resize events
		case SDL_WINDOWEVENT_RESIZED:
			{
			SDL_Log("Window %d resized to %dx%d",
				mEvent->window.windowID, mEvent->window.data1,
				mEvent->window.data2);
			} break;
		case SDL_WINDOWEVENT_EXPOSED:
			{
			SDL_Log("Window %d exposed", mEvent->window.windowID);
			} break;

		//TODO: might not need to record any of these events
		case SDL_WINDOWEVENT_SHOWN:
			{
			SDL_Log("Window %d shown", mEvent->window.windowID);
			} break;
		case SDL_WINDOWEVENT_HIDDEN:
			{
			SDL_Log("Window %d hidden", mEvent->window.windowID);
			} break;		
		case SDL_WINDOWEVENT_MOVED:
			{
			SDL_Log("Window %d moved to %d,%d",
					mEvent->window.windowID, mEvent->window.data1,
					mEvent->window.data2);
			} break;
		case SDL_WINDOWEVENT_MINIMIZED:
			SDL_Log("Window %d minimized", mEvent->window.windowID);
			break;
		case SDL_WINDOWEVENT_MAXIMIZED:
			SDL_Log("Window %d maximized", mEvent->window.windowID);
			break;
		case SDL_WINDOWEVENT_RESTORED:
			SDL_Log("Window %d restored", mEvent->window.windowID);
			break;
		case SDL_WINDOWEVENT_ENTER:
			SDL_Log("Mouse entered window %d",
					mEvent->window.windowID);
			break;
		case SDL_WINDOWEVENT_LEAVE:
			SDL_Log("Mouse left window %d", mEvent->window.windowID);
			break;
		case SDL_WINDOWEVENT_FOCUS_GAINED:
			SDL_Log("Window %d gained keyboard focus",
					mEvent->window.windowID);
			break;
		case SDL_WINDOWEVENT_FOCUS_LOST:
			SDL_Log("Window %d lost keyboard focus",
					mEvent->window.windowID);
			break;
		case SDL_WINDOWEVENT_CLOSE:
			SDL_Log("Window %d closed", mEvent->window.windowID);
			break;
		default:
			SDL_Log("Window %d got unknown event %d",
					mEvent->window.windowID, mEvent->window.event);
			break;
		}
	}
	if(mEvent->type == SDL_RENDER_TARGETS_RESET)
	{
		/**< The render targets have been reset */
	}

	return 0;
}

/*
sets draw color _col to renderer _ren.
leave _ren as NULL for default/primary renderer.
*/
void cRenderer::SetDrawColour(SDL_Color _col, SDL_Renderer* _ren)
{
	if(!_ren) _ren = mRenderer;
	if(SDL_SetRenderDrawColor(_ren,_col.r,_col.g,_col.b,_col.a) != 0)
		mLog->LogSDLError("cRenderer::SetDrawColour()");
}

/*
uses cRenderer::SetDrawColour(SDL_Color, SDL_Renderer*)
*/
void cRenderer::SetDrawColour(Uint8 _r, Uint8 _g, Uint8 _b, Uint8 _a, SDL_Renderer* _ren)
{
	SDL_Color l_temp = { _r,_g,_b,_a };
	SetDrawColour(l_temp,_ren);
}

/*
gets current draw colour from SDL_renderer* _ren.
set _Ren to NULL for default/primary renderer.
*/
SDL_Color cRenderer::GetDrawColour(SDL_Renderer* _ren) const
{
	if(!_ren) _ren = mRenderer;
	Uint8 l_col[4];
	if(SDL_GetRenderDrawColor(_ren,&l_col[0],&l_col[1],&l_col[2],&l_col[3]) != 0)
		mLog->LogSDLError("cRenderer::GetDrawColour()");
	SDL_Color l_result = { l_col[0],l_col[1],l_col[2],l_col[3] };
	return l_result;
}

/*
draws a rectangle at position x,y
with width and height w,h
and colour col
RenderDrawColor is set back to what it was before operation
*/
void cRenderer::DrawRect(float _x, float _y, int _w, int _h, SDL_Color _col, SDL_Renderer* _ren, int _space)
{
	if(!_ren) _ren = mRenderer;
	SDL_Color l_prevCol = GetDrawColour();
	SetDrawColour(_col);

	JVector2 camPos = mCamera->GetPos();
	if(_space == WORLD_SPACE/* && camPos.w > 0*/)
	{
		_x += camPos.x;
		_y += camPos.y;
	}
	SDL_Rect aRect = {(int)_x,(int)_y,_w,_h};
	if(SDL_RenderDrawRect(_ren,&aRect) != 0)
		mLog->LogSDLError("cRenderer::DrawRect()");

	SetDrawColour(l_prevCol);
}

/*
renders a texture at position x,y
renderer can be set to NULL then default renderer will be used
*/
void cRenderer::RenderTexture(SDL_Texture* _tex, float _x, float _y, SDL_Renderer* _ren, int _space)
{
	if(!_ren) _ren = mRenderer;
	JVector2 camPos = mCamera->GetPos();
	if(_space == WORLD_SPACE/* && camPos.w > 0*/)
	{
		_x += camPos.x;
		_y += camPos.y;
	}
	SDL_Rect l_rect = { (int)_x,(int)_y,0,0 };
	if(SDL_QueryTexture(_tex,NULL,NULL,&l_rect.w,&l_rect.h) != 0)
		mLog->LogSDLError("cRenderer::RenderTexture() QueryTexture()");
	if(SDL_RenderCopy(_ren,_tex,NULL,&l_rect) != 0)		
		mLog->LogSDLError("cRenderer::RenderTexture() RenderCopy()");
}

/*
renders a texture at position x,y
with width and height w,h
renderer can be set to NULL then default renderer will be used
*/
void cRenderer::RenderTexture(SDL_Texture* _tex, float _x, float _y, SDL_Renderer* _ren, int _w, int _h, int _space)
{
	if(!_ren) _ren = mRenderer;
	JVector2 camPos = mCamera->GetPos();
	if(_space == WORLD_SPACE/* && camPos.w > 0*/)
	{
		_x += camPos.x;
		_y += camPos.y;
	}
	SDL_Rect dst = { (int)_x,(int)_y,_w,_h };
	if(SDL_RenderCopy(_ren, _tex, NULL, &dst) != 0)
		mLog->LogSDLError("cRenderer::RenderTexture()");
}

/*
renders text at x,y with colour _col.
if renderer is NULL default renderer is used.
camera offset is passed on to RenderTexture() to avoid duplicating offset.
if font does not exist/ not initialised returns -1.
font surface error returns -2.
font texture error returns -3.
success returns 0.
*/
int cRenderer::RenderText(const char* _string, float _x, float _y, int _font,
						  SDL_Color _col, SDL_Renderer* _ren, int _space)
{
	if(!mFont) return -1;
	if(!_ren) _ren = mRenderer;

	mFontSurface = TTF_RenderText_Blended(mFont,_string,_col);
	if(!mFontSurface)
	{
		mLog->LogSDLError("TTF_RenderText_Blended");
		return -2;
	}

	mFontTexture = SDL_CreateTextureFromSurface(_ren,mFontSurface);
	if(!mFontTexture)
	{
		mLog->LogSDLError("SDL_CreateTextureFromSurface");
		return -3;
	}

	SDL_FreeSurface(mFontSurface);
	mFontSurface = NULL;
	RenderTexture(mFontTexture,_x,_y,_ren,_space);
	SDL_DestroyTexture(mFontTexture);
	return 0;
}

/*
uses and returns result of
RenderText(const char*,int,int,int,SDL_Color,SDL_Renderer*,int).
*/
int cRenderer::RenderText(const int _value, float _x, float _y, int _font,
						  SDL_Color _col, SDL_Renderer* _ren, int _space)
{
	std::stringstream textStream;
	textStream.str("");
	textStream << _value;
	return RenderText(textStream.str().c_str(),_x,_y,_font,_col,_ren,_space);
}

/*
render a polygon from vertices.
if _2D is true (default) z axis of verts will be used
to identify WORLD_SPACE vs SCREEN_SPACE.
*/
void cRenderer::RenderVerts(float _x, float _y, const vector<JVector3>& _verts, bool _2D, int _space)
{
	int num_verts = _verts.size();
	vector<SDL_Point> points;
	SDL_Color l_prev_col = GetDrawColour();

	JVector2 camPos = mCamera->GetPos();
	if(_space == WORLD_SPACE)
	{
		_x += camPos.x;
		_y += camPos.y;
	}
	for(int i = 0; i < num_verts; i++)
	{
		points.push_back(SDL_Point());
		if(_2D && _verts[i].z == WORLD_SPACE)
		{
			points.back().x = (int)(_verts[i].x + camPos.x);
			points.back().y = (int)(_verts[i].y + camPos.y);
		}
		else // LOCAL_SPACE
		{
			points.back().x = (int)(_x + _verts[i].x);
			points.back().y = (int)(_y + _verts[i].y);
		}
	}
	points.push_back(SDL_Point());
	points.back() = points[0];

	SetDrawColour(mColourBlack);
	if(SDL_RenderDrawLines(mRenderer,&points.front(),num_verts + 1) != 0)
		mLog->LogSDLError("cRenderer::RenderVerts()");
	SetDrawColour(mColourDef);
}

/*
presents / flips renderer (back buffer) to display
then clears renderer to RenderDrawColor
renderer can be set to NULL then default renderer will be used
*/
void cRenderer::Present(SDL_Renderer* _ren)
{
	if(!_ren) _ren = mRenderer;
	SDL_RenderPresent(_ren);
	SetDrawColour(mColourDef,_ren);
	if(SDL_RenderClear(_ren) != 0) mLog->LogSDLError("cRenderer::Present()");
}

/*
returns SDL_Texture* from a .bmp file
renderer can be set to NULL then default renderer will be used.
call cRenderer::UploadBitmap() when finished with texture.
*/
SDL_Texture* cRenderer::LoadBitmap(const char* _filename, SDL_Renderer* _ren)
{
	if(!_ren) _ren = mRenderer;
	SDL_Texture* local_texture = NULL;
	SDL_Surface* local_bitmap = NULL;

	local_bitmap = SDL_LoadBMP(_filename);
	if(!local_bitmap) mLog->LogSDLError("SDL_LoadBMP");
	else
	{
		if(SDL_SetColorKey(local_bitmap,SDL_TRUE,SDL_MapRGB(local_bitmap->format,255,0,255)) != 0)
			mLog->LogSDLError("crenderer::LoadTextureFromBMP() SDL_SetKey");
		local_texture = SDL_CreateTextureFromSurface(_ren,local_bitmap);
		if(!local_texture)
			mLog->LogSDLError("crenderer::LoadTextureFromBMP() SDL_CreateTextureFromSuface");
	}

	SDL_FreeSurface(local_bitmap);
	return local_texture;
}

/*
destroys bitmap and sets pointer to NULL
*/
void cRenderer::UnloadBitmap(SDL_Texture* _bitmap)
{
	if(_bitmap) SDL_DestroyTexture(_bitmap);
	_bitmap = NULL;
}
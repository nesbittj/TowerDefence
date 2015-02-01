#include "cRenderer.h"

cRenderer* cRenderer::mInstance = NULL;

cRenderer* cRenderer::Instance()
{
	if(!mInstance) mInstance = new cRenderer;
	return mInstance;
}

/*
initialises default renderer
sets default RenderDrawColor
*/
int cRenderer::Init(SDL_Window* window)
{
	mRenderer = NULL;
	mFont = NULL;
	mFontSurface = NULL;
	mFontTexture = NULL;
	mCamera = NULL;

	mLog = cLogger::Instance();


	mRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(!mRenderer)
	{
		mLog->LogSDLError("SDL_CreateRenderer");
		return -1;
	}

	if(TTF_Init())
	{
		mLog->LogSDLError("TTF_Init");
		return -1;
	}

	mFont = TTF_OpenFont("assets/DejaVuSans.ttf",12);
	if(!mFont)
	{
		mLog->LogSDLError("TTF_Font_OpenFont");
		return -1;
	}

	//set defalt render colours
	mColourDef.r = mColourDef.g = mColourDef.b = 102; mColourDef.a = 255;
	mColourBlack.r = mColourBlack.g = mColourBlack.b = 0; mColourBlack.a = 255;
	SetDrawColour(mColourDef,mRenderer);

	return 0;
}

int cRenderer::CleanUp()
{
	if(mFont) TTF_CloseFont(mFont);
	mFont = NULL;
	if(mFontSurface) SDL_FreeSurface(mFontSurface);
	mFontSurface = NULL;
	if(mFontTexture) SDL_DestroyTexture(mFontTexture);
	mFontTexture = NULL;

	if(mRenderer) SDL_DestroyRenderer(mRenderer);
	mRenderer = NULL;

	mCamera = NULL;
	mLog = NULL;

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
void cRenderer::DrawRect(int _x, int _y, int _w, int _h, SDL_Color _col, SDL_Renderer* _ren, int _space)
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
	SDL_Rect aRect = {_x,_y,_w,_h};
	if(SDL_RenderDrawRect(_ren,&aRect) != 0)
		mLog->LogSDLError("cRenderer::DrawRect()");

	SetDrawColour(l_prevCol);
}

/*
renders a texture at position x,y
renderer can be set to NULL then default renderer will be used
*/
void cRenderer::RenderTexture(SDL_Texture* _tex, int _x, int _y, SDL_Renderer* _ren, int _space)
{
	if(!_ren) _ren = mRenderer;
	SDL_Rect local_rect;
	JVector2 camPos = mCamera->GetPos();
	if(_space == WORLD_SPACE/* && camPos.w > 0*/)
	{
		_x += camPos.x;
		_y += camPos.y;
	}
	local_rect.x = _x;
	local_rect.y = _y;
	if(SDL_QueryTexture(_tex,NULL,NULL,&local_rect.w,&local_rect.h) != 0)
		mLog->LogSDLError("cRenderer::RenderTexture() QueryTexture() Error");
	if(SDL_RenderCopy(_ren,_tex,NULL,&local_rect) != 0)		
		mLog->LogSDLError("cRenderer::RenderTexture() RenderCopy() Error");
}

/*
renders a texture at position x,y
with width and height w,h
renderer can be set to NULL then default renderer will be used
*/
void cRenderer::RenderTexture(SDL_Texture* tex, int x, int y, SDL_Renderer* ren, int w, int h, int _space)
{
	if(!ren) ren = mRenderer;
	SDL_Rect dst;
	JVector2 camPos = mCamera->GetPos();
	if(_space == WORLD_SPACE/* && camPos.w > 0*/)
	{
		x += camPos.x;
		y += camPos.y;
	}
	dst.x = x;	dst.y = y;
	dst.w = w;	dst.h = h;
	SDL_RenderCopy(ren, tex, NULL, &dst);
}

/*
loads and creates a texture from a .bmp file
renderer can be set to NULL then default renderer will be used
*/
SDL_Texture* cRenderer::LoadTextureFromBMP(const char* bitmap, SDL_Renderer* ren)
{
	if(!ren) ren = mRenderer;
	SDL_Texture* local_texture = NULL;
	SDL_Surface* local_bitmap = NULL;

	local_bitmap = SDL_LoadBMP(bitmap);
	if(!local_bitmap) mLog->LogSDLError("SDL_LoadBMP");
	else
	{
		SDL_SetColorKey(local_bitmap,SDL_TRUE,SDL_MapRGB(local_bitmap->format,255,0,255));
		local_texture = SDL_CreateTextureFromSurface(ren,local_bitmap);
		if(!local_texture) mLog->LogSDLError("SDL_CreateTextureFromSuface");
	}

	SDL_FreeSurface(local_bitmap);
	return local_texture;
}

/*
renders text at x,y with colour
creates surface and texture then
renders texture
surface and texture are class members
the surface is freed at the end of this operation
if renderer is NULL default renderer is used.
does not offset by camera becuse this is done in RenderTexture() and would duplicate.
*/
void cRenderer::RenderText(const char* string, int x, int y, int font, SDL_Color colour, SDL_Renderer* ren, int space)
{
	if(!mFont) return;
	if(!ren) ren = mRenderer;

	mFontSurface = TTF_RenderText_Blended(mFont,string,colour);
	if(!mFontSurface)
	{
		mLog->LogSDLError("TTF_RenderText_Blended");
		return;
	}

	mFontTexture = SDL_CreateTextureFromSurface(ren,mFontSurface);
	if(!mFontTexture)
	{
		mLog->LogSDLError("SDL_CreateTextureFromSurface");
		return;
	}

	SDL_FreeSurface(mFontSurface);
	mFontSurface = NULL;
	RenderTexture(mFontTexture,x,y,ren,space);
	SDL_DestroyTexture(mFontTexture);
	return;
}

void cRenderer::RenderText(const int value, int x, int y, int font, SDL_Color colour, SDL_Renderer* ren, int space)
{
	std::stringstream textStream;
	textStream.str("");
	textStream << value;
	RenderText(textStream.str().c_str(),x,y,font,colour,ren,space);
}

/*
render a polygon from vertices.
if _2D is true (default) z axis of verts will be used
to identify WORLD_SPACE vs SCREEN_SPACE.
*/
void cRenderer::RenderVerts(int x, int y, const vector<JVector3>& verts, bool _2D, int _space)
{
	int num_verts = verts.size();
	vector<SDL_Point> points;
	SDL_Color l_prev_col = GetDrawColour();

	JVector2 camPos = mCamera->GetPos();
	if(_space == WORLD_SPACE)
	{
		x += camPos.x;
		y += camPos.y;
	}
	for(int i = 0; i < num_verts; i++)
	{
		points.push_back(SDL_Point());
		if(_2D && verts[i].z == WORLD_SPACE)
		{
			points.back().x = verts[i].x + camPos.x;
			points.back().y = verts[i].y + camPos.y;
		}
		else // LOCAL_SPACE
		{
			points.back().x = x + verts[i].x;
			points.back().y = y + verts[i].y;
		}
	}
	points.push_back(SDL_Point());
	points.back() = points[0];

	SetDrawColour(mColourBlack);
	SDL_RenderDrawLines(mRenderer,&points.front(),num_verts + 1);
	SetDrawColour(mColourDef);
}

/*
presents / flips renderer (back buffer) to display
then clears renderer to RenderDrawColor
renderer can be set to NULL then default renderer will be used
*/
void cRenderer::Present(SDL_Renderer* ren)
{
	if(!ren) ren = mRenderer;
	SDL_RenderPresent(ren);
	SetDrawColour(mColourDef,ren);
	SDL_RenderClear(ren);
}

/*
loads bitmap from .bmp
returns SDL_Texture*
remember to call UnloadBitmap() when finished
*/
SDL_Texture* cRenderer::LoadBitmap(const char* filename)
{
	SDL_Texture* result = LoadTextureFromBMP(filename,NULL);
	if(!result) 
	mLog->LogSDLError("cEntity::LoadBitmap() Error Loading Bitmap");
	return result;
}

/*
destroys bitmap and sets pointer to NULL
*/
void cRenderer::UnloadBitmap(SDL_Texture* _bitmap)
{
	if(_bitmap) SDL_DestroyTexture(_bitmap);
	_bitmap = NULL;
}
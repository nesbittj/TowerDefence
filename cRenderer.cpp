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

	//set default render colour
	SDL_SetRenderDrawColor(mRenderer,255,255,255,255);

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
draws a rectangle at position x,y
with width and height w,h
and colour col
RenderDrawColor is set back to what it was before operation
*/
void cRenderer::DrawRect(int x, int y, int w, int h, SDL_Color col, int _space)
{
	Uint8 prevColRect[4];
	SDL_GetRenderDrawColor(mRenderer,
		&prevColRect[0],&prevColRect[1],&prevColRect[2],&prevColRect[3]);

	SDL_SetRenderDrawColor(mRenderer,col.r,col.g,col.b,col.a);
	float2 camPos = mCamera->GetPos();
	if(_space == WORLD_SPACE/* && camPos.w > 0*/)
	{
		x += camPos.x;
		y += camPos.y;
	}
	SDL_Rect aRect = {x,y,w,h};
	if(SDL_RenderDrawRect(mRenderer,&aRect) != 0)
		mLog->LogSDLError("cRenderer::DrawRect()");

	if(SDL_SetRenderDrawColor(mRenderer,prevColRect[0],prevColRect[1],prevColRect[2],prevColRect[3]) != 0)
		mLog->LogSDLError("cRenderer::DrawRect()");
}

/*
x and y at centre of circle
*/
void cRenderer::DrawCircleFill(int _x, int _y, int _rad, SDL_Color _col, int _space)
{
	Uint8 prevColRect[4];
	SDL_GetRenderDrawColor(mRenderer,
		&prevColRect[0],&prevColRect[1],&prevColRect[2],&prevColRect[3]);
	SDL_SetRenderDrawColor(mRenderer,_col.r,_col.g,_col.b,_col.a);

	float2 camPos = mCamera->GetPos();
	if(_space == WORLD_SPACE/* && camPos.w > 0*/)
	{
		_x += camPos.x;
		_y += camPos.y;
	}

	for(int x = _x - _rad; x < (_x + _rad); x++)
	{
		for(int y = _y - _rad; y < (_y + _rad); y++)
		{
			if(((x*x) + (y*y)) < (_rad*_rad))
			{
				if(SDL_RenderDrawPoint(mRenderer,x,y) != 0)
					mLog->LogSDLError("cRenderer::DrawCircleFill() SDL_RenderDrawPoint()");
			}
		}
	}

	if(SDL_SetRenderDrawColor(mRenderer,prevColRect[0],prevColRect[1],prevColRect[2],prevColRect[3]) != 0)
		mLog->LogSDLError("cRenderer::DrawRect()");
}

/*
 * This is a 32-bit pixel function created with help from this
 * website: http://www.libsdl.org/intro.en/usingvideo.html
 *
 * You will need to make changes if you want it to work with
 * 8-, 16- or 24-bit surfaces.  Consult the above website for
 * more information.
 */
void cRenderer::set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	Uint8 *target_pixel = (Uint8 *)surface->pixels + y * surface->pitch + x * 4;
	*(Uint32 *)target_pixel = pixel;
}

/*
 * This is an implementation of the Midpoint Circle Algorithm 
 * found on Wikipedia at the following link:
 *
 *   http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
 *
 * The algorithm elegantly draws a circle quickly, using a
 * set_pixel function for clarity.
 */
void cRenderer::draw_circle(SDL_Surface *surf, int x0, int y0, int radius, Uint32 pixel)
{
	// if the first pixel in the screen is represented by (0,0) (which is in sdl)
	// remember that the beginning of the circle is not in the middle of the pixel
	// but to the left-top from it:
	
	int width = 640;
	int height = radius*3;
	Uint32 rmask, gmask, bmask, amask;
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
	SDL_Surface *surface = SDL_CreateRGBSurface(0,width,height,32,
								   rmask, gmask, bmask, amask);
	SDL_LockSurface(surface); 

	int x = radius;
  int y = 0;
  int radiusError = 1-x;

	while (x >= y)
	{
		set_pixel(surface,x + x0, y   + y0, pixel);
		set_pixel(surface,y + x0, x   + y0, pixel);
		set_pixel(surface,-x + x0, y  + y0, pixel);
		set_pixel(surface,-y + x0, x  + y0, pixel);
		set_pixel(surface,-x + x0, -y + y0, pixel);
		set_pixel(surface,-y + x0, -x + y0, pixel);
		set_pixel(surface,x + x0, -y  + y0, pixel);
		set_pixel(surface,y + x0, -x  + y0, pixel);
 
		y++;
		if (radiusError<0)
		{
		  radiusError += 2 * y + 1;
		}
		else
		{
		  x--;
		  radiusError += 2 * (y - x + 1);
		}
	}
	
	SDL_Texture* texture = SDL_CreateTextureFromSurface(mRenderer,surface);
	RenderTexture(texture,x0-width/2,y0-height/2,NULL);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
}

/*
renders a texture at position x,y
renderer can be set to NULL then default renderer will be used
*/
void cRenderer::RenderTexture(SDL_Texture* tex, int x, int y, SDL_Renderer* ren, int _space)
{
	if(!ren) ren = mRenderer;
	SDL_Rect local_rect;
	float2 camPos = mCamera->GetPos();
	if(_space == WORLD_SPACE/* && camPos.w > 0*/)
	{
		x += camPos.x;
		y += camPos.y;
	}
	local_rect.x = x;
	local_rect.y = y;
	if(SDL_QueryTexture(tex,NULL,NULL,&local_rect.w,&local_rect.h) != 0)
		mLog->LogSDLError("cRenderer::RenderTexture() QueryTexture() Error");
	if(SDL_RenderCopy(ren,tex,NULL,&local_rect) != 0)		
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
	float2 camPos = mCamera->GetPos();
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
render a polygon from vertices
*/
void cRenderer::RenderVerts(int x, int y, const vector<JVector3>& verts, int _space)
{
	int num_verts = verts.size();
	vector<SDL_Point> points;
	float2 camPos = mCamera->GetPos();
	if(_space == WORLD_SPACE/* && camPos.w > 0*/)
	{
		x += camPos.x;
		y += camPos.y;
	}
	for(int i = 0; i < num_verts; i++)
	{
		points.push_back(SDL_Point());
		if(verts[i].z == WORLD_SPACE)
		{
			points.back().x = verts[i].x;
			points.back().y = verts[i].y;
		}
		else // LOCAL_SPACE
		{
			points.back().x = x + verts[i].x;
			points.back().y = y + verts[i].y;
		}
	}
	points.push_back(SDL_Point());
	points.back() = points[0];

	SDL_SetRenderDrawColor(mRenderer,0,0,0,255);
	SDL_RenderDrawLines(mRenderer,&points.front(),num_verts + 1);
}

/*
presents / flips renderer (back buffer) to display
then clears renderer to RenderDrawColor
renderer can be set to NULL then default renderer will be used
*/
void cRenderer::Present(SDL_Renderer* ren) const
{
	if(!ren) ren = mRenderer;
	SDL_RenderPresent(ren);
	SDL_RenderClear(ren);
}
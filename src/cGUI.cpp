#include "cGUI.h"

namespace cGUInamepsace
{
	
enum { GUI_STATE_NO_MOUSE, GUI_STATE_MOUSE_OVER, GUI_STATE_MOUSE_DOWN };

SDL_Color mThemeBase;
SDL_Color mThemeColour;
SDL_Color mThemeColourMouseOver;
SDL_Color mThemeColourMouseDown;
SDL_Color mFontColour;

cRenderer* mRen;
element* mRoot;

void cGUI::Init(SDL_Color _theme_colour)
{
	SetThemeColour(_theme_colour);
	mRen = cRenderer::Instance();
	mFontColour = mRen->mColourBlack;
	mRoot = new element(0,0,0,0,"",NULL);
}

void cGUI::SetThemeColour(SDL_Color _theme_colour)
{
	mThemeColour = _theme_colour;
	mThemeBase.r = mThemeBase.g = mThemeBase.b = mThemeBase.a = 100;
	mThemeColourMouseOver = mThemeColourMouseDown = mThemeColour;
	mThemeColourMouseOver.a = mThemeColourMouseDown.a = 255;
	mThemeColourMouseOver.r = mThemeColourMouseOver.g = mThemeColourMouseOver.b += 20;
	mThemeColourMouseDown.r = mThemeColourMouseDown.g = mThemeColourMouseDown.b -= 20;
}

panel* cGUI::AddElementPanel(Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h, char* _text, element* _parent)
{
	elements.push_back(new panel(_x,_y,_w,_h,_text,_parent));
	return (panel*)elements.back();
}

void cGUI::Render()
{
	for(unsigned int i = 0; i < elements.size(); i++)
		elements[i]->Render();
}

void cGUI::CleanUp()
{
	for(Uint32 i = 0; i < elements.size(); i++)
	{
		if(elements[i]) delete elements[i];
		elements[i] = NULL;
	}
	if(mRoot) delete mRoot;
	mRoot = NULL;
	mRen = NULL;
}

element::element(Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h, char* _text, element* _parent)
{
	x = _x; y = _y;
	w = _w; h = _h;
	text = _text;
	parent = _parent;
	if(!parent) parent = mRoot;
	state = GUI_STATE_NO_MOUSE;
}

panel::panel(Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h, char* _text, element* _parent)
	: element(_x,_y,_w,_h,_text,_parent)
{
}

panel::~panel()
{	
	for(Uint32 i = 0; i < elements.size(); i++)
	{
		if(elements[i]) delete elements[i];
		elements[i] = NULL;
	}
}

void panel::Render()
{
	mRen->DrawFilledRect((float)parent->x+x,(float)parent->y+y,x+w,y+h,
		mThemeBase,0,SCREEN_SPACE);
	mRen->RenderText(text,parent->x+x,parent->y+y,0,mFontColour,0,SCREEN_SPACE);
	for(unsigned int i = 0; i < elements.size(); i++)
		elements[i]->Render();
}

void panel::AddElementTextfield(Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h,
								char* _text, element* _parent)
{
	elements.push_back(new textfield(_x,_y,_w,_h,_text,_parent));
}

void panel::AddElementButton(Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h,
								char* _text, element* _parent)
{
	elements.push_back(new button(_x,_y,_w,_h,_text,_parent));
}

textfield::textfield(Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h, char* _text, element* _parent)
	: element(_x,_y,_w,_h,_text,_parent)
{
}

void textfield::Render()
{
	mRen->DrawFilledRect((float)parent->x+x,(float)parent->y+y,x+w,y+h,
		mThemeBase,0,SCREEN_SPACE);
	mRen->RenderText(text,(float)parent->x+x,(float)parent->y+y,0,mFontColour,0,SCREEN_SPACE);
}

button::button(Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h, char* _text, element* _parent)
	: element(_x,_y,_w,_h,_text,_parent)
{
}

void button::Render()
{	
	switch(state)
	{
	case GUI_STATE_NO_MOUSE:
		{
			mRen->DrawFilledRect(
				(float)parent->x+x,(float)parent->y+y,x+w,y+h,
				mThemeColour,0,SCREEN_SPACE);
		 }break;
	case GUI_STATE_MOUSE_OVER:
		{
			mRen->DrawFilledRect(
				(float)parent->x+x,(float)parent->y+y,x+w,y+h,
				mThemeColourMouseOver,0,SCREEN_SPACE);
		}break;
	case GUI_STATE_MOUSE_DOWN:
		{
			mRen->DrawFilledRect(
				(float)parent->x+x,(float)parent->y+y,x+w,y+h,
				mThemeColourMouseDown,0,SCREEN_SPACE);
		}break;
	}
	mRen->RenderText(text,(float)parent->x+x,(float)parent->y+y,0,mFontColour,0,SCREEN_SPACE);
}

}
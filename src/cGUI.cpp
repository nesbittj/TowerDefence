#include "cGUI.h"

#pragma once

namespace cGUInamepsace
{



///////////////////////////////////////////////////////////////////////
//cGUI
///////////////////////////////////////////////////////////////////////
cGUI* cGUI::mInstance = NULL;
cGUI* cGUI::Instance()
{
	if(!mInstance) mInstance = new cGUI;
	return mInstance;
}

/*
initialise GUI object.
remember to call CleanUp when finished
*/
void cGUI::Init(SDL_Color _theme_colour) //TODO:logging!!
{
	SetThemeColour(_theme_colour);
	gRen = cRenderer::Instance();
	gInput = cInput::Instance();
	gFontColour = gRen->mColourBlack;
}

void cGUI::CleanUp()
{
	for(Uint32 i = 0; i < elements.size(); i++)
	{
		if(elements[i]) delete elements[i];
		elements[i] = NULL;
	}
	gRen = NULL;
	gInput = NULL;
}

/*
sets GUI colour global variables to predefined colours
with accent colour set by parameter _theme_colour:SDL_Color.
*/
void cGUI::SetThemeColour(SDL_Color _theme_colour)
{
	//TODO: tidy, could load from file
	gThemeColour = _theme_colour;
	gThemeBase.r = gThemeBase.g = gThemeBase.b = 100;
	gThemeBase.a = 150;
	gThemeColourMouseOver = gThemeColourMouseDown = gThemeColour;
	gThemeColourMouseOver.a = gThemeColourMouseDown.a = 255;
	gThemeColourMouseOver.r -= 20;
	gThemeColourMouseOver.g -= 20;
}

/*
add a new panel to GUI.
panel will have no parent, parent position[0,0].
returns pointer to panel created.
uses panel class.
*/
panel* cGUI::AddElementPanel(Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h)
{
	elements.push_back(new panel(_x,_y,_w,_h,NULL));
	return (panel*)elements.back();
}

/*
add a new textfield to GUI.
textfield will have no parent, parent position[0,0].
returns pointer to textfield created.
uses textfield class.
*/
textfield<char*>* cGUI::AddElementTextfield(
	Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h, char* _text)
{
	elements.push_back(new textfield<char*>(_x,_y,_w,_h,_text,NULL));
	return (textfield<char*>*)elements.back();
}

/*
add a new textfield to GUI.
textfield will have no parent, parent position[0,0].
returns pointer to textfield created.
uses textfield class.
*/
textfield<Sint32*>* cGUI::AddElementTextfield(
	Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h, Sint32* _value)
{
	elements.push_back(new textfield<Sint32*>(_x,_y,_w,_h,_value,NULL));
	return (textfield<Sint32*>*)elements.back();
}

/*
update all GUI elements.
use element::SetFocus() to set individual element focus
*/
void cGUI::Update()
{
	bool l_close = false;
	for(unsigned int i = 0; i < elements.size(); i++)
	{
		elements[i]->Update();
	}
}

/*
render all GUI elements.
use element::SetFocus() to set individual element focus
*/
void cGUI::Render()
{
	for(unsigned int i = 0; i < elements.size(); i++)
		elements[i]->Render();
}


///////////////////////////////////////////////////////////////////////
//element
///////////////////////////////////////////////////////////////////////

/*
constructor of base GUI element,
should not be called explicitly.
*/
element::element(Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h, element* _parent)
{
	mGUI = cGUI::Instance();
	parent = _parent;
	if(parent) focus = parent->GetFocus();
	else focus = GUI_FOCUS_NONE;
	SetPos(_x,_y,_w,_h);
}

/*
focus types: GUI_FOCUS_NONE, GUI_FOCUS_NO_MOUSE, GUI_FOCUS_MOUSE_OVER, GUI_FOCUS_MOUSE_DOWN.
GUI_FOCUS_NONE : element will not be updated or rendered.
returns true on success, returns false on error
*/
bool element::SetFocus(Uint8 _focus)
{
	if(_focus < GUI_FOCUS_NONE) return false;
	if(_focus > GUI_FOCUS_MOUSE_DOWN) return false;
	focus = _focus;
	return true;
}

/*
sets new position of element.
is element has a parent, position is relative to parent.
*/
void element::SetPos(Sint32 _x, Sint32 _y)
{
	if(parent)
	{
		x1 = parent->x1 + _x;
		y1 = parent->y1 + _y;
		x2 = x1 + w;
		y2 = y1 + h;
	}
	else
	{		
		x1 = _x;
		y1 = _y;
		x2 = x1 + w;
		y2 = y1 + h;
	}
}

/*
sets new position and width of element.
is element has a parent, position is relative to parent.
uses SetPos(Sint32,Sint32).
*/
void element::SetPos(Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h)
{
	w = _w;
	h = _h;
	SetPos(_x,_y);
}

///////////////////////////////////////////////////////////////////////
//panel
///////////////////////////////////////////////////////////////////////


/*
constructor of panel element,
should not be called explicitly,
use cGUI::AddElementPanel or cPanel::AddPanel.
*/
panel::panel(Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h, element* _parent)
	: element(_x,_y,_w,_h,_parent)
{
	mCloseWithEsc = false;
}

panel::~panel()
{	
	for(Uint32 i = 0; i < elements.size(); i++)
	{
		if(elements[i]) delete elements[i];
		elements[i] = NULL;
	}
}

/*
updates panel and all (if any) child elements
should not be called explicitly, use cGUI::Update.
*/
void panel::Update()
{
	if(focus <= GUI_FOCUS_NONE) return;

	if(mCloseWithEsc && mGUI->gInput->GetKeyDownRelease(SDLK_ESCAPE))
		SetFocus(GUI_FOCUS_NONE);

	//TODO: account for parent pos
	if(cMaths::InRect(mGUI->gInput->GetMouseX(),mGUI->gInput->GetMouseY(),x1,y1,x2,y2))
	{
		for(unsigned int i = 0; i < elements.size(); i++)
			elements[i]->Update();
	}
}

/*
renders panel and all (if any) child elements
should not be called explicitly, use cGUI::Render.
*/
void panel::Render()
{
	if(focus > GUI_FOCUS_NONE)
	{
		mGUI->gRen->DrawFilledRect(x1,y1,w,h,
			mGUI->gThemeBase,0,SCREEN_SPACE);
		for(unsigned int i = 0; i < elements.size(); i++)
			elements[i]->Render();
	}
}

/*
focus types: GUI_FOCUS_NONE, GUI_FOCUS_NO_MOUSE, GUI_FOCUS_MOUSE_OVER, GUI_FOCUS_MOUSE_DOWN.
GUI_FOCUS_NONE : element will not be updated or rendered.
all (if any) child elements are set to the same focus type.
returns true on success, returns false on error
*/
bool panel::SetFocus(Uint8 _focus)
{
	if(element::SetFocus(_focus))
	{
		for(unsigned int i = 0; i < elements.size(); i++)
			if(!elements[i]->SetFocus(focus)) return false;
		return true;
	}
	return false;
}

/*
sets new position of panel.
is panel has a parent, position is relative to parent.
if panel has child elements, each child is updated to position reletive to this panel. 
*/
void panel::SetPos(Sint32 _x, Sint32 _y)
{
	Sint32 _old_x1 = x1;
	Sint32 _old_y1 = y1;
	element::SetPos(_x,_y);
	for(unsigned int i = 0; i < elements.size(); i++)
		elements[i]->SetPos(elements[i]->x1 - _old_x1, elements[i]->y1 - _old_y1);
}

/*
add a new panel to this panel.
panel will have this panel as parent.
position is reletive to parent.
*/
void panel::AddElementPanel(Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h)
{
	elements.push_back(new panel(_x,_y,_w,_h,this));
}

/*
add a new textfield to this panel.
textfield will have this panel as parent.
position is reletive to parent.
uses textfield class
*/
void panel::AddElementTextfield(Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h, char* _text)
{
	elements.push_back(new textfield<char*>(_x,_y,_w,_h,_text,this));
}

/*
add a new textfield to this panel.
textfield will have this panel as parent.
position is reletive to parent.
uses textfield class
*/
void panel::AddElementTextfield(Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h, Sint32* _value)
{
	elements.push_back(new textfield<Sint32*>(_x,_y,_w,_h,_value,this));
}

///////////////////////////////////////////////////////////////////////
//textfield
///////////////////////////////////////////////////////////////////////

/*
constructor of textfield element,
should not be called explicitly.
use cGUI::AddElementTextfield or panel::AddElementTextfield
*/
template <class T> textfield<T>::textfield
(Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h, T _value, element* _parent) : element(_x,_y,_w,_h,_parent)
{
	value = _value;
}

/*
render textfield with char* value.
should not be called explicitly, use cGUI::Render()
*/
template <> void textfield<char*>::Render()
{
	if(focus > GUI_FOCUS_NONE)
	{
		mGUI->gRen->DrawFilledRect(x1,y1,w,h,
			mGUI->gThemeBase,0,SCREEN_SPACE);
		mGUI->gRen->RenderText((char*)value,(float)x1,(float)y1,0,mGUI->gFontColour,0,SCREEN_SPACE);
	}
}

/*
render textfield with Sint32 value.
should not be called explicitly, use cGUI::Render()
*/
template <> void textfield<Sint32*>::Render()
{
	if(focus > GUI_FOCUS_NONE)
	{
		mGUI->gRen->DrawFilledRect(x1,y1,w,h,
			mGUI->gThemeBase,0,SCREEN_SPACE);
		mGUI->gRen->RenderText((float)*value,(float)x1,(float)y1,0,mGUI->gFontColour,0,SCREEN_SPACE);
	}
}

} //cGUInamespace
#include "cGUI.h"

namespace cGUInamepsace
{

///////////////////////////////////////////////////////////////////////
//Global Variables
///////////////////////////////////////////////////////////////////////
static SDL_Color gThemeBase;	//background colour to panels and textboxes
static SDL_Color gThemeColour;	//colour of GUI buttons and accents
static SDL_Color gThemeColourMouseOver;	//if element has interaction,
static SDL_Color gThemeColourMouseDown;	//colour of element when mouse is over it.
static SDL_Color gFontColour;	//colour of font used for all GUI elements

static cRenderer* gRen;	//pointer to cRenderer singleton
static cInput* gInput;	//pointer to cInput singleton

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
void cGUI::Init(SDL_Color _theme_colour)
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
	for(unsigned int i = 0; i < elements.size(); i++)
		elements[i]->Update();
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
	parent = _parent;
	if(parent)
	{
		x1 = parent->x1 + _x;
		y1 = parent->y1 + _y;
		x2 = x1 + _w;
		y2 = y1 + _h;
		focus = parent->GetFocus();
	}
	else
	{		
		x1 = _x;
		y1 = _y;
		x2 = x1 + _w;
		y2 = y1 + _h;
		focus = GUI_FOCUS_NONE;
	}
	w = _w;
	h = _h;
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

	//TODO: account for parent pos
	if(cMaths::InRect(gInput->GetMouseX(),gInput->GetMouseY(),x1,y1,x2,y2))
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
		gRen->DrawFilledRect(x1,y1,x2,y2,
			gThemeBase,0,SCREEN_SPACE);
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

/*
add a new button to this panel.
6th parameter is funtion pointer, see button::Update()
button will have this panel as parent.
position is reletive to parent.
uses button class
*/
void panel::AddElementButton(Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h, char* _text, void (*_function)(void))
{
	elements.push_back(new button(_x,_y,_w,_h,_text,_function,this));
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
		gRen->DrawFilledRect(x1,y1,w,h,
			gThemeBase,0,SCREEN_SPACE);
		gRen->RenderText((char*)value,(float)x1,(float)y1,0,gFontColour,0,SCREEN_SPACE);
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
		gRen->DrawFilledRect(x1,y1,w,h,
			gThemeBase,0,SCREEN_SPACE);
		gRen->RenderText((float)*value,(float)x1,(float)y1,0,gFontColour,0,SCREEN_SPACE);
	}
}


///////////////////////////////////////////////////////////////////////
//button
///////////////////////////////////////////////////////////////////////


/*
constructor of button element,
should not be called explicitly.
use panel::AddElementButton
*/
button::button(Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h, char* _text, void (*_function)(void), element* _parent)
	: element(_x,_y,_w,_h,_parent)
{
	operation = _function;
	text = _text;
}

/*
updates button element.
funtion pointer member is run if mouse is
over button and mouse button is released.
should not be called explicitly, use cGUI::Update().
uses SetFocus().
*/
void button::Update()
{
	if(focus <= GUI_FOCUS_NONE) return;

	if(cMaths::InRect(gInput->GetMouseX(),gInput->GetMouseY(),x1,y1,x2,y2))
	{
		if(gInput->GetMouseButtonDown(LEFT_MOUSE_BUTTON))
		{
			SetFocus(GUI_FOCUS_MOUSE_DOWN);
		}
		else if(gInput->GetMouseButtonReleased(LEFT_MOUSE_BUTTON))
		{
			SetFocus(GUI_FOCUS_MOUSE_OVER);
			operation();
		}
		else
			SetFocus(GUI_FOCUS_MOUSE_OVER);
	}
	else
		SetFocus(GUI_FOCUS_NO_MOUSE);
}

/*
render button element.
should not be called explicitly, use cGUI::Render().
render type depends on current focus type, see element::SetFocus()
*/
void button::Render()
{	
	switch(focus)
	{
	case GUI_FOCUS_NO_MOUSE:
		{
			gRen->DrawFilledRect(x1,y1,w,h,gThemeColour,0,SCREEN_SPACE);
		 }break;
	case GUI_FOCUS_MOUSE_OVER:
		{
			gRen->DrawFilledRect(x1,y1,w,h,gThemeColourMouseOver,0,SCREEN_SPACE);
		}break;
	case GUI_FOCUS_MOUSE_DOWN:
		{
			gRen->DrawFilledRect(x1+2,y1+2,w,h,gThemeColourMouseDown,0,SCREEN_SPACE);
		}break;
	default:
		return;
	}
	gRen->RenderText(text,(float)x1,(float)y1,0,gFontColour,0,SCREEN_SPACE);
}

} //cGUInamespace
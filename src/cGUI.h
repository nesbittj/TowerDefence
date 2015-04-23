/*
jonathan nesbitt
03/03/15

GUI class
controls GUI updates and rendering
singleton class
uses SDL2 library
*/

#pragma once

#include <SDL.h>
#include "cMaths.h"
#include "cRenderer.h"
#include "cInput.h"

namespace cGUInamepsace
{

class element;
class panel;
template <class cInstance> class button;
template <class T> class textfield;

enum { GUI_FOCUS_NONE, GUI_FOCUS_NO_MOUSE, GUI_FOCUS_MOUSE_OVER, GUI_FOCUS_MOUSE_DOWN };

class cGUI
{
private:
	static cGUI* mInstance;
	cGUI() {} ;

	vector<element*> elements;
public:
	static cGUI* Instance();

	void SetThemeColour(SDL_Color _col);
	void Init(SDL_Color _theme_colour);
	void CleanUp();
	panel* AddElementPanel(Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h);
	textfield<char*>* AddElementTextfield(
		Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h, char* _text);
	textfield<Sint32*>* AddElementTextfield(
		Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h, Sint32* _value);
	void virtual Update();
	void Render();


	SDL_Color gThemeBase;	//background colour to panels and textboxes
	SDL_Color gThemeColour;	//colour of GUI buttons and accents
	SDL_Color gThemeColourMouseOver;	//if element has interaction,
	SDL_Color gThemeColourMouseDown;	//colour of element when mouse is over it.
	SDL_Color gFontColour;	//colour of font used for all GUI elements

	cRenderer* gRen;	//pointer to cRenderer singleton
	cInput* gInput;	//pointer to cInput singleton
};


class element
{
protected:
	Uint8 focus;
	element* parent;
	cGUI* mGUI;
public:
	Sint32 x1, y1;
	Sint32 x2, y2;
	Sint32 w, h;
	element(Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h, element* _parent);
	bool virtual SetFocus(Uint8 _focus);
	void virtual SetPos(Sint32 _x, Sint32 _y);
	void virtual SetPos(Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h);
	int virtual GetFocus() { return focus; }
	void virtual Update() { } ;
	void virtual Render() { } ;
};

class panel : public element
{
private:
	vector<element*> elements;
public:
	panel(Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h, element* _parent);
	~panel();
	bool virtual SetFocus(Uint8 _focus);
	void virtual SetPos(Sint32 _x, Sint32 _y);
	void virtual Update();
	void virtual Render();
	void AddElementPanel(Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h);
	void AddElementTextfield(Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h, char* _text);
	void AddElementTextfield(Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h, Sint32* _value);
	/* use button::AddButtonToPanel to add a new button to an existing panel */
	void AddElementButton(element *_pButton) { elements.push_back(_pButton); }
};

template <class T>
class textfield : public element
{
private:
	T value;
public:
	textfield(Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h, T _value, element* _parent);
	void virtual Render();
};

template <class cInstance>
class button : public element
{
private:
	typedef bool (cInstance::*tFunction)(int);

	char* text;
	cInstance* mInst;
	tFunction mFunction;
	int mParam;
	bool mReturnValue;
	element* mCloseOnClick;

public:
	button(Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h, element* _parent,
		char* _text, cInstance* _pInst, tFunction _pFunction, Sint32 _pParam) : element(_x,_y,_w,_h,_parent)
	{
		mInst = _pInst;
		mFunction = _pFunction;
		mParam = _pParam;
		text = _text;
		mReturnValue = false;
	}	

	~button()
	{
		text = "";
		mInst = NULL;
		mFunction = NULL;
		mParam = NULL;
	}

	void virtual Execute()
	{	
		if(mInst && mFunction) mReturnValue = (mInst->*mFunction)(mParam);
		else parent->SetFocus(GUI_FOCUS_NONE);
		//else printf("\ninstance, function or parameter not defined\n");
	}

	void virtual Update()
	{
		if(focus <= GUI_FOCUS_NONE) return;

		if(cMaths::InRect(mGUI->gInput->GetMouseX(),mGUI->gInput->GetMouseY(),x1,y1,x2,y2))
		{
			if(mGUI->gInput->GetMouseButtonDown(LEFT_MOUSE_BUTTON))
			{
				SetFocus(GUI_FOCUS_MOUSE_DOWN);
			}
			else if(mGUI->gInput->GetMouseButtonReleasedRelease(LEFT_MOUSE_BUTTON))
			{
				SetFocus(GUI_FOCUS_MOUSE_OVER);
				Execute();
			}
			else
				SetFocus(GUI_FOCUS_MOUSE_OVER);
		}
		else
			SetFocus(GUI_FOCUS_NO_MOUSE);
	}

	void virtual Render()
	{	
		switch(focus)
		{
		case GUI_FOCUS_NO_MOUSE:
			{
				mGUI->gRen->DrawFilledRect(x1,y1,w,h,mGUI->gThemeColour,0,SCREEN_SPACE);
			} break;
		case GUI_FOCUS_MOUSE_OVER:
			{
				mGUI->gRen->DrawFilledRect(x1,y1,w,h,mGUI->gThemeColourMouseOver,0,SCREEN_SPACE);
			} break;
		case GUI_FOCUS_MOUSE_DOWN:
			{
				mGUI->gRen->DrawFilledRect(x1+2,y1+2,w,h,mGUI->gThemeColourMouseDown,0,SCREEN_SPACE);
			} break;
		default:
			return;
		}
		mGUI->gRen->RenderText(text,(float)x1,(float)y1,0,mGUI->gFontColour,0,SCREEN_SPACE);
	}

	bool GetReturnValue() { return mReturnValue; }

	template <class cInstance>
	static void AddButtonToPanel(panel* _pPanel, Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h,
		char* _text, cInstance* _pInst, bool (cInstance::*_pFunction)(int), Sint32 _pParam)
	{
		_pPanel->AddElementButton(new button<cInstance>
			(_x,_y,_w,_h,_pPanel,_text,_pInst,static_cast<tFunction>(_pFunction),_pParam));
	}
};

} // cGUInamespace
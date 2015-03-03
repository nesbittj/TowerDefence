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
};


class element
{
protected:
	Uint8 focus;
	element* parent;
public:
	Sint32 x1, y1;
	Sint32 x2, y2;
	Sint32 w, h;
	element(Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h, element* _parent);
	bool virtual SetFocus(Uint8 _focus);
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
	void virtual Update();
	void virtual Render();
	void AddElementPanel(Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h);
	void AddElementTextfield(Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h, char* _text);
	void AddElementTextfield(Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h, Sint32* _value);
	void AddElementButton(
		Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h,	char* _text, void (*_function)(void));
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
	
class button : public element
{
private:
	char* text;
	void (*operation)(void);
public:
	button(
		Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h,
		char* _text, void (*_function)(void), element* _parent);
	void virtual Update();
	void virtual Render();
};

} // cGUInamespace
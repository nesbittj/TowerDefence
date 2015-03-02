#pragma once

#include <SDL.h>
#include "cRenderer.h"

namespace cGUInamepsace
{

class element;
class panel;

class cGUI
{
private:
	vector<element*> elements;
public:
	void SetThemeColour(SDL_Color _col);
	void Init(SDL_Color _theme_colour);
	void CleanUp();
	panel* AddElementPanel(Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h, char* _text, element* _parent);
	void Render();
};

class element
{
protected:
	Uint8 state;
	element* parent;
	char* text;
public:
	Sint32 x, y;
	Sint32 w, h;
	element(Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h, char* _text, element* _parent);
	void virtual Render() {};
};

class panel : public element
{
private:
	vector<element*> elements;
public:
	panel(Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h, char* _text, element* _parent);
	~panel();
	void virtual Render();
	void AddElementPanel(Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h, char* _text, element* _parent);
	void AddElementTextfield(Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h, char* _text, element* _parent);
	void AddElementButton(Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h, char* _text, element* _parent);
};
	
class textfield : public element
{
private:
public:
	textfield(Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h, char* _text, element* _parent);
	void virtual Render();
};
	
class button : public element
{
private:
	//funtion pointer
public:
	button(Sint32 _x, Sint32 _y, Sint32 _w, Sint32 _h, char* _text, element* _parent);
	void virtual Render();

};

}
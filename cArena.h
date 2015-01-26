#pragma once

#include <SDL.h>
#include "cCamera.h"
#include "cRenderer.h"
#include "cMaths.h"
#include "cLogger.h"
#include "cQueue.h"
#include "tinyxml2.h"
using namespace tinyxml2;

class cArena
{
private:
	static const unsigned int NUM_TILES = 2;
	static const unsigned int GRID_SIZE = 32;

	float x,y;
	string mArenaTiles[GRID_SIZE]; //TODO: grid size should be set in this class
	SDL_Texture* mTiles[NUM_TILES];
	cRenderer* mRen;
	cLogger* mLog;

	bool UnloadBitmaps();

	//Breadth First Search Variables
	vector<JVector2> mOpenList;
	vector<JVector2> mClosedList;
	JVector2 mNeighbours[4];

public:
	cArena(float _x, float _y);
	~cArena();

	bool Init();
	bool CleanUp();

	bool LoadArenaData(const char* _filename);
	const char* GetTyleType(JVector2 _pos);
	void Draw();
	vector<JVector2> BreadthFirstOld(const JVector2 _start, const JVector2 _target);
	cQueue<JVector2> BreadthFirst(const JVector2 _start, const JVector2 _target);
	void GraphNeighbours(JVector2 _current);
	bool CheckBounds(JVector2 _pos);
	
	static int Contains(vector<JVector2>* _vect, JVector2 _val);
	inline JVector2 GetPos() const { return JVector2(x,y); }
	void SetPos(float _x, float _y);
};
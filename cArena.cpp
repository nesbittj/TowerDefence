#include "cArena.h"

cArena::cArena(float _x, float _y)
{
	x = _x; y = _y;
}

cArena::~cArena()
{
}

bool cArena::Init()
{
	mRen = cRenderer::Instance();
	mLog = cLogger::Instance();
	if(!LoadArenaData("")) return false;
	return true;
}

bool cArena::CleanUp()
{
	UnloadBitmaps();
	mLog = NULL;
	mRen = NULL;
	return true;
}

bool cArena::UnloadBitmaps()
{
	for(int i = 0; i < NUM_TILES; i++)
	{
		if(mTiles[i]) SDL_DestroyTexture(mTiles[i]);
		mTiles[i] = NULL;
	}
	return true;
}

bool cArena::LoadArenaData(const char* _filename)
{
	XMLDocument doc;
	if(!doc.LoadFile("assets/arena/arena_data.xml"))
	{
		XMLElement* l_tile_line = doc.FirstChildElement("layout")->FirstChild()->ToElement();
		int i = 0;
		for(l_tile_line; l_tile_line; l_tile_line=l_tile_line->NextSiblingElement())
		{
			mArenaTiles[i] = l_tile_line->Attribute("tiles");
			i++;
		}
		XMLElement* l_tile_bmp = doc.FirstChildElement("tiles")->FirstChild()->ToElement();
		i = 0;
		for(l_tile_bmp; l_tile_bmp; l_tile_bmp=l_tile_bmp->NextSiblingElement())
		{
			mTiles[i] = mRen->LoadBitmap(l_tile_bmp->Attribute("bitmap"));
			i++;
		}
	}
	else
	{
		mLog->LogError("cArena: XML failed to load");
		return false;
	}
	return true;
}

const char* cArena::GetTyleType(JVector2 _pos)
{
	if(_pos.x < 0 || _pos.x > 20) return "BAD_VALUE";
	if(_pos.y < 0 || _pos.y > 30) return "BAD_VALUE";
	return &mArenaTiles[(int)_pos.y][(int)_pos.x];
}

void cArena::Draw()
{
	//screen space is used because x,y is set to camera pos (move has already happened)
	//TODO: properly enumerate tiles number and render values
	for(int i = 0; i < 15; i++)
	{
		for(int j = 0; j < 20; j++)
		{
			if(mArenaTiles[i][j] == '.')
				mRen->RenderTexture(mTiles[0],GRID_SIZE*j+x,GRID_SIZE*i+y,0,32,32,1);
			if(mArenaTiles[i][j] == 'P')
				mRen->RenderTexture(mTiles[1],GRID_SIZE*j+x,GRID_SIZE*i+y,0,32,32,1);
		}
	}
}

vector<JVector2> cArena::BreadthFirstOld(const JVector2 _start, const JVector2 _target)
{
	JVector2 l_current(0,0);
	mOpenList.push_back(_start);
	//mClosedList.push_back(l_current);

	while(!mOpenList.empty())
	{
		l_current = mOpenList.back();
		mOpenList.pop_back();

		if(l_current.x == _target.x && l_current.y == _target.y) 
			break;
		
		if(Contains(&mClosedList,l_current) < 0)
			mClosedList.push_back(l_current);

		GraphNeighbours(l_current);
		for(int i = 0; i < 4; i++)
		{
			if(Contains(&mClosedList,mNeighbours[i]) < 0 && Contains(&mOpenList,mNeighbours[i]) < 0)
			{
				if(CheckBounds(mNeighbours[i])) //TODO: also check against obsticals
				{
					mOpenList.push_back(mNeighbours[i]);
				}
			}
		}
	}

	vector<JVector2> l_path;
	for(int i = 0; i < mClosedList.size(); i++)
	{
		const char l_tile = *GetTyleType(mClosedList[i]);
		if(l_tile == 'P' || l_tile == 'S' || l_tile == 'C' || l_tile == 'E')
		{
			JVector2 l_path_pos(mClosedList[i].x*GRID_SIZE,mClosedList[i].y*GRID_SIZE);
			l_path.push_back(l_path_pos);
		}
	}
	return l_path;
}

vector<JVector2> cArena::BreadthFirst(const JVector2 _start, const JVector2 _target)
{
	JVector2 l_current(0,0);
	cQueue<JVector2> l_open;
	l_open.Enqueue(_start);
	cQueue<JVector2> l_closed;

	while(!mOpenList.empty())
	{
		l_current = l_open.Dequeue();

		if(l_current.x == _target.x && l_current.y == _target.y)
			break;

		GraphNeighbours(l_current);
		for(int i = 0; i < 4; i++)
		{
			if(Contains(&mClosedList,mNeighbours[i]) < 0)
			{
				if(CheckBounds(mNeighbours[i])) //TODO: also check against obsticals
				{
					mOpenList.push_back(mNeighbours[i]);
					mClosedList.push_back(l_current);
				}
			}
		}
	}

	vector<JVector2> l_path;
	for(int i = 0; i < mClosedList.size(); i++)
	{
		const char l_tile = *GetTyleType(mClosedList[i]);
		if(l_tile == 'P' || l_tile == 'S' || l_tile == 'C' || l_tile == 'E')
		{
			JVector2 l_path_pos(mClosedList[i].x*GRID_SIZE,mClosedList[i].y*GRID_SIZE);
			l_path.push_back(l_path_pos);
		}
	}
	return l_path;
}

/*
searches vector _vect for value _val.
return index of found element,
returns -1 if value not found
*/
int cArena::Contains(vector<JVector2>* _vect, JVector2 _val)
{
	for(int i = 0; i < (*_vect).size(); i++)
	{
		if((*_vect)[i].x == _val.x && (*_vect)[i].y == _val.y)
			return i;
	}
	return -1;
}

void cArena::GraphNeighbours(JVector2 _current)
{	
	//right
	mNeighbours[0].x = _current.x + 1;
	mNeighbours[0].y = _current.y;
	//bottom
	mNeighbours[1].x = _current.x;
	mNeighbours[1].y = _current.y + 1;
	//left
	mNeighbours[2].x = _current.x - 1;
	mNeighbours[2].y = _current.y;
	//top
	mNeighbours[3].x = _current.x;
	mNeighbours[3].y = _current.y - 1;
}

bool cArena::CheckBounds(JVector2 _pos)
{
	if(_pos.x < 0.f) return false;
	if(_pos.y < 0.f) return false;
	if(_pos.x > 10) return false;
	if(_pos.y > 10) return false;
	return true;
}

void cArena::SetPos(float _x, float _y)
{
	//TODO: check bounds
	x = _x;
	y = _y;
}

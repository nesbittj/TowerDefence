#include "cArena.h"

cArena::cArena()
{
	mEnemyTargetPos = mEnemyExitPos = mEnemyStartPos = JVector2();
}

cArena::~cArena()
{
}

bool cArena::Init()
{
	mRen = cRenderer::Instance();
	mLog = cLogger::Instance();
	if(!LoadArenaData("")) return false;

	mArenaWidthProdGrid = mArenaWidth * GRID_SIZE;
	mArenaHeightProdGrid = mArenaHeight * GRID_SIZE;
	
	mCore = new cCore(mEnemyTargetPos.x,mEnemyTargetPos.y);
	mCore->Init(0);

	return true;
}

bool cArena::CleanUp()
{
	mCore->CleanUp();
	delete mCore; mCore = NULL;
	
	for(int i = 0; i < NUM_TILES; i++)
	{
		if(mTiles[i]) SDL_DestroyTexture(mTiles[i]);
		mTiles[i] = NULL;
	}

	mLog = NULL;
	mRen = NULL;

	return true;
}

bool cArena::LoadArenaData(const char* _filename)
{
	//TODO: load grid size from file data
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
		mArenaHeight = i;
		mArenaWidth = mArenaTiles[0].size();
		//TODO: only square arenas will work with this implomentation

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
		mArenaWidth = 10;
		mArenaHeight = 10;
		mLog->LogError("cArena: XML failed to load");
		return false;
	}

	//TODO: properly set bounds
	for(int i = 0; i < mArenaHeight; i++)
	{
		for(int j = 0; j < mArenaWidth; j++)
		{
			JVector2 l_pos(j,i);
			if(*GetTileType(l_pos) == 'S')
			{
				mEnemyStartPos = l_pos;
			}
			if(*GetTileType(l_pos) == 'E')
			{
				mEnemyExitPos = l_pos;
			}
			if(*GetTileType(l_pos) == 'C')
			{
				mEnemyTargetPos = JVector2(l_pos * GRID_SIZE);
			}
		}
	}
	return true;
}

const char* cArena::GetTileType(JVector2 _pos)
{
	if(_pos.x < 0 || _pos.x > mArenaWidth) return "_";
	if(_pos.y < 0 || _pos.y > mArenaHeight) return "_";
	return &mArenaTiles[(int)_pos.y][(int)_pos.x];
}

void cArena::Update()
{
	mCore->Update();
}

void cArena::Draw()
{
	for(int i = 0; i < mArenaHeight; i++)
	{
		for(int j = 0; j < mArenaWidth; j++)
		{
			if(mArenaTiles[i][j] == '_')
				mRen->RenderTexture(mTiles[0],GRID_SIZE*j,GRID_SIZE*i,0,GRID_SIZE,GRID_SIZE,WORLD_SPACE);
			if(mArenaTiles[i][j] == 'P')
				mRen->RenderTexture(mTiles[1],GRID_SIZE*j,GRID_SIZE*i,0,GRID_SIZE,GRID_SIZE,WORLD_SPACE);
		}
	}

	mCore->Draw();
}

stack<pair<int,int>> cArena::BreadthFirst(const pair<int,int> _start, const pair<int,int> _target)
{
	map<pair<int,int>,pair<int,int>> l_parent;
	l_parent[_start] = make_pair(-1,-1);
	vector<pair<int,int>> l_openList;
	l_openList.push_back(_start);
	pair<int,int> l_node;

	while(!l_openList.empty())
	{
		l_node = l_openList.back();
		if(l_node == _target) break;
		GraphNeighbours(l_node);
		l_openList.pop_back();
		for(int j = 0; j < 4; j++)
		{			
			if(CheckBounds(mAdj[j]) && l_parent.find(mAdj[j]) == l_parent.end())
			{
				l_parent[mAdj[j]] = l_node;
				l_openList.push_back(mAdj[j]);
			}
		}
	}
	
	l_node = _target;
	stack<pair<int,int>> l_path_s;
	l_path_s.push(l_node);
	while(l_node != _start)
	{
		l_node = l_parent[l_node];
		l_path_s.push(l_node);
	}

	return l_path_s;
}

void cArena::GraphNeighbours(pair<int,int> _u)
{
	//right
	char l_tile = *GetTileType(JVector2(_u.first + 1,_u.second));
	if(l_tile != '_')
	{
		mAdj[0].first =  _u.first + 1;
		mAdj[0].second =  _u.second;
	}
	else
		mAdj[0] = make_pair(-1,-1);
	//bottom
	l_tile = *GetTileType(JVector2(_u.first,_u.second + 1));
	if(l_tile != '_')
	{
		mAdj[1].first =  _u.first;
		mAdj[1].second =  _u.second + 1;
	}
	else
		mAdj[1] = make_pair(-1,-1);
	//left
	l_tile = *GetTileType(JVector2(_u.first - 1,_u.second));
	if(l_tile != '_')
	{
		mAdj[2].first =  _u.first - 1;
		mAdj[2].second =  _u.second;
	}
	else
		mAdj[2] = make_pair(-1,-1);
	//top
	l_tile = *GetTileType(JVector2(_u.first,_u.second - 1));
	if(l_tile != '_')
	{
		mAdj[3].first =  _u.first;
		mAdj[3].second =  _u.second - 1;
	}
	else
		mAdj[3] = make_pair(-1,-1);
}

/*
if _pos is out of bounds x and y are set to be in arena bounds.
only for world position (tile array index * grid size)
*/
void cArena::CheckBounds(float* _x, float* _y)
{
	if(*_x < 0.f) *_x = 0.f;
	if(*_y < 0.f) *_y = 0.f;
	if(*_x > mArenaWidthProdGrid-1) *_x = mArenaWidthProdGrid-1;
	if(*_y > mArenaHeightProdGrid-1) *_y = mArenaHeightProdGrid-1;
}

/*
returns true is _u is inside arena bounds.
returns false if x or y is outdise of arena bounds
only for tile array index values
*/
bool cArena::CheckBounds(pair<int,int> _u) const
{
	if(_u.first  < 0.f) return false;
	if(_u.second < 0.f) return false;
	if(_u.first  > mArenaWidth-1) return false;
	if(_u.second > mArenaHeight-1) return false;
	return true;
}
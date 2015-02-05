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
	//if(!LoadArenaData("")) return false;
	LoadArenaData("assets/arena/arena_01.tmx");
	//TODO: error report
	
	mCore = new cCore(mEnemyTargetPos.x,mEnemyTargetPos.y);
	mCore->Init(0);

	return true;
}

bool cArena::CleanUp()
{
	mCore->CleanUp();
	delete mCore; mCore = NULL;

	if(mTilesSpriteSheet) mRen->UnloadBitmap(mTilesSpriteSheet);
	mTilesSpriteSheet = NULL;

	mLog = NULL;
	mRen = NULL;

	return true;
}

int cArena::LoadArenaData(const char* _filename)
{
	TmxReturn l_error;

	l_error = parseFromFile(_filename,&mArena,"assets/arena/");
	if(!l_error)
	{
		mTilesSpriteSheet = mRen->LoadBitmap(mArena.tilesetCollection[0].image.source.c_str());
		if(!mTilesSpriteSheet) mLog->LogSDLError("LoadArena LoadBitmap");

		for(int i = 0; i < mArena.height; i++)
		{
			for(int j = 0; j < mArena.width; j++)
			{
				JVector2 l_pos(j,i);
				if(GetTileType(l_pos) == TILE_START)
				{
					mEnemyStartPos = l_pos;
				}
				if(GetTileType(l_pos) == TILE_EXIT)
				{
					mEnemyExitPos = l_pos;
				}
				if(GetTileType(l_pos) == TILE_CORE)
				{
					mEnemyTargetPos = JVector2(l_pos * GetGridSize());
				}
			}
		}		
	}
	else
	{
		//TODO: proper error reporting
		printf("error parsing file");
	}
	return l_error;
}

//TODO: consider changing to (int _x, int _y)
int cArena::GetTileType(JVector2 _pos)
{
	//TODO: only siupports one Tiled layer
	if(_pos.x < 0 || _pos.x > GetArenaWidth()) return TILE_EMPTY;
	if(_pos.y < 0 || _pos.y > GetArenaHeight()) return TILE_EMPTY;
	return mArena.layerCollection[0].tiles[_pos.y*mArena.width + _pos.x].tileFlatIndex;
}

void cArena::Update()
{
	mCore->Update();
}

void cArena::Draw()
{
	for(int i = 0; i < mArena.height; i++)
	{
		for(int j = 0; j < mArena.width; j++)
		{
			DrawTile(j*mArena.tileWidth,i*mArena.tileHeight,
				GetTileType(JVector2(j,i)),WORLD_SPACE);
		}
	}

	//mCore->Draw();
}

void cArena::DrawTile(int _x, int _y, int _tile_type, int _space)
{
	//TODO: might need a better method of recording tile location
	SDL_Rect l_tile = { _tile_type*mArena.tileWidth,0,mArena.tileWidth,mArena.tileHeight };
	mRen->RenderTexture(mTilesSpriteSheet,_x,_y,NULL,l_tile,_space);
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
	int l_tile = GetTileType(JVector2(_u.first + 1,_u.second));
	if(l_tile != TILE_EMPTY)
	{
		mAdj[0].first =  _u.first + 1;
		mAdj[0].second =  _u.second;
	}
	else
		mAdj[0] = make_pair(-1,-1);
	//bottom
	l_tile = GetTileType(JVector2(_u.first,_u.second + 1));
	if(l_tile != TILE_EMPTY)
	{
		mAdj[1].first =  _u.first;
		mAdj[1].second =  _u.second + 1;
	}
	else
		mAdj[1] = make_pair(-1,-1);
	//left
	l_tile = GetTileType(JVector2(_u.first - 1,_u.second));
	if(l_tile != TILE_EMPTY)
	{
		mAdj[2].first =  _u.first - 1;
		mAdj[2].second =  _u.second;
	}
	else
		mAdj[2] = make_pair(-1,-1);
	//top
	l_tile = GetTileType(JVector2(_u.first,_u.second - 1));
	if(l_tile != TILE_EMPTY)
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
	if(*_x > GetArenaWidth()-1) *_x = GetArenaWidth()-1;
	if(*_y > GetArenaHeight()-1) *_y = GetArenaHeight()-1;
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
	if(_u.first  > mArena.width-1) return false;
	if(_u.second > mArena.height-1) return false;
	return true;
}
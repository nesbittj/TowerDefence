#include "cArena.h"

bool cArena::Init()
{
	mRen = cRenderer::Instance();
	mLog = cLogger::Instance();
	mInput = cInput::Instance();
		
	mEnemyExitPos = mEnemyStartPos = JVector2();
	mArenaEditMode = 0;

	//if(!LoadArenaData("")) return false;
	LoadArenaData("assets/arena/arena_02.tmx");
	//TODO: error report

	BreadthFirst(
		make_pair((int)mEnemyExitPos.x,(int)mEnemyExitPos.y),
		make_pair((int)mEnemyStartPos.x,(int)mEnemyStartPos.y));
	mUpdateBFS = false;

	return true;
}

bool cArena::CleanUp()
{
	if(mTilesSpriteSheet) mRen->UnloadBitmap(mTilesSpriteSheet);
	mTilesSpriteSheet = NULL;

	mInput = NULL;
	mLog = NULL;
	mRen = NULL;

	return true;
}

/*
load arena data from tmx file _filename.
finds enemy start,exit and target positions.
loads arena sprite sheet.
returns zero on success.
*/
int cArena::LoadArenaData(const char* _filename)
{
	int l_result = 0;
	TmxReturn l_tmx_error = parseFromFile(_filename,&mArena,"assets/arena/");
	if(!l_tmx_error)
	{
		mTilesSpriteSheet = mRen->LoadBitmap(mArena.tilesetCollection[0].image.source.c_str());
		if(!mTilesSpriteSheet)
		{
			l_result = -1;
			mLog->LogSDLError("LoadArenaData() LoadBitmap()");
		}

		mArena.height -= 1;
		mArena.width -= 1;
		JVector2 l_pos;
		for(Uint32 i = 0; i <= mArena.height; i++)
		{
			for(Uint32 j = 0; j <= mArena.width; j++)
			{
				l_pos.Set((float)j,(float)i);
				if(GetTileType(l_pos) == TILE_START)
				{
					mEnemyStartPos = l_pos;
				}
				if(GetTileType(l_pos) == TILE_EXIT)
				{
					mEnemyExitPos = l_pos;
				}
			}
		}		
	}
	else
	{
		l_result = l_tmx_error;
		mLog->LogError("LoadArenaData() error parsing file");
	}
	return l_result;
}

/*
updates arena edit mode and core
*/
void cArena::Update()
{
	if(mInput->GetKeyDownNotRepeat(SDLK_1)) mArenaEditMode = 0;
	if(mInput->GetKeyDownNotRepeat(SDLK_2)) mArenaEditMode = 0;
	if(mInput->GetKeyDownNotRepeat(SDLK_3)) mArenaEditMode = 0;
	if(mInput->GetKeyDownNotRepeat(SDLK_4)) mArenaEditMode = 0;
	if(mInput->GetKeyDownNotRepeat(SDLK_5)) mArenaEditMode = 0;
	if(mInput->GetKeyDownNotRepeat(SDLK_6)) mArenaEditMode = 1;
	if(mInput->GetKeyDownNotRepeat(SDLK_7)) mArenaEditMode = 2;

	if(mArenaEditMode > 0)
	{
		if(mInput->GetMouseButtonDown(LEFT_MOUSE_BUTTON))
		{
			SetTileType(mRen->mCamera->GetCursorX()/GetGridSize(),mRen->mCamera->GetCursorY()/GetGridSize(),mArenaEditMode - 1);
		}
	}
	if(mUpdateBFS && !mInput->GetMouseButtonDown(LEFT_MOUSE_BUTTON))
	{
		mUpdateBFS = false;
		BreadthFirst(
			make_pair((int)mEnemyExitPos.x,(int)mEnemyExitPos.y),
			make_pair((int)mEnemyStartPos.x,(int)mEnemyStartPos.y));
	}
}

/*
draw arena, including all tiles and core.
also draws currently selected tileif in edit mode.
*/
void cArena::Draw()
{
	for(Uint32 i = 0; i <= mArena.height; i++)
	{
		for(Uint32 j = 0; j <= mArena.width; j++)
		{
			DrawTile(j*mArena.tileWidth,i*mArena.tileHeight,
				GetTileType(JVector2((float)j,(float)i)),WORLD_SPACE);
		}
	}

	if(mArenaEditMode > 0)
		DrawTile(mRen->mCamera->GetCursorX(),mRen->mCamera->GetCursorY(),mArenaEditMode-1,WORLD_SPACE);
}

/*
draw single tile.
x and y are world position values.
*/
void cArena::DrawTile(int _x, int _y, int _tile_type, int _space)
{
	//TODO: might need a better method of recording tile location
	SDL_Rect l_tile = { _tile_type*mArena.tileWidth,0,mArena.tileWidth,mArena.tileHeight };
	mRen->RenderTexture(mTilesSpriteSheet,(float)_x,(float)_y,NULL,l_tile,_space);
}

/*
finds shortest path from _Start to _target.
does not record path mPath:stack, use GetPathParent() to find path in revers order,
ie _start and _target are reversed.
returns true on success.
*/
bool cArena::BreadthFirst(const pair<int,int> _start, const pair<int,int> _target)
{
	int result = 0;
	mParent.clear();
	mParent[_start] = make_pair(-1,-1);
	queue<pair<int,int>> l_openList;
	l_openList.push(_start);
	pair<int,int> l_node;

	while(!l_openList.empty())
	{
		l_node = l_openList.front();
		if(l_node == _target) result = 1;
		GraphNeighbours(l_node);
		l_openList.pop();
		for(int j = 0; j < 4; j++)
		{			
			if(CheckIndexBounds(mAdj[j]) && mParent.find(mAdj[j]) == mParent.end())
			{
				mParent[mAdj[j]] = l_node;
				l_openList.push(mAdj[j]);
			}
		}
	}

	if(result == 0) // path to _target not found probably because of user edit, try again with vanila map
	{
		mParent.clear();
		mParent[_start] = make_pair(-1,-1);
		queue<pair<int,int>>().swap(l_openList);
		l_openList.push(_start);

		while(!l_openList.empty())
		{
			l_node = l_openList.front();
			if(l_node == _target) result = 2;
			GraphNeighbours(l_node,1);
			l_openList.pop();
			for(int j = 0; j < 4; j++)
			{			
				if(CheckIndexBounds(mAdj[j]) && mParent.find(mAdj[j]) == mParent.end())
				{
					mParent[mAdj[j]] = l_node;
					l_openList.push(mAdj[j]);
				}
			}
		}
	}

	/*
	if(result > 0)
	{
		l_node = _target;
		mPath.push(l_node);
		while(l_node != _start)
		{
			l_node = mParent[l_node];
			mPath.push(l_node);
		}
	}
	*/
	return 1 & result;
}

/*
x,y are world positions.
returns breadth first search parent of current tile.
*/
JVector2 cArena::GetPathParent(int _x, int _y)
{
	_x = cMaths::Round(_x,GetGridSize())/GetGridSize();
	_y = cMaths::Round(_y,GetGridSize())/GetGridSize();
	pair<int,int> t = mParent[make_pair(_x,_y)];
	return JVector2((float)t.first,(float)t.second);
}

/*
finds four neibours of tile _u,
records neighbours into mAjd[4].
tile type TILE_EMPTY recorded as [-1,-1],
all other tile types accepted as passable.
default _vanila = 0, set to 1 to get vanula map data.
*/
void cArena::GraphNeighbours(pair<int,int> _u, int _vanila)
{
	//right
	int l_tile = GetTileType(JVector2((float)_u.first + 1,(float)_u.second),_vanila);
	if(l_tile != TILE_EMPTY && l_tile != TILE_TOWER)
	{
		mAdj[0].first =  _u.first + 1;
		mAdj[0].second =  _u.second;
	}
	else
		mAdj[0] = make_pair(-1,-1);
	//bottom
	l_tile = GetTileType(JVector2((float)_u.first,(float)_u.second + 1),_vanila);
	if(l_tile != TILE_EMPTY && l_tile != TILE_TOWER)
	{
		mAdj[1].first =  _u.first;
		mAdj[1].second =  _u.second + 1;
	}
	else
		mAdj[1] = make_pair(-1,-1);
	//left
	l_tile = GetTileType(JVector2((float)_u.first - 1,(float)_u.second),_vanila);
	if(l_tile != TILE_EMPTY && l_tile != TILE_TOWER)
	{
		mAdj[2].first =  _u.first - 1;
		mAdj[2].second =  _u.second;
	}
	else
		mAdj[2] = make_pair(-1,-1);
	//top
	l_tile = GetTileType(JVector2((float)_u.first,(float)_u.second - 1),_vanila);
	if(l_tile != TILE_EMPTY && l_tile != TILE_TOWER)
	{
		mAdj[3].first =  _u.first;
		mAdj[3].second =  _u.second - 1;
	}
	else
		mAdj[3] = make_pair(-1,-1);	
}

/*
if x or y are out of bounds x or y respectively
are set to be in arena bounds.
only for world position (tile array index * grid size)
*/
void cArena::CheckWorldBounds(float* _x, float* _y)
{
	if(*_x < 0.f) *_x = 0.f;
	if(*_y < 0.f) *_y = 0.f;
	if(*_x > GetArenaWidth()) *_x = (float)(GetArenaWidth());
	if(*_y > GetArenaHeight()) *_y = (float)(GetArenaHeight());
}

/*
returns true is _u is inside arena bounds.
returns false if x or y is outdise of arena bounds
only for tile array index values
*/
bool cArena::CheckIndexBounds(pair<int,int> _u) const
{
	if(_u.first  < 0) return false;
	if(_u.second < 0) return false;
	if((unsigned int)_u.first  > mArena.width) return false;
	if((unsigned int)_u.second > mArena.height) return false;
	return true;
}

/*
x,y are index values (0 to arena.width),
divide by GetGridSize() to convert world positions to index values
default _vanila = 0, set to 1 to get vanula map data.
*/
ARENA_TILE_TYPE cArena::GetTileType(int _x, int _y, int _vanila)
{
	//TODO: only supports one Tiled layer
	if(_x < 0 || _x > (int)mArena.width
	|| _y < 0 || _y > (int)mArena.height) return TILE_EMPTY;
	Uint32 l_index = _y*(mArena.width) + _y + _x;
	if(_vanila) return (ARENA_TILE_TYPE)(mArena.layerCollection[0].tiles[l_index].gid - 1);
	return (ARENA_TILE_TYPE)mArena.layerCollection[0].tiles[l_index].tileFlatIndex;
}

/*
x,y are index values (0 to arena.width),
divide by GetGridSize() to convert world positions to index values
*/
void cArena::SetTileType(int _x, int _y, int _tile_type)
{
	//TODO: only supports one Tiled layer
	if(_x < 0 || _x > (int)mArena.width
	|| _y < 0 || _y > (int)mArena.height) return;
	if(_tile_type < 0 || _tile_type > TILE_TYPE_RESET) return;

	Uint32 l_index = (_y) * mArena.width + (_y) + (_x);
	TmxLayerTile& l_tile = mArena.layerCollection[0].tiles[l_index];

	if(l_tile.tileFlatIndex != _tile_type)
	{
		if(_tile_type == TILE_TYPE_RESET)
			l_tile.tileFlatIndex = l_tile.gid - 1;
		else
			l_tile.tileFlatIndex = _tile_type;
		mUpdateBFS = true;
	}
}
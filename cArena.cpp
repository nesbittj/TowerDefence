#include "cArena.h"

cArena::cArena(float _x, float _y)
{
	x = _x; y = _y;
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
	
	mCore = new cCore(mEnemyTargetPos.x,mEnemyTargetPos.y,GRID_SIZE);
	mCore->Init(0);

	return true;
}

bool cArena::CleanUp()
{
	mCore->CleanUp();
	delete mCore; mCore = NULL;

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

	//TODO: properly set bounds
	for(int i = 0; i < 15; i++)
	{
		for(int j = 0; j < 20; j++)
		{
			JVector2 l_pos(j,i);
			if(*GetTyleType(l_pos) == 'S')
			{
				mEnemyStartPos = l_pos;
			}
			if(*GetTyleType(l_pos) == 'E')
			{
				mEnemyExitPos = l_pos;
			}
			if(*GetTyleType(l_pos) == 'C')
			{
				mEnemyTargetPos = JVector2(l_pos * GRID_SIZE);
			}
		}
	}
	return true;
}

//TODO: spell tile correctly
const char* cArena::GetTyleType(JVector2 _pos)
{
	if(_pos.x < 0 || _pos.x > 20) return "_";
	if(_pos.y < 0 || _pos.y > 15) return "_";
	return &mArenaTiles[(int)_pos.y][(int)_pos.x];
}

void cArena::Update()
{
	mCore->Update();
}

void cArena::Draw()
{

	//SetPos(mRen->GetCamera()->GetPos().x,mRen->GetCamera()->GetPos().y);

	//screen space is used because x,y is set to camera pos (move has already happened)
	//TODO: properly enumerate tiles number and render values
	for(int i = 0; i < 15; i++)
	{
		for(int j = 0; j < 20; j++)
		{
			if(mArenaTiles[i][j] == '_')
				mRen->RenderTexture(mTiles[0],GRID_SIZE*j+x,GRID_SIZE*i+y,0,32,32,WORLD_SPACE);
			if(mArenaTiles[i][j] == 'P')
				mRen->RenderTexture(mTiles[1],GRID_SIZE*j+x,GRID_SIZE*i+y,0,32,32,WORLD_SPACE);
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
	char l_tile = *GetTyleType(JVector2(_u.first + 1,_u.second));
	if(l_tile != '_')
	{
		mAdj[0].first =  _u.first + 1;
		mAdj[0].second =  _u.second;
	}
	else
		mAdj[0] = make_pair(-1,-1);
	//bottom
	l_tile = *GetTyleType(JVector2(_u.first,_u.second + 1));
	if(l_tile != '_')
	{
		mAdj[1].first =  _u.first;
		mAdj[1].second =  _u.second + 1;
	}
	else
		mAdj[1] = make_pair(-1,-1);
	//left
	l_tile = *GetTyleType(JVector2(_u.first - 1,_u.second));
	if(l_tile != '_')
	{
		mAdj[2].first =  _u.first - 1;
		mAdj[2].second =  _u.second;
	}
	else
		mAdj[2] = make_pair(-1,-1);
	//top
	l_tile = *GetTyleType(JVector2(_u.first,_u.second - 1));
	if(l_tile != '_')
	{
		mAdj[3].first =  _u.first;
		mAdj[3].second =  _u.second - 1;
	}
	else
		mAdj[3] = make_pair(-1,-1);
}

bool cArena::CheckBounds(JVector2 _pos)
{
	if(_pos.x < 0.f) return false;
	if(_pos.y < 0.f) return false;
	if(_pos.x > 19) return false;
	if(_pos.y > 15) return false;
	return true;
}

bool cArena::CheckBounds(pair<int,int> _u)
{
	if(_u.first  < 0.f) return false;
	if(_u.second < 0.f) return false;
	if(_u.first  > 19) return false;
	if(_u.second > 15) return false;
	return true;
}

void cArena::SetPos(float _x, float _y)
{
	//TODO: check bounds
	x = _x;
	y = _y;
}

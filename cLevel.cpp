#include "cLevel.h"

cLevel::cLevel(float _x, float _y)
{
	x = _x; y = _y;
}

cLevel::~cLevel()
{
}

bool cLevel::Init()
{
	mRen = cRenderer::Instance();
	mLog = cLogger::Instance();
	if(!LoadLevelData("")) return false;
	return true;
}

bool cLevel::CleanUp()
{
	UnloadBitmaps();
	mLog = NULL;
	mRen = NULL;
	return true;
}

bool cLevel::UnloadBitmaps()
{
	for(int i = 0; i < NUM_TILES; i++)
	{
		if(mTiles[i]) SDL_DestroyTexture(mTiles[i]);
		mTiles[i] = NULL;
	}
	return true;
}

bool cLevel::LoadLevelData(const char* _filename)
{
	XMLDocument doc;
	if(!doc.LoadFile("assets/level/level_data.xml"))
	{
		XMLElement* l_tile_line = doc.FirstChildElement("layout")->FirstChild()->ToElement();
		int i = 0;
		for(l_tile_line; l_tile_line; l_tile_line=l_tile_line->NextSiblingElement())
		{
			mLevelTiles[i] = l_tile_line->Attribute("tiles");
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
		mLog->LogError("cLevel: XML failed to load");
		return false;
	}
	return true;
}

const char* cLevel::GetTyleType(float2 _pos)
{
	if(_pos.x < 0 || _pos.x > GRID_SIZE) return "BAD_VALUE";
	if(_pos.y < 0 || _pos.y > GRID_SIZE) return "BAD_VALUE";
	return ".";//mLevelTiles[(int)_pos.x][(int)_pos.y];
}

void cLevel::Draw()
{
	//screen space is used because x,y is set to camera pos (move has already happened)
	//TODO: properly enumerate tiles number and render values
	for(int i = 0; i < 15; i++)
	{
		for(int j = 0; j < 20; j++)
		{
			if(mLevelTiles[i][j] == '.')
				mRen->RenderTexture(mTiles[0],GRID_SIZE*j+x,GRID_SIZE*i+y,0,32,32,1);
			if(mLevelTiles[i][j] == 'P')
				mRen->RenderTexture(mTiles[1],GRID_SIZE*j+x,GRID_SIZE*i+y,0,32,32,1);
		}
	}
}

void cLevel::SetPos(float _x, float _y)
{
	//TODO: check bounds
	x = _x;
	y = _y;
}

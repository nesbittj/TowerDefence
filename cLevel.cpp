#include "cLevel.h"

cLevel::cLevel(float _x, float _y)
{
	x = _x; y = _y;
}

cLevel::~cLevel()
{
}

bool cLevel::Init(cCamera* _camera)
{
	mCamera = _camera;
	mRen = cRenderer::Instance();
	if(!LoadLevel("")) return false;
	if(!LoadBitmaps("")) return false;
	return true;
}

bool cLevel::CleanUp()
{
	UnloadBitmaps();
	mRen = NULL;
	mCamera = NULL;
	return true;
}

bool cLevel::LoadBitmaps(const char* _data)
{
	for(int i = 0; i < NUM_TILES; i++)
	{
		mTiles[i] = NULL;
		//open xml
		//read tile data
	}
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

bool cLevel::LoadLevel(const char* _filename)
{
	//open xml
	//read line
	for(int i = 0; i < GRID_SIZE; i++)
	{
		//read value
		for(int j = 0; j < GRID_SIZE; i++)
		{
			mLevelTiles[i][j] = ".";
		}
	}
	return true;
}

const char* cLevel::GetTyleType(float2 _pos)
{
	if(_pos.x < 0 || _pos.x > GRID_SIZE) return "BAD_VALUE";
	if(_pos.y < 0 || _pos.y > GRID_SIZE) return "BAD_VALUE";
	return mLevelTiles[(int)_pos.x][(int)_pos.y];
}

void cLevel::Draw()
{
	for(int i = 0; i < GRID_SIZE; i++)
	{
		for(int j = 0; j < GRID_SIZE; i++)
		{
			//TODO: properly enumerate tiles number and render values
			if(mLevelTiles[i][j] == ".") mRen->RenderTexture(mTiles[0],i,j,0,32,32);
			if(mLevelTiles[i][j] == "P") mRen->RenderTexture(mTiles[1],i,j,0,32,32);
		}
	}
}

void cLevel::SetPos(float _x, float _y)
{
	//TODO: check bounds
	x = _x; y = _y;
}

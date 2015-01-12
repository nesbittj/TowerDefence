#include "cEntity.h"

cEntity::cEntity(Uint32 _x, Uint32 _y, char* _bitmap_filename, Uint32 _grid_size)
{
	cEntity(_x,_y,(SDL_Texture*)NULL,_grid_size);
	LoadBitmap(_bitmap_filename);
}

cEntity::cEntity(Uint32 _x, Uint32 _y, SDL_Texture* _bitmap, Uint32 _grid_size)
{
	mLog = cLogger::Instance();
	mRen = cRenderer::Instance();
	mInput = cInput::Instance();
	SetPos(_x,_y);
	mGridSize = _grid_size;
	mLevel = 0;
	mLives = 10;
	mBitmap = _bitmap;
}

cEntity::~cEntity()
{
	mRen = NULL;
	mLog = NULL;
	UnloadBitmap();
}

void cEntity::SetPos(Uint32 _x, Uint32 _y)
{
	x = _x;	y = _y;
}

bool cEntity::LoadBitmap(char* filename)
{
	mBitmap = mRen->LoadTextureFromBMP(filename,NULL);
	if(mBitmap)
		return true;
	mLog->LogSDLError("cEntity::LoadBitmap() Error Loading Bitmap");
	return false;
}

void cEntity::UnloadBitmap()
{
	if(mBitmap) SDL_DestroyTexture(mBitmap);
	mBitmap = NULL;
}

void cEntity::Update()
{
}

void cEntity::Draw()
{
}
#include "cEntity.h"

cEntity::cEntity(Uint32 _x, Uint32 _y, Uint32 _grid_size)
{
	SetPos(_x,_y);
	mGridSize = _grid_size;
	mLevel = 0;
	mLives = 10;
}

cEntity::~cEntity()
{
}

bool cEntity::Init(SDL_Texture* _bitmap)
{
	mLog = cLogger::Instance();
	mRen = cRenderer::Instance();
	mInput = cInput::Instance();
	mBitmap = _bitmap;
	return true;
}

bool cEntity::CleanUp()
{
	mRen = NULL;
	mLog = NULL;
	mInput = NULL;
	mBitmap = NULL;
	return true;
}

void cEntity::SetPos(Uint32 _x, Uint32 _y) { x = _x; y = _y; }

void cEntity::Update()
{
}

void cEntity::Draw()
{
	mRen->RenderTexture(mBitmap,x,y,NULL);
}
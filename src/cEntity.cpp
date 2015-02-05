#include "cEntity.h"

cEntity::cEntity(Uint32 _x, Uint32 _y)
{
	SetPos(_x,_y);
	mLevel = 0;
	mLives = 10;
	mBitmap = NULL;
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

/*
make sure to destroy bitmap elsewhere
as this method does not.
*/
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

void cEntity::Draw(int _space)
{
	SDL_Color col = { 0,0,0,0 };
	mRen->RenderTexture(mBitmap,x,y,NULL,_space);
	mRen->RenderText(mLives,x+5,y+8,0,col,NULL,_space);
}
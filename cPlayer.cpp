#include "cPlayer.h"

cPlayer::cPlayer(int start_lives, int start_score, int grid_size, cCamera* _camera)
{
	mCurserPosX = 0;
	mCurserPosY = 0;
	mScore = start_score;
	mLives = start_lives;
	mGridSize = grid_size;

	mInput = cInput::Instance();
	mCamera = _camera;
}

/*
incroments mScore by one in positive or negative.
value of zero does nothing
*/
void cPlayer::IncromentScore(int _inc)
{
	if(_inc == 0) return;
	if(_inc < 0) mScore--;
	else mScore++;
	return;
}

/*
incroments mLives by one in positive or negative.
value of zero does nothing
*/
void cPlayer::IncromentLives(int _inc)
{
	if(_inc == 0) return;
	if(_inc < 0) mLives--;
	else mLives++;
	return;
}

void cPlayer::Update()
{
	JVector2 camPos = mCamera->GetPos();
	SetCurserX(cMaths::Round(mInput->GetMouseX(),mGridSize) + ((int)camPos.x % mGridSize));
	SetCurserY(cMaths::Round(mInput->GetMouseY(),mGridSize) + ((int)camPos.y % mGridSize));
}
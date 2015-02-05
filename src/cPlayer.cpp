#include "cPlayer.h"

cPlayer::cPlayer(int start_lives, int start_score)
{
	mScore = start_score;
	mLives = start_lives;
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
}
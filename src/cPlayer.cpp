#include "cPlayer.h"

cPlayer::cPlayer(int _start_lives, int _start_score, int _start_money)
{
	mScore = _start_score;
	mLives = _start_lives;
	mMoney = _start_money;

	mInput = cInput::Instance();
}

/*
incroments mScore by one in positive or negative.
value of zero does nothing
*/
void cPlayer::IncScore(int _inc)
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
void cPlayer::IncLives(int _inc)
{
	if(_inc == 0) return;
	if(_inc < 0) mLives--;
	else mLives++;
	return;
}

/*
give money to player.
uses abs() value always positive.
*/
void cPlayer::AddMoney(int _money)
{
	mMoney += abs(_money);
}

/*
take money from player.
uses abs(), always negative.
does not go below zero.
*/
void cPlayer::SpendMoney(int _money)
{
	mMoney -= abs(_money);
	if(mMoney < 0) mMoney = 0;
}

void cPlayer::Update()
{
	if(mInput->GetKeyDownNotRepeat(SDLK_g))
		AddMoney(100);
}
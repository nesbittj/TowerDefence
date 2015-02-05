#pragma once

#include <SDL.h>
#include "cInput.h"
#include "cCamera.h"
#include "cMaths.h"

class cPlayer
{
private:
	int mScore;
	int mLives;

public:
	inline int GetScore() const { return mScore; }
	inline int GetLives() const {return mLives; }

	void IncromentScore(int _int);
	void IncromentLives(int _inc);

	cPlayer() { mScore = 0, mLives = 10; }
	cPlayer(int start_lives, int start_score);
	~cPlayer() { }

	void Update();
};
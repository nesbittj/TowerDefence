#pragma once

#include <SDL.h>
#include "cInput.h"
#include "cCamera.h"
#include "cMaths.h"

class cPlayer
{
private:
	Sint32 mScore;
	Uint32 mLives;
	Uint32 mMoney;

	cInput* mInput;

public:
	inline const Sint32* GetScore() const { return &mScore; }
	inline const Uint32* GetLives() const { return &mLives; }
	inline const Uint32* GetMoney() const { return &mMoney; }

	void IncScore(int _inc);
	void IncLives(int _inc);
	void AddMoney(int _money);
	void SpendMoney(int _money);

	cPlayer() { mScore = 0, mLives = 10; mMoney = 1000; }
	cPlayer(int _start_lives, int _start_score, int _start_money);
	~cPlayer() { mInput = NULL; }

	void Update();
};
#pragma once

#include <SDL.h>
#include "cInput.h"
#include "cCamera.h"
#include "cMaths.h"

class cPlayer
{
private:
	int mCurserPosX;
	int mCurserPosY;
	int mScore;
	int mLives;
	int mGridSize;

	cInput* mInput;
	cCamera* mCamera;

public:
	inline int GetCurserX() const { return mCurserPosX; }
	inline int GetCurserY() const { return mCurserPosY; }
	inline int GetScore() const { return mScore; }
	inline int GetLives() const {return mLives; }

	inline void SetCurserX(int _x) { mCurserPosX = _x; }
	inline void SetCurserY(int _y) { mCurserPosY = _y; }
	void IncromentScore(int _int);
	void IncromentLives(int _inc);

	cPlayer() { mScore = 0, mLives = 10; mGridSize = 32; mCurserPosX = 0; mCurserPosY = 0; mInput = NULL; mCamera = NULL; }
	cPlayer(int start_lives, int start_score, int grid_size, cCamera* _camera);
	~cPlayer() { mGridSize = NULL; mInput = NULL; mCamera = NULL; }

	void Update();
};
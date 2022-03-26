#pragma once

#include "TetrisBlock.h"
#include "DDraw.h"

class Tetris
{
public:
	Tetris();
	~Tetris();

	void Initialize(HWND hwnd);
	void Process();
	void Update();
	void DrawScene();

	void OnKeyDown(WPARAM wParam, LPARAM lParam);
	void OnKeyUp(WPARAM wParam, LPARAM lParam);
	void OnUpdateWindowPos();
private:
	enum {
		MAP_WIDTH = 10,
		MAP_HEIGHT = 22,
		MAX_BLOCK_COUNT = 512,
		BLOCK_VERTEX_COUNT = 4,
	};

	DDraw* mpDDraw = nullptr;
	
	unsigned int mSeed = 0;
	BitmapImage* mBlockImage = nullptr;
	BitmapImage* mBackgroundImage = nullptr;
	TetrisBlock mCurrentTetrisBlock = { 0, };
	TetrisBlock mNextTetrisBlock = { 0, };
	BOOL mbExistFixedBlock[MAP_HEIGHT][MAP_WIDTH] = { 0, };

	BOOL mKeyUpPressed = false;
	BOOL mKeyDownPressed = false;
	BOOL mKeyRightPressed = false;
	BOOL mKeyLeftPressed = false;
	BOOL mShouldBlockDownOrStop = false;

	ULONGLONG mBlockDownOrStopMilliseconds = 0;
	ULONGLONG mFPSMilliseconds = 0;
};

extern Tetris* gpTetris;

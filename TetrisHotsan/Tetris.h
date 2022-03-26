#pragma once

#include "Coord.h"
#include "EBlockType.h"
#include "Block.h"
#include "BitmapImage.h"
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

private:
	enum {
		MAP_WIDTH = 10,
		MAP_HEIGHT = 22,
		MAX_BLOCK_COUNT = 512,
		BLOCK_VERTEX_COUNT = 4
	};

	DDraw* mpDDraw = nullptr;
	BitmapImage* mCell = nullptr;
	BitmapImage* mBackgroundImage = nullptr;

	double fps = 1;
	BOOL blockStopped = false;

	Block mBlock = { 0, };
	Block mNextBlock = { 0, };

	BOOL mbMap[MAP_HEIGHT][MAP_WIDTH] = { 0, };
	unsigned int mSeed = 0;

	BOOL mKeyUpPressed = false;
	BOOL mKeyDownPressed = false;
	BOOL mKeyRightPressed = false;
	BOOL mKeyLeftPressed = false;

	DWORD mShouldBlockDownOrStop = false;

	ULONGLONG mBlockDownOrStopMilliseconds = 0;
	ULONGLONG mFPSMilliseconds = 0;
};

extern Tetris* gpTetris;

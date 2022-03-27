#pragma once

#include "TetrisBlock.h"
#include "DDraw.h"
#include "TetrisMap.h"

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
		BLOCK_VERTEX_COUNT = 4,
	};

	DDraw* mpDDraw = nullptr;
	
	unsigned int mSeed = 0;
	BitmapImage* mBlockImages[(DWORD)EColor::Count] = { 0, };
	BitmapImage* mMapImage = nullptr;
	BitmapImage* mBackgroundImage = nullptr;
	TetrisBlock mCurrentTetrisBlock = { 0, };
	TetrisBlock mNextTetrisBlock = { 0, };

	TetrisMap mTetrisMap[(DWORD)ETetrisMap::HEIGHT][(DWORD)ETetrisMap::WIDTH] = {};
	DWORD mTetrisMapRowBlockCounts[(DWORD)ETetrisMap::HEIGHT] = { 0, };

	BOOL mKeyUpPressed = false;
	BOOL mKeyDownPressed = false;
	BOOL mKeyRightPressed = false;
	BOOL mKeyLeftPressed = false;
	BOOL mShouldBlockDownOrStop = false;

	ULONGLONG mBlockDownOrStopMilliseconds = 0;
	ULONGLONG mFPSMilliseconds = 0;
};

extern Tetris* gpTetris;

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif /* _CRT_SECURE_NO_WARNINGS */

#include <stdio.h>
#include <time.h>

#include "Tetris.h"
#include "TickCounter.h"

Tetris* gpTetris = nullptr;

Tetris::Tetris()
{
}

Tetris::~Tetris()
{
	if (mpDDraw != nullptr)
	{
		delete mpDDraw;
	}

	if (mMapImage != nullptr)
	{
		delete mMapImage;
	}

	if (mBackgroundImage != nullptr)
	{
		delete mBackgroundImage;
	}

	for (DWORD i = 0; i < (DWORD)EColor::Count; ++i)
	{
		if (mBlockImages[i] != nullptr)
		{
			delete mBlockImages[i];
		}
	}
}

void Tetris::Initialize(HWND hwnd)
{
	mpDDraw = new DDraw;
	mpDDraw->Initialize(hwnd);
	
	mSeed = (unsigned int)time(nullptr);
	srand(mSeed);

	mBackgroundImage = new BitmapImage;
	mBackgroundImage->Load24BitsBitmap("./assets/bonobono.bmp");

	mMapImage = new BitmapImage;
	mMapImage->Load24BitsBitmap("./assets/map2.bmp");

	char fileName[128];
	for (DWORD i = 0; i < (DWORD)EColor::Count; ++i)
	{
		sprintf(fileName, "./assets/block%u.bmp", i);

		mBlockImages[i] = new BitmapImage;
		mBlockImages[i]->Load24BitsBitmap(fileName);
	}

	mCurrentTetrisBlock = ::MakeRandomBlock((DWORD)ETetrisMap::WIDTH / 2, 0);
	mNextTetrisBlock = ::MakeRandomBlock((DWORD)ETetrisMap::WIDTH / 2, 0);

	::InitTickCounter(&mBlockDownOrStopMilliseconds);
	::InitTickCounter(&mFPSMilliseconds);
}

void Tetris::Process()
{
	DWORD FPSElasped = ::GetElaspedMillisecond(&mFPSMilliseconds);
	DWORD blockDownOrStopElasped = ::GetElaspedMillisecond(&mBlockDownOrStopMilliseconds);

	if (blockDownOrStopElasped >= 1000)
	{
		::InitTickCounter(&mBlockDownOrStopMilliseconds);
		mShouldBlockDownOrStop = TRUE;
	}

	if (FPSElasped >= 100)
	{
		::InitTickCounter(&mFPSMilliseconds);
		Update();
	}

	DrawScene();
}

void Tetris::Update()
{
	int absPosX = 0;
	int absPosY = 0;

	{
		int rotatedAbsPosX = 0;
		int rotatedAbsPosY = 0;

		BOOL bCanRotate = TRUE;
		BOOL bCanMoveRight = TRUE;
		BOOL bCanMoveLeft = TRUE;
		BOOL bCanMoveDown = TRUE;

		for (DWORD i = 0; i < BLOCK_VERTEX_COUNT; ++i)
		{
			::GetBlockAbsPos(&mCurrentTetrisBlock, i, mCurrentTetrisBlock.RotateCount, &absPosX, &absPosY);
			::GetBlockAbsPos(&mCurrentTetrisBlock, i, (mCurrentTetrisBlock.RotateCount + 1) % 4, &rotatedAbsPosX, &rotatedAbsPosY);

			if (rotatedAbsPosX >= (DWORD)ETetrisMap::WIDTH || rotatedAbsPosX < 0 || rotatedAbsPosY >= (DWORD)ETetrisMap::HEIGHT || TRUE == mTetrisMap[rotatedAbsPosX][rotatedAbsPosY].ExistFixedBlock)
			{
				bCanRotate = FALSE;
			}

			if (absPosX + 1 >= (DWORD)ETetrisMap::WIDTH || TRUE == mTetrisMap[absPosY][absPosX + 1].ExistFixedBlock)
			{
				bCanMoveRight = FALSE;
			}

			if (absPosX - 1 < 0 || TRUE == mTetrisMap[absPosY][absPosX - 1].ExistFixedBlock)
			{
				bCanMoveLeft = FALSE;
			}

			if (absPosY + 1 >= (DWORD)ETetrisMap::HEIGHT || TRUE == mTetrisMap[absPosY + 1][absPosX].ExistFixedBlock)
			{
				bCanMoveDown = FALSE;
			}
		}

		if (mKeyUpPressed && bCanRotate)
		{
			mCurrentTetrisBlock.RotateCount == 3 ? mCurrentTetrisBlock.RotateCount = 0 : mCurrentTetrisBlock.RotateCount++;
		}

		if (mKeyRightPressed && bCanMoveRight)
		{
			mCurrentTetrisBlock.pos.X += 1;
		}

		if (mKeyLeftPressed && bCanMoveLeft)
		{
			mCurrentTetrisBlock.pos.X -= 1;
		}

		if (mKeyDownPressed && bCanMoveDown)
		{
			mCurrentTetrisBlock.pos.Y += 1;
		}
	}
	
	if (mShouldBlockDownOrStop)
	{
		mShouldBlockDownOrStop = false;

		BOOL bCanBlockDown = TRUE;

		for (DWORD i = 0; i < BLOCK_VERTEX_COUNT; ++i)
		{
			::GetBlockAbsPos(&mCurrentTetrisBlock, i, mCurrentTetrisBlock.RotateCount, &absPosX, &absPosY);

			if (absPosY + 1 >= (DWORD)ETetrisMap::HEIGHT || TRUE == mTetrisMap[absPosY + 1][absPosX].ExistFixedBlock)
			{
				bCanBlockDown = FALSE;
			}
		}

		if (bCanBlockDown)
		{
			mCurrentTetrisBlock.pos.Y += 1;
			return;
		}

		for (DWORD i = 0; i < BLOCK_VERTEX_COUNT; ++i)
		{
			::GetBlockAbsPos(&mCurrentTetrisBlock, i, mCurrentTetrisBlock.RotateCount, &absPosX, &absPosY);

			mTetrisMap[absPosY][absPosX].Color = mCurrentTetrisBlock.Color;
			mTetrisMap[absPosY][absPosX].ExistFixedBlock = TRUE;

			mTetrisMapRowBlockCounts[absPosY]++;
		}

		// HACK: Will Fix later
		for (DWORD row = 0; row < (DWORD)ETetrisMap::HEIGHT; ++row)
		{
			DWORD rowBlockCount = mTetrisMapRowBlockCounts[row];

			if (rowBlockCount >= (DWORD)ETetrisMap::WIDTH)
			{
				for (int y = row; y > 0; --y)
				{
					for (int x = 0; x < (DWORD)ETetrisMap::WIDTH; ++x)
					{
						mTetrisMap[y][x].ExistFixedBlock = mTetrisMap[y - 1][x].ExistFixedBlock;
						mTetrisMapRowBlockCounts[y] = mTetrisMapRowBlockCounts[y - 1];
					}
				}
			}
		}

		mCurrentTetrisBlock = mNextTetrisBlock;
		mNextTetrisBlock = ::MakeRandomBlock((DWORD)ETetrisMap::WIDTH / 2, 0);
	}

	mShouldBlockDownOrStop = false;
}

void Tetris::DrawScene()
{
	mpDDraw->LockBackBuffer();
	{
		mpDDraw->DrawBitmapImage(0, 0, mBackgroundImage);
		mpDDraw->DrawBitmapImage(0, 0, mMapImage);
		
		const DWORD imageWidth = mBlockImages[(DWORD)mCurrentTetrisBlock.Color]->GetWidth();
		const DWORD imageHeight = mBlockImages[(DWORD)mCurrentTetrisBlock.Color]->GetHeight();

		for (DWORD i = 0; i < BLOCK_VERTEX_COUNT; ++i)
		{
			int absPosX = 0;
			int absPosY = 0;

			::GetBlockAbsPos(&mCurrentTetrisBlock, i, mCurrentTetrisBlock.RotateCount, &absPosX, &absPosY);

			mpDDraw->DrawBitmapImage(absPosX * imageWidth + 1, absPosY * imageHeight + 1, mBlockImages[(DWORD)mCurrentTetrisBlock.Color]);
		}

		for (DWORD i = 0; i < BLOCK_VERTEX_COUNT; ++i)
		{
			int absPosX = 0;
			int absPosY = 0;

			::GetBlockAbsPos(&mNextTetrisBlock, i, mNextTetrisBlock.RotateCount, &absPosX, &absPosY);

			mpDDraw->DrawBitmapImage(absPosX * imageWidth + 120, absPosY * imageHeight + 120, mBlockImages[(DWORD)mNextTetrisBlock.Color]);
		}

		const DWORD startX = 1;
		const DWORD startY = 1;

		for (DWORD y = 0; y < (DWORD)ETetrisMap::HEIGHT; ++y)
		{
			for (DWORD x = 0; x < (DWORD)ETetrisMap::WIDTH; ++x)
			{
				if (mTetrisMap[y][x].ExistFixedBlock == TRUE)
				{
					mpDDraw->DrawBitmapImage(startX + x * imageWidth,  startY + y * imageHeight, mBlockImages[(DWORD)mTetrisMap[y][x].Color]); // +1 to avoid border
				}
			}
		}
	}
	mpDDraw->UnlockBackBuffer();
	mpDDraw->Blt();
}

void Tetris::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case VK_UP:
		mKeyUpPressed = true;
		break;
	case VK_LEFT:
		mKeyLeftPressed = true;
		break;
	case VK_RIGHT:
		mKeyRightPressed = true;
		break;
	case VK_DOWN:
		mKeyDownPressed = true;
		break;
	default:
		break;
	}
}

void Tetris::OnKeyUp(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case VK_UP:
		mKeyUpPressed = false;
		break;
	case VK_LEFT:
		mKeyLeftPressed = false;
		break;
	case VK_RIGHT:
		mKeyRightPressed = false;
	case VK_DOWN:
		mKeyDownPressed = false;
	default:
		break;
	}
}

void Tetris::OnUpdateWindowPos()
{
	if (nullptr != mpDDraw)
	{
		mpDDraw->OnUpdateWindowPos();
	}
}
#include <time.h>
#include <stdlib.h>

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

	if (mBackgroundImage != nullptr)
	{
		delete mBackgroundImage;
	}

	if (mCell != nullptr)
	{
		delete mCell;
	}
}

void Tetris::Initialize(HWND hwnd)
{
	mpDDraw = new DDraw;
	mpDDraw->Initialize(hwnd);
	
	mSeed = time(nullptr);
	srand(mSeed);

	mBackgroundImage = new BitmapImage;
	mBackgroundImage->Load24BitsBitmap("./assets/map2.bmp");

	mCell = new BitmapImage;
	mCell->Load24BitsBitmap("./assets/redBlockGradient.bmp");

	mBlock = ::MakeRandomBlock(MAP_WIDTH / 2, 0);
	mNextBlock = ::MakeRandomBlock(MAP_WIDTH / 2, 0);

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

	int rotatedAbsPosX = 0;
	int rotatedAbsPosY = 0;

	BOOL bCanRotate = TRUE;
	BOOL bCanMoveRight = TRUE;
	BOOL bCanMoveLeft = TRUE;
	BOOL bCanMoveDown = TRUE;

	for (DWORD i = 0; i < BLOCK_VERTEX_COUNT; ++i)
	{
		::GetBlockAbsCoord(&mBlock, i, mBlock.RotateCount, &absPosX, &absPosY);
		::GetBlockAbsCoord(&mBlock, i, (mBlock.RotateCount + 1) % 4, &rotatedAbsPosX, &rotatedAbsPosY);

		if (rotatedAbsPosX >= MAP_WIDTH || rotatedAbsPosX < 0 || rotatedAbsPosY >= MAP_HEIGHT || true == mbMap[rotatedAbsPosX][rotatedAbsPosY])
		{
			bCanRotate = FALSE;
		}

		if (absPosX + 1 >= MAP_WIDTH || true == mbMap[absPosY][absPosX + 1])
		{
			bCanMoveRight = FALSE;
		}

		if (absPosX - 1 < 0 || true == mbMap[absPosY][absPosX - 1])
		{
			bCanMoveLeft = FALSE;
		}

		if (absPosY + 1 >= MAP_HEIGHT || true == mbMap[absPosY + 1][absPosX])
		{
			bCanMoveDown = FALSE;
		}
	}

	if (mKeyUpPressed && bCanRotate)
	{
		mBlock.RotateCount == 3 ? mBlock.RotateCount = 0 : mBlock.RotateCount++;
	}

	if (mKeyRightPressed && bCanMoveRight)
	{
		mBlock.pos.X += 1;
	}

	if (mKeyLeftPressed && bCanMoveLeft)
	{
		mBlock.pos.X -= 1;
	}

	if (mKeyDownPressed && bCanMoveDown)
	{
		mBlock.pos.Y += 1;
	}

	if (mShouldBlockDownOrStop)
	{
		BOOL bCanBlockDown = TRUE;

		for (DWORD i = 0; i < BLOCK_VERTEX_COUNT; ++i)
		{
			::GetBlockAbsCoord(&mBlock, i, mBlock.RotateCount, &absPosX, &absPosY);

			if (absPosY + 1 >= MAP_HEIGHT || true == mbMap[absPosY + 1][absPosX])
			{
				bCanBlockDown = FALSE;
			}
		}

		if (bCanBlockDown)
		{
			mBlock.pos.Y += 1;
		}
		else
		{
			for (DWORD i = 0; i < BLOCK_VERTEX_COUNT; ++i)
			{
				::GetBlockAbsCoord(&mBlock, i, mBlock.RotateCount, &absPosX, &absPosY);

				mbMap[absPosY][absPosX] = true;
			}

			// HACK: Will Fix later
			for (DWORD y = 0; y < MAP_HEIGHT; ++y)
			{
				DWORD count = 0;
				for (DWORD x = 0; x < MAP_WIDTH; ++x)
				{
					if (mbMap[y][x] == true)
					{
						count++;
					}
				}

				if (count >= MAP_WIDTH - 1)
				{
					for (DWORD x = 0; x < MAP_WIDTH; ++x)
					{
						mbMap[y][x] = false;
					}
				}
			}

			mBlock = mNextBlock;
			mNextBlock = ::MakeRandomBlock(MAP_WIDTH / 2, 0);
		}

		mShouldBlockDownOrStop = false;
	}
}

void Tetris::DrawScene()
{
	mpDDraw->LockBackBuffer();
	{
		mpDDraw->DrawBitmapImage(0, 0, mBackgroundImage);
		
		const DWORD cellWidth = mCell->GetWidth();
		const DWORD cellHeight = mCell->GetHeight();

		for (DWORD i = 0; i < BLOCK_VERTEX_COUNT; ++i)
		{
			int absPosX = 0;
			int absPosY = 0;

			::GetBlockAbsCoord(&mBlock, i, mBlock.RotateCount, &absPosX, &absPosY);

			mpDDraw->DrawBitmapImage(absPosX * cellWidth + 1, absPosY * cellHeight + 1, mCell);
		}

		const DWORD startX = 1;
		const DWORD startY = 1;

		for (DWORD y = 0; y < MAP_HEIGHT; ++y)
		{
			for (DWORD x = 0; x < MAP_WIDTH; ++x)
			{
				if (mbMap[y][x] == true)
				{
					mpDDraw->DrawBitmapImage(startX + x * cellWidth,  startY + y * cellHeight, mCell); // +1 to avoid border
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

/*
if (blockStopped) {
	blockStopped = false;

	for (size_t i = 0; i < BLOCK_VERTEX_COUNT; ++i) {
		int absPosX = mBlock.pos.X + blockVertexArray[mBlock.BlockType][mBlock.RotateCount][i].X;
		int absPosY = mBlock.pos.Y + blockVertexArray[mBlock.BlockType][mBlock.RotateCount][i].Y;

		mbMap[absPosY][absPosX] = true;
	}

	mBlock = ::MakeRandomBlock(0, 0);

}

for (size_t i = 0; i < BLOCK_VERTEX_COUNT; ++i) {
	int absPosX = mBlock.pos.X + blockVertexArray[mBlock.BlockType][mBlock.RotateCount][i].X;
	int absPosY = mBlock.pos.Y + blockVertexArray[mBlock.BlockType][mBlock.RotateCount][i].Y;

	if (absPosY + 1 >= 22 || true == mbMap[absPosY + 1][absPosX]) {
		blockStopped = true;
	}
}

if (!blockStopped)
{
	mBlock.pos.Y += 1;
}
*/
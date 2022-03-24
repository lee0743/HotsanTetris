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

	::InitTickCounter(&mblockDownOrStopMilliseconds);
	::InitTickCounter(&mFPSMilliseconds);
}

void Tetris::Process()
{
	DWORD mFPSElasped = ::GetElaspedMillisecond(&mFPSMilliseconds);
	DWORD mblockDownOrStopElasped = ::GetElaspedMillisecond(&mblockDownOrStopMilliseconds);

	if (mblockDownOrStopElasped >= 1000)
	{
		::InitTickCounter(&mblockDownOrStopMilliseconds);
		
		mShouldBlockDownOrStop = TRUE;
		Update();
	}

	if (mFPSElasped >= 30)
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

	BOOL bCanRotate = TRUE;
	BOOL bCanMoveRight = TRUE;
	BOOL bCanMoveLeft = TRUE;

	for (DWORD i = 0; i < BLOCK_VERTEX_COUNT; ++i)
	{
		::GetBlockAbsCoord(&mBlock, i, &absPosX, &absPosY);

		if (absPosX + 1 >= MAP_WIDTH || true == mbMap[absPosY][absPosX + 1])
		{
			bCanMoveRight = FALSE;
		}

		if ((int)absPosX - 1 < 0 || true == mbMap[absPosY][absPosX - 1])
		{
			bCanMoveLeft = FALSE;
		}
	}

	if (mKeyUpPressed)
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

	if (mShouldBlockDownOrStop)
	{
		BOOL bCanBlockDown = TRUE;

		for (DWORD i = 0; i < BLOCK_VERTEX_COUNT; ++i)
		{
			::GetBlockAbsCoord(&mBlock, i, &absPosX, &absPosY);

			if (absPosY + 1 >= MAP_HEIGHT || true == mbMap[absPosY + 1][absPosX])
			{
				bCanBlockDown = FALSE;
			}
		}

		if (bCanBlockDown)
		{
			mBlock.pos.Y += 1;
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

			::GetBlockAbsCoord(&mBlock, i, &absPosX, &absPosY);

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
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

	mBackgroundImage = new BitmapImage();
	mBackgroundImage->Load24BitsBitmap("./assets/map2.bmp");

	mCell = new BitmapImage();
	mCell->Load24BitsBitmap("./assets/redBlockGradient.bmp");
	mBlock = ::MakeRandomBlock(MAP_WIDTH / 2, 0);
}

void Tetris::Process()
{
	DWORD elapsedMillisecond = GetElaspedMillisecond();

	if (elapsedMillisecond >= 300)
	{
		InitTickCounter();
		Update();
	}
	
	DrawScene();
}

void Tetris::Update()
{
	DWORD rotatedAbsPosX = 0;
	DWORD rotatedAbsPosY = 0;
	DWORD absPosX = 0;
	DWORD absPosY = 0;

	BOOL bCanRotate = TRUE;
	BOOL bCanMoveRight = TRUE;
	BOOL bCanMoveLeft = TRUE;

	for (DWORD i = 0; i < BLOCK_VERTEX_COUNT; ++i)
	{
		::GetBlockAbsCoord(&mBlock, i, &absPosX, &absPosY);

		if (absPosX + 1 <= MAP_WIDTH || true == mbMap[absPosY][absPosX + 1])
		{
			bCanMoveRight = FALSE;
		}

		if (absPosX - 1 > 0 || true == mbMap[absPosY][absPosX - 1])
		{
			bCanMoveRight = FALSE;
		}
	}

	if (mKeyUpPressed)
	{
			mBlock.RotateCount += 1;
	}

	if (mKeyRightPressed && bCanMoveRight)
	{
		mBlock.pos.X += 1;
	}

	if (mkeyLeftPressed && bCanMoveLeft)
	{
		mBlock.pos.X -= 1;
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
			DWORD absPosX = 0;
			DWORD absPosY = 0;

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
		mkeyLeftPressed = true;
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
		mkeyLeftPressed = false;
		break;
	case VK_RIGHT:
		mkeyRightPressed = false;
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
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
	mSeed = time(nullptr);
	srand(mSeed);
	
	mpDDraw = new DDraw;
	mpDDraw->Initialize(hwnd);

	mBackgroundImage = new BitmapImage();
	mBackgroundImage->Load24BitsBitmap("./assets/map2.bmp");

	mCell = new BitmapImage();
	mCell->Load24BitsBitmap("./assets/redBlockGradient.bmp");
	mBlock = ::MakeRandomBlock(MAP_WIDTH / 2, 0);
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
			int absPosX = mBlock.pos.X + blockVertexArray[mBlock.BlockType][mBlock.RotateCount][i].X;
			int absPosY = mBlock.pos.Y + blockVertexArray[mBlock.BlockType][mBlock.RotateCount][i].Y;

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

void Tetris::Update()
{
	DWORD elapsedMillisecond = GetElaspedMillisecond();
	InitTickCounter();

	if (elapsedMillisecond >= 1000)
	{
		mBlock.pos.Y += 1;
	}

	DrawScene();
}

Block * Tetris::GetBlock()
{
	return &mBlock;
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
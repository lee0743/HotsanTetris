#include <time.h>
#include <stdlib.h>

#include "Tetris.h"

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
	mCell->Load24BitsBitmap("./assets/redBlock.bmp");
	mBlock = ::MakeRandomBlock(0, 0);

	for (DWORD y = 0; y < MAP_HEIGHT; ++y)
	{
		for (DWORD x = 0; x < MAP_WIDTH; ++x)
		{
			mbMap[random()%MAP_HEIGHT][random()]
		}
	}
}

void Tetris::DrawScene()
{
	mpDDraw->LockBackBuffer();
	{
		mpDDraw->DrawBitmapImage(0, 0, mBackgroundImage);
		
		DWORD cellWidth = mCell->GetWidth() * 2;
		DWORD cellHeight = mCell->GetHeight() * 2;

		for (DWORD y = 0; y < MAP_HEIGHT; ++y)
		{
			for (DWORD x = 0; x < MAP_WIDTH; ++x)
			{
				if (mbMap[y][x] == true)
				{
					mpDDraw->DrawBitmapImage(x * cellWidth, y * cellHeight, mCell);
				}
			}
		}
	}
	mpDDraw->UnlockBackBuffer();
	mpDDraw->Blt();
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
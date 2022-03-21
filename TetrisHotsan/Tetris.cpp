#include <time.h>
#include <stdlib.h>

#include "Tetris.h"

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
	/*
	for (int i = 0; i < 30; ++i)
	{
		mbMap[rand() % (MAP_HEIGHT - 10) + 10][rand() % MAP_WIDTH] = true;
	}
	*/
}

void Tetris::DrawScene()
{
	mpDDraw->LockBackBuffer();
	{
		mpDDraw->DrawBitmapImage(0, 0, mBackgroundImage);
		
		DWORD cellWidth = mCell->GetWidth();
		DWORD cellHeight = mCell->GetHeight();

		for (DWORD i = 0; i < BLOCK_VERTEX_COUNT; ++i)
		{
			int absPosX = mBlock.pos.X + blockVertexArray[mBlock.BlockType][mBlock.RotateCount][i].X;
			int absPosY = mBlock.pos.Y + blockVertexArray[mBlock.BlockType][mBlock.RotateCount][i].Y;

			mpDDraw->DrawBitmapImage(absPosX * cellWidth + 1, absPosY * cellHeight + 1, mCell);
		}

		for (DWORD y = 0; y < MAP_HEIGHT; ++y)
		{
			for (DWORD x = 0; x < MAP_WIDTH; ++x)
			{
				if (mbMap[y][x] == true)
				{
					mpDDraw->DrawBitmapImage(x * cellWidth + 1, y * cellHeight + 1, mCell); // +1 to avoid border
				}
			}
		}
		
		mBlock.pos.Y += 1;

		int randrandNum = (rand() % 3);

		if (randrandNum == 0 || randrandNum == 1)
		{
			int randomNum = (rand() % 3) - 1;
			bool canMove = true;

			for (size_t i = 0; i < BLOCK_VERTEX_COUNT; ++i) {
				int absPosX = mBlock.pos.X + blockVertexArray[mBlock.BlockType][mBlock.RotateCount][i].X;
				int absPosY = mBlock.pos.Y + blockVertexArray[mBlock.BlockType][mBlock.RotateCount][i].Y;

				if (true == mbMap[absPosY][absPosX + randomNum] || MAP_WIDTH <= mbMap[absPosY][absPosX + randomNum] || 0 > mbMap[absPosY][absPosX + randomNum]) {
					canMove = false;
				}
			}

			if (canMove)
			{
				mBlock.pos.X += randomNum;
			}
		}
		else
		{
			bool canMove = true;

			for (size_t i = 0; i < BLOCK_VERTEX_COUNT; ++i) {
				int absPosX = mBlock.pos.X + blockVertexArray[mBlock.BlockType][(mBlock.RotateCount + 1) % 4][i].X;
				int absPosY = mBlock.pos.Y + blockVertexArray[mBlock.BlockType][(mBlock.RotateCount + 1) % 4][i].Y;

				if (true == mbMap[absPosY][absPosX] || MAP_WIDTH <= mbMap[absPosY][absPosX] || 0 > mbMap[absPosY][absPosX]) {
					canMove = false;
				}
			}

			if (canMove)
			{
				mBlock.RotateCount = (mBlock.RotateCount + 1) % 4;
			}
		}
		
	}
	mpDDraw->UnlockBackBuffer();
	mpDDraw->Blt();
}

void Tetris::Update()
{
	for (size_t i = 0; i < BLOCK_VERTEX_COUNT; ++i) {
		int absPosX = mBlock.pos.X + blockVertexArray[mBlock.BlockType][mBlock.RotateCount][i].X;
		int absPosY = mBlock.pos.Y + blockVertexArray[mBlock.BlockType][mBlock.RotateCount][i].Y;

		if (absPosY + 1 > 22 || true == mbMap[absPosY][absPosX]) {
			for (size_t i = 0; i < BLOCK_VERTEX_COUNT; ++i) {
				int absPosX = mBlock.pos.X + blockVertexArray[mBlock.BlockType][mBlock.RotateCount][i].X;
				int absPosY = mBlock.pos.Y + blockVertexArray[mBlock.BlockType][mBlock.RotateCount][i].Y - 1;

				mbMap[absPosY][absPosX] = true;
			}

			mBlock = ::MakeRandomBlock(MAP_WIDTH / 2, 0);
		}
	}
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
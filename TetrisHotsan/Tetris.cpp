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

	if (mBackgroundImage != nullptr)
	{
		delete mBackgroundImage;
	}

	if (mBlockImage != nullptr)
	{
		delete mBlockImage;
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

	mBlockImage = new BitmapImage;
	mBlockImage->Load24BitsBitmap("./assets/redBlockGradient.bmp");

	mCurrentTetrisBlock = ::MakeRandomBlock(MAP_WIDTH / 2, 0);
	mNextTetrisBlock = ::MakeRandomBlock(MAP_WIDTH / 2, 0);

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
		::GetBlockAbsPos(&mCurrentTetrisBlock, i, mCurrentTetrisBlock.RotateCount, &absPosX, &absPosY);
		::GetBlockAbsPos(&mCurrentTetrisBlock, i, (mCurrentTetrisBlock.RotateCount + 1) % 4, &rotatedAbsPosX, &rotatedAbsPosY);

		if (rotatedAbsPosX >= MAP_WIDTH || rotatedAbsPosX < 0 || rotatedAbsPosY >= MAP_HEIGHT || true == mbExistFixedBlock[rotatedAbsPosX][rotatedAbsPosY])
		{
			bCanRotate = FALSE;
		}

		if (absPosX + 1 >= MAP_WIDTH || true == mbExistFixedBlock[absPosY][absPosX + 1])
		{
			bCanMoveRight = FALSE;
		}

		if (absPosX - 1 < 0 || true == mbExistFixedBlock[absPosY][absPosX - 1])
		{
			bCanMoveLeft = FALSE;
		}

		if (absPosY + 1 >= MAP_HEIGHT || true == mbExistFixedBlock[absPosY + 1][absPosX])
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

	if (mShouldBlockDownOrStop)
	{
		BOOL bCanBlockDown = TRUE;

		for (DWORD i = 0; i < BLOCK_VERTEX_COUNT; ++i)
		{
			::GetBlockAbsPos(&mCurrentTetrisBlock, i, mCurrentTetrisBlock.RotateCount, &absPosX, &absPosY);

			if (absPosY + 1 >= MAP_HEIGHT || true == mbExistFixedBlock[absPosY + 1][absPosX])
			{
				bCanBlockDown = FALSE;
			}
		}

		if (bCanBlockDown)
		{
			mCurrentTetrisBlock.pos.Y += 1;
		}
		else
		{
			for (DWORD i = 0; i < BLOCK_VERTEX_COUNT; ++i)
			{
				::GetBlockAbsPos(&mCurrentTetrisBlock, i, mCurrentTetrisBlock.RotateCount, &absPosX, &absPosY);

				mbExistFixedBlock[absPosY][absPosX] = true;
			}

			// HACK: Will Fix later
			for (DWORD y = 0; y < MAP_HEIGHT; ++y)
			{
				DWORD count = 0;
				for (DWORD x = 0; x < MAP_WIDTH; ++x)
				{
					if (mbExistFixedBlock[y][x] == true)
					{
						count++;
					}
				}

				if (count >= MAP_WIDTH)
				{
					for (int yPos = y; yPos > 0; --yPos)
					{
						for (int x = 0; x < MAP_WIDTH; ++x)
						{
							mbExistFixedBlock[yPos][x] = mbExistFixedBlock[yPos - 1][x];
						}
					}
				}
			}

			mCurrentTetrisBlock = mNextTetrisBlock;
			mNextTetrisBlock = ::MakeRandomBlock(MAP_WIDTH / 2, 0);
		}

		mShouldBlockDownOrStop = false;
	}
}

void Tetris::DrawScene()
{
	mpDDraw->LockBackBuffer();
	{
		mpDDraw->DrawBitmapImage(0, 0, mBackgroundImage);
		
		const DWORD cellWidth = mBlockImage->GetWidth();
		const DWORD cellHeight = mBlockImage->GetHeight();

		for (DWORD i = 0; i < BLOCK_VERTEX_COUNT; ++i)
		{
			int absPosX = 0;
			int absPosY = 0;

			::GetBlockAbsPos(&mCurrentTetrisBlock, i, mCurrentTetrisBlock.RotateCount, &absPosX, &absPosY);

			mpDDraw->DrawBitmapImage(absPosX * cellWidth + 1, absPosY * cellHeight + 1, mBlockImage);
		}

		const DWORD startX = 1;
		const DWORD startY = 1;

		for (DWORD y = 0; y < MAP_HEIGHT; ++y)
		{
			for (DWORD x = 0; x < MAP_WIDTH; ++x)
			{
				if (mbExistFixedBlock[y][x] == true)
				{
					mpDDraw->DrawBitmapImage(startX + x * cellWidth,  startY + y * cellHeight, mBlockImage); // +1 to avoid border
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
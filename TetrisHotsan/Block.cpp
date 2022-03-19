#include "Block.h"

Block::Block()
{
	for (DWORD i = 0; i < MAX_BLOCK_UNIT_COUNT; ++i)
	{
		mBlockUnits[i].Coord = { 0, };
		mBlockUnits[i].blockImage = nullptr;
	}
}

Block::~Block()
{
	for (DWORD i = 0; i < MAX_BLOCK_UNIT_COUNT; ++i)
	{
		if (mBlockUnits[i].blockImage != nullptr)
		{
			delete mBlockUnits[i].blockImage;
			mBlockUnits[i].blockImage = nullptr;
		}
	}
}

Point Block::GetPos() const
{
	return mPositon;
}

void Block::GetPos(int * outX, int * outY) const
{
	*outX = mPositon.X;
	*outY = mPositon.Y;
}

void Block::SetPos(Point point)
{
	mPositon = point;
}

void Block::SetPos(int x, int y)
{
	mPositon.X = x;
	mPositon.Y = y;
}

const BlockUnit * Block::GetBlockUnits() const
{
	return mBlockUnits;
}

void Block::AddBlockUnit(Rect Coord, BlockImage * blockUnits)
{
	mBlockUnits[mBlockUnitNum].Coord = Coord;
	mBlockUnits[mBlockUnitNum].blockImage = new BlockImage(*blockUnits);
	mBlockUnitNum++;
}

DWORD Block::GetBlockUnitCount() const
{
	return mBlockUnitNum;
}

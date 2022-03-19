#include "Block.h"

Block::Block()
{
}

Block::~Block()
{
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

BlockImage * Block::GetImage()
{
	return mImage;
}

void Block::SetImage(BlockImage * Image)
{
	mImage = Image;
}

#include "Block.h"

Block MakeRandomBlock(int x, int y)
{
	Block block;
	{
		block.pos.X = x;
		block.pos.Y = y;
		block.BlockType = (EBlockType)(rand() % EBlockTypeCount);
		block.RotateCount = 0;
	}

	return block;
}
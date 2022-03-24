#include "Block.h"

Point blockVertexArray[EBlockTypeCount][4][4] = {
	{ // L Shape 
		{ Point{0, 0}, Point{0, 1}, Point{0, 2}, Point{1, 2} },
		{ Point{0, 0}, Point{1, 0}, Point{2, 0}, Point{0, 1} },
		{ Point{0, 0}, Point{1, 0}, Point{1, 1}, Point{1, 2} },
		{ Point{2, 0}, Point{0, 1}, Point{1, 1}, Point{2, 1} }
	},

	{ // T Shape
		{ Point{1, 0}, Point{0, 1}, Point{1, 1}, Point{2, 1} },
		{ Point{0, 0}, Point{0, 1}, Point{1, 1}, Point{0, 2} },
		{ Point{0, 0}, Point{1, 0}, Point{2, 0}, Point{1, 1} },
		{ Point{1, 0}, Point{0, 1}, Point{1, 1}, Point{1, 2} }
	},

	{ // O Shape
		{ Point{0, 0}, Point{1, 0}, Point{0, 1}, Point{1, 1} },
		{ Point{0, 0}, Point{1, 0}, Point{0, 1}, Point{1, 1} },
		{ Point{0, 0}, Point{1, 0}, Point{0, 1}, Point{1, 1} },
		{ Point{0, 0}, Point{1, 0}, Point{0, 1}, Point{1, 1} }
	},

	{ // Z Shape
		{ Point{0, 0}, Point{1, 0}, Point{1, 1}, Point{2, 1} },
		{ Point{1, 0}, Point{0, 1}, Point{1, 1}, Point{0, 2} },
		{ Point{0, 0}, Point{1, 0}, Point{1, 1}, Point{2, 1} },
		{ Point{1, 0}, Point{0, 1}, Point{1, 1}, Point{0, 2} }
	},

	{ // InvL Shape
		{ Point{1, 0}, Point{1, 1}, Point{0, 2}, Point{1, 2} },
		{ Point{0, 0}, Point{0, 1}, Point{1, 1}, Point{2, 1} },
		{ Point{0, 0}, Point{1, 0}, Point{0, 1}, Point{0, 2} },
		{ Point{0, 0}, Point{1, 0}, Point{2, 0}, Point{2, 1} }
	},

	{ // InvZ Shape
		{ Point{1, 0}, Point{2, 0}, Point{0, 1}, Point{1, 1} },
		{ Point{0, 0}, Point{0, 1}, Point{1, 1}, Point{1, 2} },
		{ Point{1, 0}, Point{2, 0}, Point{0, 1}, Point{1, 1} },
		{ Point{0, 0}, Point{0, 1}, Point{1, 1}, Point{1, 2} }
	},

	{ // I Shape
		{ Point{0, 0}, Point{0, 1}, Point{0, 2}, Point{0, 3} },
		{ Point{0, 0}, Point{1, 0}, Point{2, 0}, Point{3, 0} },
		{ Point{0, 0}, Point{0, 1}, Point{0, 2}, Point{0, 3} },
		{ Point{0, 0}, Point{1, 0}, Point{2, 0}, Point{3, 0} }
	}
};

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

void GetBlockAbsCoord(const Block* block, DWORD vertexIndex, int* outX, int* outY)
{
	int absPosX = block->pos.X + blockVertexArray[block->BlockType][block->RotateCount][vertexIndex].X;
	int absPosY = block->pos.Y + blockVertexArray[block->BlockType][block->RotateCount][vertexIndex].Y;

	*outX = absPosX;
	*outY = absPosY;
}

#include "TetrisBlock.h"

#define MAX_ROTATION_COUNT (4)
#define VERTEX_COUNT (4)

const Point blockVertexArray[ETetrisBlockType::Count][MAX_ROTATION_COUNT][VERTEX_COUNT] = {
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

TetrisBlock MakeRandomBlock(int x, int y)
{
	TetrisBlock block;
	block.pos.X = x;
	block.pos.Y = y;
	block.BlockType = (ETetrisBlockType)(rand() % ETetrisBlockType::Count);
	block.RotateCount = 0;

	return block;
}

void GetBlockAbsPos(const TetrisBlock* block, DWORD vertexIndex, DWORD rotateCount, int* outX, int* outY)
{
	int absPosX = block->pos.X + blockVertexArray[block->BlockType][rotateCount][vertexIndex].X;
	int absPosY = block->pos.Y + blockVertexArray[block->BlockType][rotateCount][vertexIndex].Y;

	*outX = absPosX;
	*outY = absPosY;
}

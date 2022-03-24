#pragma once

#include "Coord.h"
#include "EBlockType.h"


struct Block
{
	Point pos;
	EBlockType BlockType = (EBlockType)0;
	int RotateCount = 0;
};

Block MakeRandomBlock(int x, int y);
void GetBlockAbsCoord(const Block* block, DWORD vertexIndex, int* outX, int* outY);
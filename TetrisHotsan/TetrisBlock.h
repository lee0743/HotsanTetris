#pragma once

#include <Windows.h>

#include "Coord.h"
#include "ETetrisBlockType.h"
#include "RGBA.h"

struct TetrisBlock
{
	Point pos;
	ETetrisBlockType BlockType;
	int RotateCount;
	EColor Color;
};

TetrisBlock MakeRandomBlock(int x, int y);
void GetBlockAbsPos(const TetrisBlock* block, DWORD vertexIndex, DWORD rotateCount, int* outX, int* outY);
#pragma once

#include "Coord.h"
#include "EBlockType.h"


struct Block
{
	Point pos;
	EBlockType BlockType;
	int RotateCount = 0;
};

Block MakeRandomBlock(int x, int y);
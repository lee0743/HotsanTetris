#pragma once

#include "BlockImage.h"
#include "Coord.h"

struct BlockUnit
{
	Rect Coord;
	BlockImage* blockImage;
};

#define MAX_BLOCK_UNIT_COUNT (4)

class Block
{
public:
	Block();
	~Block();

	Point GetPos() const;
	void GetPos(int* outX, int* outY) const;
	void SetPos(Point point);
	void SetPos(int x, int y);
	const BlockUnit* GetBlockUnits() const;
	void AddBlockUnit(Rect Coord, BlockImage* blockImage);
	DWORD GetBlockUnitCount() const;

private:
	Point mPositon = { 0, };
	DWORD mBlockUnitNum = 0;
	BlockUnit mBlockUnits[MAX_BLOCK_UNIT_COUNT];
};


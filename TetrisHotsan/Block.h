#pragma once

#include "BlockImage.h"
#include "Coord.h"

struct BlockUnit
{
	Rect Coord;
	BlockImage* blockImage;
};

class Block
{
public:
	Block();
	~Block();

	Point GetPos() const;
	void GetPos(int* outX, int* outY) const;
	void SetPos(Point point);
	void SetPos(int x, int y);
	BlockImage* GetImage();
	void SetImage(BlockImage* Image);
private:
	Point mPositon = { 0, };
	BlockImage* mImage = nullptr;
};


#pragma once

#include "Coord.h"
#include "EBlockType.h"
#include "Block.h"
#include "BitmapImage.h"
#include "DDraw.h"

class Tetris
{
public:
	Tetris();
	~Tetris();

	void Initialize(HWND hwnd);
	void DrawScene();
	void Update();
	Block* GetBlock();
	double fps = 1;
private:
	enum {
		MAP_WIDTH = 10,
		MAP_HEIGHT = 22,
		MAX_BLOCK_COUNT = 512,
		BLOCK_VERTEX_COUNT = 4
	};

	DDraw* mpDDraw = nullptr;
	BitmapImage* mCell = nullptr;
	BitmapImage* mBackgroundImage = nullptr;

	BOOL blockStopped = false;
	Block mBlock;
	bool mbMap[MAP_HEIGHT][MAP_WIDTH] = { 0, };
	unsigned int mSeed = 0;

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

};

extern Tetris* gpTetris;

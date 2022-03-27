#pragma once

#include <Windows.h>
#include "RGBA.h"

enum class ETetrisMap{
	WIDTH = 10,
	HEIGHT = 22
};

struct TetrisMap
{
	EColor Color;
	BOOL ExistFixedBlock;
};
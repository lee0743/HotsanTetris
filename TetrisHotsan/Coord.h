#pragma once

#include <Windows.h>

struct Point
{
	int X;
	int Y;
};

struct Rect
{
	Point LeftTop;
	Point RightBottom;
};
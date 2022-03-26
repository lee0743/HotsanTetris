#pragma once
#include <assert.h>
#include <Windows.h>

constexpr DWORD MakeRGBA(DWORD r, DWORD g, DWORD b, DWORD a = 0xff) 
{
	assert(0 <= r && r <= 255);
	assert(0 <= g && g <= 255);
	assert(0 <= b && b <= 255);
	assert(0 <= a && a <= 255);

	return (0xff << 24 | r << 16 | g << 8 | b);
}

constexpr DWORD RevRGBA(DWORD bgr)
{
	DWORD b = (bgr & 0x00ff0000) >> 16;
	DWORD g = (bgr & 0x0000ff00) >> 8;
	DWORD r = (bgr & 0x000000ff);

	return (b << 24 | g << 16 | r << 8 | 0xff);
}

enum class EColor
{
	Red = (int)MakeRGBA(0xff, 0x00, 0x00),
	Green = (int)MakeRGBA(0x00, 0xff, 0x00),
	Blue = (int)MakeRGBA(0x00, 0x00, 0xff),
	Purple = (int)MakeRGBA(0x80, 0x00, 0xff),
	Gray = (int)MakeRGBA(0x80, 0x80, 0x80),
	Ibory = (int)MakeRGBA(0xc8, 0xbf, 0xe7)
};
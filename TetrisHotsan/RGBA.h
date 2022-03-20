#pragma once
#include <assert.h>
#include <Windows.h>

constexpr DWORD MakeRGBA(DWORD r, DWORD g, DWORD b, DWORD a = 0xff) 
{
	assert(0 <= r && r <= 255);
	assert(0 <= g && g <= 255);
	assert(0 <= b && b <= 255);
	assert(0 <= a && a <= 255);

	return (0xff << 24 | b << 16 | g << 8 | r); 
}

enum class EColor
{
	Red = MakeRGBA(0xff, 0x00, 0x00),
	Green = MakeRGBA(0x00, 0xff, 0x00),
	Blue = MakeRGBA(0x00, 0x00, 0xff),
	Purple = MakeRGBA(0x80, 0x00, 0xff),
	Gray = MakeRGBA(0x80, 0x80, 0x80),
	Ibory = MakeRGBA(0xc8, 0xbf, 0xe7)
};
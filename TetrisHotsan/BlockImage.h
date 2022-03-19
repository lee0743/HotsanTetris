#pragma once

#include <Windows.h>

struct PixelStream
{
	WORD PixelNum;
	DWORD Pixel;
};

#define MAX_STREAM_NUM (3)

struct CompressedBlockLine
{
	DWORD StreamNum;
	PixelStream pPixelStream[MAX_STREAM_NUM];
};

class BlockImage
{
public:
	BlockImage();
	BlockImage(const BlockImage& other);
	~BlockImage();

	void Create(const char* pSrcBits, DWORD width, DWORD height);
	DWORD GetWidth() const;
	DWORD GetHeight() const;
	const CompressedBlockLine* GetCompressedImage(int y) const;
private:
	DWORD CreatePerLine(PixelStream * pDset, const DWORD* pSrc, DWORD width);

	DWORD mWidth = 0;
	DWORD mHeight = 0;
	CompressedBlockLine* mpCompressedImage = nullptr;
};


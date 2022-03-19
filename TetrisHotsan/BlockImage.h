#pragma once

#include <Windows.h>

struct PixelStream
{
	DWORD XPos;
	WORD PixelNum;
	DWORD Pixel;
};

struct CompressedBlockLine
{
	DWORD StreamNum;

	union
	{
		struct
		{
			DWORD offset;
		};

		struct
		{
			PixelStream* pPixelStream;
		};
	};
};

class BlockImage
{
public:
	BlockImage();
	~BlockImage();

	void Create(const char* pSrcBits, DWORD width, DWORD height, DWORD colorKey);
	DWORD GetWidth() const;
	DWORD GetHeight() const;
private:
	DWORD mWidth = 0;
	DWORD mHeight = 0;
	char* mpCompressedImage = nullptr;
};


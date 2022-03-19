#include <assert.h>
#include "BlockImage.h"

BlockImage::BlockImage()
{
}

BlockImage::~BlockImage()
{
	if (mpCompressedImage != nullptr)
	{
		free(mpCompressedImage);
		mpCompressedImage = nullptr;
	}
}

void BlockImage::Create(const char* pSrcBits, DWORD width, DWORD height, DWORD colorKey)
{
	const DWORD workingMemSize = width * height * sizeof(PixelStream);
	PixelStream* workingMem = (PixelStream*)malloc(workingMemSize);
	memset(workingMem, 0, workingMemSize);

	const DWORD linesMemSize = height * sizeof(CompressedBlockLine);
	CompressedBlockLine* lines = (CompressedBlockLine*)malloc(linesMemSize);
	memset(lines, 0, linesMemSize);

	DWORD usedWorkingMemSize = 0;

	PixelStream* dest = workingMem;
	const DWORD* src = (const DWORD*)pSrcBits;

	for (DWORD i = 0; i < height; ++i)
	{
		DWORD streamCount = 0;
		DWORD x = 0;

		while (x < width)
		{
			while (src[x] == colorKey)
			{
				++x;

				if (x >= width)
				{
					goto EXIT_LOOP;
				}
			}

			DWORD currentPixel = src[x];
			dest[streamCount].XPos = x;
			dest[streamCount].Pixel = currentPixel;

			while (src[x] == currentPixel && x < width)
			{
				dest[streamCount].PixelNum++;
				++x;
			}

			++streamCount;
		}

	EXIT_LOOP:
		DWORD offset = dest - workingMem;

		lines[i].StreamNum = streamCount;
		lines[i].offset = offset;

		usedWorkingMemSize += streamCount * sizeof(PixelStream);
		assert(usedWorkingMemSize <= workingMemSize);

		src += width;
		dest += streamCount;
	}

	mpCompressedImage = (char*)malloc(linesMemSize + usedWorkingMemSize);
	PixelStream* const pixelStreamStart = (PixelStream*)(mpCompressedImage + linesMemSize);

	for (DWORD i = 0; i < height; ++i)
	{
		lines[i].pPixelStream = pixelStreamStart + lines[i].offset;
	}

	memcpy(mpCompressedImage, lines, linesMemSize);
	memcpy(pixelStreamStart, workingMem, usedWorkingMemSize);

	free(workingMem);
	free(lines);

	mWidth = width;
	mHeight = height;
}

DWORD BlockImage::GetWidth() const
{
	return mWidth;
}

DWORD BlockImage::GetHeight() const
{
	return mHeight;
}




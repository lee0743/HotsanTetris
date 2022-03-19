#include "BlockImage.h"

BlockImage::BlockImage()
{
}

BlockImage::BlockImage(const BlockImage& other)
	: mWidth(other.mWidth)
	, mHeight(other.mHeight)
{
	mpCompressedImage = (CompressedBlockLine*)malloc(other.mHeight * sizeof(CompressedBlockLine));
	memcpy(mpCompressedImage, other.mpCompressedImage, other.mHeight * sizeof(CompressedBlockLine) );
}

BlockImage::~BlockImage()
{
	if (mpCompressedImage != nullptr)
	{
		free(mpCompressedImage);
		mpCompressedImage = nullptr;
	}
}

void BlockImage::Create(const char * pSrcBits, DWORD width, DWORD height)
{
	mpCompressedImage = (CompressedBlockLine*)malloc(height * sizeof(CompressedBlockLine));
	
	for (DWORD i = 0; i < height; ++i)
	{
		const DWORD* pSrc = (const DWORD*)(pSrcBits) + (i * width);
		PixelStream* pDest = mpCompressedImage[i].pPixelStream;

		DWORD StreamNumPerLine = CreatePerLine(pDest, pSrc, width);
		mpCompressedImage[i].StreamNum = StreamNumPerLine;
	}

	mWidth = width;
	mHeight = height;
}

DWORD BlockImage::CreatePerLine(PixelStream * pDest, const DWORD* pSrc, DWORD width)
{
	memset(pDest, 0, sizeof(PixelStream) * MAX_STREAM_NUM);

	DWORD streamCount = 0;
	DWORD x = 0;

	while (x < width)
	{
		DWORD pixel = pSrc[x];

		pDest[streamCount].Pixel = pixel;
		pDest[streamCount].PixelNum = 0;

		while (pSrc[x] == pixel && x < width)
		{
			pDest[streamCount].PixelNum++;
			++x;
		}

		++streamCount;
	}

	return streamCount;
}

DWORD BlockImage::GetWidth() const
{
	return mWidth;
}

DWORD BlockImage::GetHeight() const
{
	return mHeight;
}

const CompressedBlockLine* BlockImage::GetCompressedImage(int y) const
{
	return mpCompressedImage + y;
}




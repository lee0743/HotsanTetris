#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif // !_CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include "BitmapImage.h"

BitmapImage::BitmapImage()
{
}

BOOL BitmapImage::Load24BitsBitmap(const char * fileName)
{
	BOOL bResult = FALSE;

	FILE* fstream = nullptr;
	DWORD imageSize = 0;
	unsigned char* temp = nullptr;
	DWORD width;
	DWORD height;
	DWORD line;
	DWORD widthBytes;

	fstream = fopen(fileName, "rb");
	if (nullptr == fstream)
	{
		goto bReturn;
	}

	Destroy();

	BitmapFileHeader fileHeader;
	fread(&fileHeader, BITMAP_FILE_HEADER_SIZE, 1, fstream);

	BitmapInfoHeader infoHeader;
	fread(&infoHeader, BITMAP_INFO_HEADER_SIZE, 1, fstream);

	width = infoHeader.Width;
	height = infoHeader.Height;
	imageSize = width * height * 3;

	temp = (unsigned char*)malloc(imageSize * sizeof(unsigned char));
	fread(temp, imageSize, 1, fstream);

	mpRawImage = new char[width * height * 4];

	line = height;
	widthBytes = width * 4;

	for (DWORD y = 0; y < height; ++y)
	{
		--line;
		for (DWORD x = 0; x < width; ++x)
		{
			mpRawImage[line * widthBytes + x * 4 + 0] = temp[y * width * 3 + x * 3 + 0];
			mpRawImage[line * widthBytes + x * 4 + 1] = temp[y * width * 3 + x * 3 + 1];
			mpRawImage[line * widthBytes + x * 4 + 2] = temp[y * width * 3 + x * 3 + 2];
			mpRawImage[line * widthBytes + x * 4 + 0] = (BYTE)255;
		}
	}

	free(temp);
	temp = nullptr;

	mWidth = width;
	mHeight = height;

	fclose(fstream);

	bResult = TRUE;

bReturn:
	return bResult;
}

DWORD BitmapImage::GetWidth() const
{
	return mWidth;
}

DWORD BitmapImage::GetHeight() const
{
	return mHeight;
}

char * BitmapImage::GetRawImage() const
{
	return mpRawImage;
}

BitmapImage::~BitmapImage()
{
	Destroy();
}

void BitmapImage::Destroy()
{
	if (nullptr != mpRawImage)
	{
		delete[] mpRawImage;
		mpRawImage = nullptr;
	}
}

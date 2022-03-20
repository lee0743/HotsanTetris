#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif // !_CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <assert.h>
#include "BitmapImage.h"

BitmapImage::BitmapImage()
{
}

BOOL BitmapImage::Load24BitsBitmap(const char * fileName)
{
	BOOL bResult = FALSE;

	FILE* fstream = nullptr;
	unsigned char* temp = nullptr;
	DWORD width = 0;
	DWORD height = 0;
	DWORD line = 0;
	DWORD widthBytes = 0;
	DWORD pitch = 0;

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

	// get nearest multiple of 4
	// pitch = (width + 3) >> 2 << 2;
	pitch = ((width * 3) + 3) & ~0x03;
	assert(pitch * height == infoHeader.ImageSize);

	temp = (unsigned char*)malloc(infoHeader.ImageSize * sizeof(unsigned char));
	fread(temp, infoHeader.ImageSize, 1, fstream);

	mpRawImage = new char[width * height * 4];

	line = height;
	widthBytes = width * 4;

	for (DWORD y = 0; y < height; ++y)
	{
		--line;
		for (DWORD x = 0; x < width; ++x)
		{
			mpRawImage[line * widthBytes + x * 4 + 0] = temp[y * pitch + x * 3 + 0];
			mpRawImage[line * widthBytes + x * 4 + 1] = temp[y * pitch + x * 3 + 1];
			mpRawImage[line * widthBytes + x * 4 + 2] = temp[y * pitch + x * 3 + 2];
			mpRawImage[line * widthBytes + x * 4 + 3] = (BYTE)255;
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

DWORD BitmapImage::GetPixel(DWORD x, DWORD y) const
{
	return *((DWORD*)(mpRawImage)+y * mWidth + x);
}

void BitmapImage::SetPixel(DWORD x, DWORD y, DWORD color)
{
	*((DWORD*)(mpRawImage)+y * mWidth + x) = color;
}

char * BitmapImage::GetRawImage() const
{
	return mpRawImage;
}

BitmapImage::~BitmapImage()
{
	Destroy();
}

BitmapImage::BitmapImage(BitmapImage* other)
	: mWidth(other->mWidth)
	, mHeight(other->mHeight)
{
	mpRawImage = new char[other->mWidth * other->mHeight * 4];
	memcpy(mpRawImage, other->GetRawImage(), other->mWidth * other->mHeight * 4);
}

BitmapImage::BitmapImage(DWORD width, DWORD height, char * rawImage)
	: mWidth(width)
	, mHeight(height)
	, mpRawImage(rawImage)
{
}

void BitmapImage::Destroy()
{
	if (nullptr != mpRawImage)
	{
		delete[] mpRawImage;
		mpRawImage = nullptr;
	}
}

BOOL Save24BitsBitmap(const char * fileName, BitmapImage* src)
{
	BOOL bResult = FALSE;

	const DWORD width = src->GetWidth();
	const DWORD height = src->GetHeight();

	const DWORD headerSize = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);
	const DWORD pitch = ((width * 3) + 3) & ~0x03;

	BitmapFileHeader fileHeader;
	fileHeader.Type = (WORD)0x4D42;
	fileHeader.FileSize = pitch * height * 3 + headerSize;
	fileHeader.Reserved1 = (WORD)0;
	fileHeader.Reserved2 = (WORD)0;
	fileHeader.Offset = headerSize;

	BitmapInfoHeader infoHeader;
	infoHeader.Size = sizeof(BitmapInfoHeader);
	infoHeader.Width = width;
	infoHeader.Height = height;
	infoHeader.NumPlanes = (WORD)1;
	infoHeader.BitsPerPixel = (WORD)24;
	infoHeader.Compression = 0;
	infoHeader.ImageSize = pitch * height;
	infoHeader.HorizontalResolution = 0;
	infoHeader.VerticalResolution = 0;
	infoHeader.NumColor = 0;
	infoHeader.NumColorUsed = 0;

	const DWORD bufferSize = infoHeader.ImageSize * sizeof(unsigned char);
	unsigned char* buffer = (unsigned char*)malloc(bufferSize);
	memset(buffer, 0, bufferSize);
	const char* pRawImage = src->GetRawImage();

	DWORD line = height;
	DWORD widthBytes = width * 4;

	for (DWORD y = 0; y < height; ++y)
	{
		--line;
		for (DWORD x = 0; x < width; ++x)
		{
			buffer[y * pitch + x * 3 + 0] = pRawImage[line * widthBytes + x * 4 + 0];
			buffer[y * pitch + x * 3 + 1] = pRawImage[line * widthBytes + x * 4 + 1];
			buffer[y * pitch + x * 3 + 2] = pRawImage[line * widthBytes + x * 4 + 2];
		}
	}

	FILE* fstream = fopen(fileName, "wb");

	if (fstream == nullptr)
	{
		goto RETURN;
	}

	fwrite(&fileHeader, sizeof(BitmapFileHeader), 1, fstream);
	fwrite(&infoHeader, sizeof(BitmapInfoHeader), 1, fstream);
	fwrite(buffer, bufferSize, 1, fstream);

	fclose(fstream);

	free(buffer);
	bResult = TRUE;

RETURN:
	return bResult;
}
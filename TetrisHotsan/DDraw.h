#pragma once

#include <ddraw.h>
#include "BitmapImage.h"

class DDraw
{
public:
	DDraw();
	~DDraw();
	BOOL Initialize(HWND hwnd);
	BOOL LockBackBuffer();
	void UnlockBackBuffer();
	void Blt();
	void Clear();

	BOOL DrawBitmapImage(DWORD xPos, DWORD yPos, BitmapImage* image);
	BOOL DrawBitmapImageWithColorKey(DWORD xPos, DWORD yPos, BitmapImage* image, DWORD colorKey);
	BOOL CalcClipArea(int * x, int * y, int * width, int * height);

	void OnUpdateWindowPos();
private:
	IDirectDraw* mpDD = nullptr;
	IDirectDraw7* mpDD7 = nullptr;
	IDirectDrawSurface7* mpDDPrimary = nullptr;
	IDirectDrawSurface7* mpDDBack = nullptr;
	IDirectDrawClipper* mpClipper = nullptr;

	char* mpLockedBackBuffer = nullptr;
	DWORD mLockedBackBufferPitch = 0;
	DWORD mWidth = 0;
	DWORD mHeight = 0;

	HWND mHWnd;
	RECT mWindow;
};
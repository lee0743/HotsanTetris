#include <assert.h>
#include "DDraw.h"

DDraw::DDraw()
{
}

BOOL DDraw::Initialize(HWND hwnd)
{
	BOOL bResult = FALSE;

	mHWnd = hwnd;

	DDSURFACEDESC2 ddsd = {};
	DWORD width;
	DWORD height;

	if (DD_OK != DirectDrawCreate(nullptr, &mpDD, nullptr))
	{
		MessageBox(mHWnd, L"DirectDrawCreate failed!", nullptr, MB_OK);
		goto bReturn;
	}

	if (DD_OK != mpDD->QueryInterface(IID_IDirectDraw7, (LPVOID*)&mpDD7))
	{
		MessageBox(mHWnd, L"QueryInterface failed!", nullptr, MB_OK);
		goto bReturn;
	}

	if (DD_OK != mpDD7->SetCooperativeLevel(mHWnd, DDSCL_NORMAL))
	{
		MessageBox(mHWnd, L"SetCooperativeLevel failed!", nullptr, MB_OK);
		goto bReturn;
	}

	ddsd.dwSize = sizeof(DDSURFACEDESC2);
	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

	if (DD_OK != mpDD7->CreateSurface(&ddsd, &mpDDPrimary, nullptr))
	{
		MessageBox(mHWnd, L"CreatePrimarySurface failed!", nullptr, MB_OK);
		goto bReturn;
	}

	if (DD_OK != mpDD7->CreateClipper(0, &mpClipper, nullptr))
	{
		MessageBox(mHWnd, L"CreateClipper failed!", nullptr, MB_OK);
		goto bReturn;
	}

	mpClipper->SetHWnd(0, mHWnd);
	mpDDPrimary->SetClipper(mpClipper);

	GetClientRect(mHWnd, &mWindow);
	::ClientToScreen(mHWnd, (POINT*)&mWindow.left);
	::ClientToScreen(mHWnd, (POINT*)&mWindow.right);

	width = mWindow.right - mWindow.left;
	height = mWindow.bottom - mWindow.top;

	ddsd.dwSize = sizeof(DDSURFACEDESC2);
	ddsd.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	ddsd.dwWidth = width;
	ddsd.dwHeight = height;

	if (DD_OK != mpDD7->CreateSurface(&ddsd, &mpDDBack, nullptr))
	{
		MessageBox(mHWnd, L"CreateBackSurface failed!", nullptr, MB_OK);
		assert(false);
		goto bReturn;
	}

	mWidth = width;
	mHeight = height;

	bResult = TRUE;

bReturn:
	return bResult;
}

BOOL DDraw::LockBackBuffer()
{
	BOOL bResult = FALSE;
	
	if (mpDDBack != nullptr)
	{
		DDSURFACEDESC2 ddsd;
		memset(&ddsd, 0, sizeof(DDSURFACEDESC2));
		ddsd.dwSize = sizeof(DDSURFACEDESC2);

		mpDDBack->Lock(nullptr, &ddsd, DDLOCK_WAIT|DDLOCK_WRITEONLY, nullptr);
		mpLockedBackBuffer = (char*)ddsd.lpSurface;
		mLockedBackBufferPitch = ddsd.lPitch;
	
		assert(ddsd.dwWidth == mWidth);
		assert(ddsd.dwHeight == mHeight);
	}

	bResult = TRUE;
	return bResult;
}

void DDraw::UnlockBackBuffer()
{
	if (mpDDBack != nullptr)
	{
		mpDDBack->Unlock(nullptr);
	}
}

void DDraw::Blt()
{
	mpDDPrimary->Blt(&mWindow, mpDDBack, nullptr, DDBLT_WAIT, nullptr);
}

BOOL DDraw::DrawBitmapImage(DWORD x, DWORD y, BitmapImage* image)
{
	assert(mpLockedBackBuffer != nullptr);

	DWORD width = image->GetWidth();
	DWORD height = image->GetHeight();
	char* pBits = image->GetRawImage();
	{
		int startX = max(0, x);
		startX = min(mWidth, x);

		int startY = max(0, y);
		startY = min(mHeight, y);

		int endX = max(x + width, 0);
		endX = min(x + width, mWidth);

		int endY = max(x + height, 0);
		endY = min(x + height, mHeight);

		int newWidth = endX - startX;
		int newHeight = endY - startY;

		if (newWidth <= 0 || newHeight <= 0)
		{
			return FALSE;
		}

		x = startX;
		y = startY;

		width = newWidth;
		height = newHeight;
	}

	char* src = (char*)pBits;
	char* dest = mpLockedBackBuffer + (y * mLockedBackBufferPitch) + x;

	for (DWORD y = 0; y < height; ++y)
	{
		for (DWORD x = 0; x < width; ++x)
		{
			*(DWORD*)dest = *(DWORD*)src;

			dest += 4;
			src += 4;
		}

		src -= (width * 4);
		src += (width * 4);
		dest -= (width * 4);
		dest += mLockedBackBufferPitch;
	}

	return TRUE;
}

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif /* _CRT_SECURE_NO_WARNINGS */
// TetrisHotsan.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#include <stdio.h>
#include <assert.h>

#include "framework.h"
#include "BitmapImage.h"
#include "BlockImage.h"
#include "Block.h"
#include "DDraw.h"
#include "RGBA.h"
#include "TetrisHotsan.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
HWND gHWnd;

// HACK: Will Fix Later
BOOL canUseKeyDown = FALSE;
ULONGLONG start_time;
ULONGLONG end_time;
DWORD interval_time;
DWORD fps = 1;
DWORD y = 0;
DWORD x = 0;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

static void SaveBlocksToAssets();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.

	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_TETRISHOTSAN, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TETRISHOTSAN));

	MSG msg;

	DDraw* pDDraw = new DDraw();
	pDDraw->Initialize(gHWnd);

	SaveBlocksToAssets();

	BitmapImage* backGround = new BitmapImage();
	if (backGround->Load24BitsBitmap("./assets/map2.bmp") == false)
	{
		__debugbreak();
	}

	BitmapImage* block1 = new BitmapImage();
	if (block1->Load24BitsBitmap("./assets/redBlock1.bmp") == false)
	{
		__debugbreak();
	}

	BitmapImage* block2 = new BitmapImage();
	if (block2->Load24BitsBitmap("./assets/blueBlock5.bmp") == false)
	{
		__debugbreak();
	}

	{
		pDDraw->LockBackBuffer();
		{
			pDDraw->DrawBitmapImage(0, 0, backGround);

			DWORD colorKey = MakeRGBA(0xff, 0x00, 0xff);
			pDDraw->DrawBitmapImageWithColorKey(x, y, block1, colorKey);
			pDDraw->DrawBitmapImageWithColorKey(91, y, block2, colorKey);
		}
		pDDraw->UnlockBackBuffer();
		pDDraw->Blt();

		y += 22;
	}
	
	start_time= GetTickCount64();
	canUseKeyDown = TRUE;

	// 기본 메시지 루프입니다:
	while (TRUE)
	{
		end_time = GetTickCount64();
		interval_time = (end_time - start_time) / 1000;

		if (TRUE == PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (interval_time >= fps)
			{
				start_time = GetTickCount64();
				canUseKeyDown = TRUE;

				pDDraw->LockBackBuffer();
				{
					pDDraw->DrawBitmapImage(0, 0, backGround);

					DWORD colorKey = MakeRGBA(0xff, 0x00, 0xff);
					pDDraw->DrawBitmapImageWithColorKey(x, y, block1, colorKey);
					pDDraw->DrawBitmapImageWithColorKey(92, y, block2, colorKey);
				}
				pDDraw->UnlockBackBuffer();
				pDDraw->Blt();

				y += 22;
			}

			if (y >= backGround->GetHeight())
			{
				break;
			}
		}
	}

	delete backGround;
	delete block1;
	delete block2;

	delete pDDraw;
	return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TETRISHOTSAN));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TETRISHOTSAN);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	gHWnd = hWnd;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 메뉴 선택을 구문 분석합니다:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_KEYDOWN:
	{
		if (canUseKeyDown)
		{
			if (wParam == VK_UP)
			{
				y -= 22;
			}
			else if (wParam == VK_DOWN)
			{
				y += 22;
			}
			else if (wParam == VK_RIGHT)
			{
				x += 92;
			}
			else if (wParam == VK_LEFT)
			{
				x -= 92;
			}

			canUseKeyDown = FALSE;
		}
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

static void SaveBlocksToAssets()
{	// WARNING: FUCKING CODE
	BitmapImage* srcImage = new BitmapImage();

	const DWORD numColor = 5;

	const char* blockColorNames[numColor] = { "green", "blue", "purple", "gray", "ibory" };
	DWORD blockColors[numColor] = { (DWORD)EColor::Green, (DWORD)EColor::Blue, (DWORD)EColor::Purple, (DWORD)EColor::Gray, (DWORD)EColor::Ibory};

	char fileName[100];

	BOOL result = srcImage->Load24BitsBitmap("./assets/redBlock.bmp");
	assert(result == TRUE);
	
	BitmapImage* copied = new BitmapImage(srcImage);

	for (DWORD i = 0; i < numColor; ++i)
	{

		for (DWORD y = 0; y < srcImage->GetHeight(); ++y)
		{
			for (DWORD x = 0; x < srcImage->GetWidth(); ++x)
			{
				if (srcImage->GetPixel(x, y) == MakeRGBA(0x24, 0x1c, 0xed))
				{
					copied->SetPixel(x, y, blockColors[i]);
				}
			}
		}

		sprintf(fileName, "./assets/%sBlock.bmp", blockColorNames[i]);

		Save24BitsBitmap(fileName, copied);
	}

	delete copied;

	memset(fileName, 0, 100);

	result = srcImage->Load24BitsBitmap("./assets/redBlock1.bmp");
	assert(result == TRUE);

	BitmapImage* copied2 = new BitmapImage(srcImage);

	for (DWORD i = 0; i < numColor; ++i)
	{

		for (DWORD y = 0; y < srcImage->GetHeight(); ++y)
		{
			for (DWORD x = 0; x < srcImage->GetWidth(); ++x)
			{
				if (srcImage->GetPixel(x, y) == MakeRGBA(0x24, 0x1c, 0xed))
				{
					copied2->SetPixel(x, y, blockColors[i]);
				}
			}
		}

		sprintf(fileName, "./assets/%sBlock1.bmp", blockColorNames[i]);

		Save24BitsBitmap(fileName, copied2);
	}

	delete copied2;

	memset(fileName, 0, 100);

	result = srcImage->Load24BitsBitmap("./assets/redBlock2.bmp");
	assert(result == TRUE);

	BitmapImage* copied3 = new BitmapImage(srcImage);

	for (DWORD i = 0; i < numColor; ++i)
	{

		for (DWORD y = 0; y < srcImage->GetHeight(); ++y)
		{
			for (DWORD x = 0; x < srcImage->GetWidth(); ++x)
			{
				if (srcImage->GetPixel(x, y) == MakeRGBA(0x24, 0x1c, 0xed))
				{
					copied3->SetPixel(x, y, blockColors[i]);
				}
			}
		}

		sprintf(fileName, "./assets/%sBlock2.bmp", blockColorNames[i]);

		Save24BitsBitmap(fileName, copied3);
	}

	delete copied3;

	memset(fileName, 0, 100);

	result = srcImage->Load24BitsBitmap("./assets/redBlock3.bmp");
	assert(result == TRUE);

	BitmapImage* copied4 = new BitmapImage(srcImage);

	for (DWORD i = 0; i < numColor; ++i)
	{

		for (DWORD y = 0; y < srcImage->GetHeight(); ++y)
		{
			for (DWORD x = 0; x < srcImage->GetWidth(); ++x)
			{
				if (srcImage->GetPixel(x, y) == MakeRGBA(0x24, 0x1c, 0xed))
				{
					copied4->SetPixel(x, y, blockColors[i]);
				}
			}
		}

		sprintf(fileName, "./assets/%sBlock3.bmp", blockColorNames[i]);

		Save24BitsBitmap(fileName, copied4);
	}

	delete copied4;

	memset(fileName, 0, 100);

	result = srcImage->Load24BitsBitmap("./assets/redBlock4.bmp");
	assert(result == TRUE);

	BitmapImage* copied5 = new BitmapImage(srcImage);

	for (DWORD i = 0; i < numColor; ++i)
	{

		for (DWORD y = 0; y < srcImage->GetHeight(); ++y)
		{
			for (DWORD x = 0; x < srcImage->GetWidth(); ++x)
			{
				if (srcImage->GetPixel(x, y) == MakeRGBA(0x24, 0x1c, 0xed))
				{
					copied5->SetPixel(x, y, blockColors[i]);
				}
			}
		}

		sprintf(fileName, "./assets/%sBlock4.bmp", blockColorNames[i]);

		Save24BitsBitmap(fileName, copied5);
	}

	delete copied5;

	memset(fileName, 0, 100);

	result = srcImage->Load24BitsBitmap("./assets/redBlock5.bmp");
	assert(result == TRUE);

	BitmapImage* copied6 = new BitmapImage(srcImage);

	for (DWORD i = 0; i < numColor; ++i)
	{

		for (DWORD y = 0; y < srcImage->GetHeight(); ++y)
		{
			for (DWORD x = 0; x < srcImage->GetWidth(); ++x)
			{
				if (srcImage->GetPixel(x, y) == MakeRGBA(0x24, 0x1c, 0xed))
				{
					copied6->SetPixel(x, y, blockColors[i]);
				}
			}
		}

		sprintf(fileName, "./assets/%sBlock5.bmp", blockColorNames[i]);

		Save24BitsBitmap(fileName, copied6);
	}

	delete copied6;
}
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif /* _CRT_SECURE_NO_WARNINGS */
// TetrisHotsan.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#include <stdio.h>
#include <assert.h>
#include <crtdbg.h>

#include "framework.h"
#include "BitmapImage.h"
#include "BlockImage.h"
#include "Block.h"
#include "DDraw.h"
#include "RGBA.h"
#include "TetrisHotsan.h"
#include "Tetris.h"
#include "TickCounter.h"

#define MAX_LOADSTRING (100)
#define MILLISECOND_TO_SECOND (1000)
#define MAX_PATH_LENGTH (128)

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
HWND gHWnd;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

static void SaveBlocksToAssets();
static void SaveBlocks(const char* srcFileName, const char* destFileName);
static BitmapImage* GetRotatedBitmap(BitmapImage* src);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

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

	gpTetris = new Tetris;
	gpTetris->Initialize(gHWnd);

	// 기본 메시지 루프입니다:
	while (TRUE)
	{
		BOOL bHasMessage = PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE);

		if (TRUE == bHasMessage)
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			gpTetris->Process();
		}
	}

	if (nullptr != gpTetris)
	{
		delete gpTetris;
		gpTetris = nullptr;
	}

#ifdef _DEBUG
	_ASSERT(_CrtCheckMemory());
#endif
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
		if (nullptr != gpTetris)
		{
			gpTetris->OnKeyDown(wParam, lParam);
		}
	}
	break;
	case WM_KEYUP:
	{
		if (nullptr != gpTetris)
		{
			gpTetris->OnKeyUp(wParam, lParam);
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
	BitmapImage* src = new BitmapImage();
	{
		src->Load24BitsBitmap("./assets/redBlock1.bmp");

		BitmapImage* rotated90Degrees = GetRotatedBitmap(src);
		Save24BitsBitmap("./assets/redBlock1Rotated1.bmp", rotated90Degrees);

		BitmapImage* rotated180Degrees = GetRotatedBitmap(rotated90Degrees);
		Save24BitsBitmap("./assets/redBlock1Rotated2.bmp", rotated180Degrees);

		BitmapImage* rotated270Degrees = GetRotatedBitmap(rotated180Degrees);
		Save24BitsBitmap("./assets/redBlock1Rotated3.bmp", rotated270Degrees);

		delete rotated90Degrees;
		delete rotated180Degrees;
		delete rotated270Degrees;
	}

	{
		src->Load24BitsBitmap("./assets/redBlock2.bmp");
		BitmapImage* rotated90Degrees = GetRotatedBitmap(src);
		Save24BitsBitmap("./assets/redBlock2Rotated1.bmp", rotated90Degrees);

		BitmapImage* rotated180Degrees = GetRotatedBitmap(rotated90Degrees);
		Save24BitsBitmap("./assets/redBlock2Rotated2.bmp", rotated180Degrees);

		BitmapImage* rotated270Degrees = GetRotatedBitmap(rotated180Degrees);
		Save24BitsBitmap("./assets/redBlock2Rotated3.bmp", rotated270Degrees);

		delete rotated90Degrees;
		delete rotated180Degrees;
		delete rotated270Degrees;
	}

	{
		src->Load24BitsBitmap("./assets/redBlock3.bmp");
		BitmapImage* rotated90Degrees = GetRotatedBitmap(src);
		Save24BitsBitmap("./assets/redBlock3Rotated1.bmp", rotated90Degrees);

		BitmapImage* rotated180Degrees = GetRotatedBitmap(rotated90Degrees);
		Save24BitsBitmap("./assets/redBlock3Rotated2.bmp", rotated180Degrees);

		BitmapImage* rotated270Degrees = GetRotatedBitmap(rotated180Degrees);
		Save24BitsBitmap("./assets/redBlock3Rotated3.bmp", rotated270Degrees);

		delete rotated90Degrees;
		delete rotated180Degrees;
		delete rotated270Degrees;
	}

	{
		src->Load24BitsBitmap("./assets/redBlock4.bmp");
		BitmapImage* rotated90Degrees = GetRotatedBitmap(src);
		Save24BitsBitmap("./assets/redBlock4Rotated1.bmp", rotated90Degrees);

		BitmapImage* rotated180Degrees = GetRotatedBitmap(rotated90Degrees);
		Save24BitsBitmap("./assets/redBlock4Rotated2.bmp", rotated180Degrees);

		BitmapImage* rotated270Degrees = GetRotatedBitmap(rotated180Degrees);
		Save24BitsBitmap("./assets/redBlock4Rotated3.bmp", rotated270Degrees);

		delete rotated90Degrees;
		delete rotated180Degrees;
		delete rotated270Degrees;
	}
	
	{
		src->Load24BitsBitmap("./assets/redBlock5.bmp");
		BitmapImage* rotated90Degrees = GetRotatedBitmap(src);
		Save24BitsBitmap("./assets/redBlock5Rotated1.bmp", rotated90Degrees);

		BitmapImage* rotated180Degrees = GetRotatedBitmap(rotated90Degrees);
		Save24BitsBitmap("./assets/redBlock5Rotated2.bmp", rotated180Degrees);

		BitmapImage* rotated270Degrees = GetRotatedBitmap(rotated180Degrees);
		Save24BitsBitmap("./assets/redBlock5Rotated3.bmp", rotated270Degrees);

		delete rotated90Degrees;
		delete rotated180Degrees;
		delete rotated270Degrees;
	}

	delete src;
	src = nullptr;

	SaveBlocks("redBlock.bmp", "Block.bmp");
	SaveBlocks("redBlock1.bmp", "Block1.bmp");
	SaveBlocks("redBlock2.bmp", "Block2.bmp");
	SaveBlocks("redBlock3.bmp", "Block3.bmp");
	SaveBlocks("redBlock4.bmp", "Block4.bmp");
	SaveBlocks("redBlock5.bmp", "Block5.bmp");

	SaveBlocks("redBlock1Rotated1.bmp", "Block1Rotated1.bmp");
	SaveBlocks("redBlock1Rotated2.bmp", "Block2Rotated2.bmp");
	SaveBlocks("redBlock1Rotated3.bmp", "Block3Rotated3.bmp");

	SaveBlocks("redBlock2Rotated1.bmp", "Block2Rotated1.bmp");
	SaveBlocks("redBlock2Rotated2.bmp", "Block2Rotated2.bmp");
	SaveBlocks("redBlock2Rotated3.bmp", "Block2Rotated3.bmp");

	SaveBlocks("redBlock3Rotated1.bmp", "Block3Rotated1.bmp");
	SaveBlocks("redBlock3Rotated2.bmp", "Block3Rotated2.bmp");
	SaveBlocks("redBlock3Rotated3.bmp", "Block3Rotated3.bmp");

	SaveBlocks("redBlock4Rotated1.bmp", "Block4Rotated1.bmp");
	SaveBlocks("redBlock4Rotated2.bmp", "Block4Rotated2.bmp");
	SaveBlocks("redBlock4Rotated3.bmp", "Block4Rotated3.bmp");

	SaveBlocks("redBlock5Rotated1.bmp", "Block5Rotated1.bmp");
	SaveBlocks("redBlock5Rotated2.bmp", "Block5Rotated2.bmp");
	SaveBlocks("redBlock5Rotated3.bmp", "Block5Rotated3.bmp");
}

static void SaveBlocks(const char* srcFileName, const char* destFileName)
{
	char srcFilePath[MAX_PATH_LENGTH];
	char destFilePath[MAX_PATH_LENGTH];

	sprintf(srcFilePath, "./assets/%s", srcFileName);

	BitmapImage* srcImage = new BitmapImage();
	BOOL result = srcImage->Load24BitsBitmap(srcFilePath);
	assert(result == TRUE);

	const DWORD numColor = 5;
	const char* blockColorNames[numColor] = {
		"green",
		"blue",
		"purple",
		"gray",
		"ibory"
	};
	DWORD blockColors[numColor] = {
		(DWORD)EColor::Green,
		(DWORD)EColor::Blue,
		(DWORD)EColor::Purple,
		(DWORD)EColor::Gray,
		(DWORD)EColor::Ibory 
	};

	BitmapImage* copied = new BitmapImage(srcImage);

	for (DWORD i = 0; i < numColor; ++i)
	{
		for (DWORD y = 0; y < srcImage->GetHeight(); ++y)
		{
			for (DWORD x = 0; x < srcImage->GetWidth(); ++x)
			{
				if (srcImage->GetPixel(x, y) == MakeRGBA(0xed, 0x1c, 0x24))
				{
					copied->SetPixel(x, y, blockColors[i]);
				}
			}
		}

		sprintf(destFilePath, "./assets/%s%s", blockColorNames[i], destFileName);
		Save24BitsBitmap(destFilePath, copied);

		memset(destFilePath, 0, 128);
	}

	delete srcImage;
	delete copied;
}

BitmapImage * GetRotatedBitmap(BitmapImage* src)
{
	int width = src->GetHeight(); // indented
	int height = src->GetWidth();

	const DWORD* srcImage = (DWORD*)src->GetRawImage();
	DWORD* destImage = (DWORD*)malloc(width * height * 4);

	for (DWORD y = 0; y < height; ++y)
	{
		for (DWORD x = 0; x < width; ++x)
		{
			destImage[y * width + x] = srcImage[(width - 1 - x) * src->GetWidth() + y];
		}
	}

	BitmapImage* dest = new BitmapImage(width, height, (char*)destImage);
	return dest;
}

#include <windows.h>
#include <tchar.h>

#include "LoadTime.h"
#include "RunTime.h"

// !!! MAGIC !!!

#pragma comment(lib, "LoadTime.lib")

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// Common Data

const WORD IDC_LOADLIB = 1001;
HINSTANCE hInstance = NULL;
HWND hButton = NULL;
HMODULE hRunTime = NULL;
RUNTIMEFUNCPTR RunTime_Function = NULL;

// Window Procedure

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uiMsg)
	{
	case WM_CREATE:
		hButton = CreateWindow(
			TEXT("BUTTON"), TEXT("Load Library"),
			WS_VISIBLE | WS_CHILD,
			16, 16 + 48, 240, 30,
			hWnd, (HMENU)IDC_LOADLIB, hInstance, NULL);
		break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps = { 0 };
		HDC hdc = BeginPaint(hWnd, &ps);

		int iTextLen = 0;
		TCHAR szText[1024] = { 0 };

		iTextLen = _stprintf_s(szText, TEXT("Load-Time Result: %s"), LoadTime_Function(3.1415f));
		TextOut(hdc, 16, 16, szText, iTextLen);
		iTextLen = _stprintf_s(szText, TEXT("Run-Time Result: %s"), RunTime_Function ? RunTime_Function(3.1415f) : TEXT("Not loaded!"));
		TextOut(hdc, 16, 16 + 24, szText, iTextLen);

		EndPaint(hWnd, &ps);
	}
	return 0;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_LOADLIB && HIWORD(wParam) == BN_CLICKED)
		{
			if (hRunTime)
			{
				RunTime_Function = NULL;
				FreeLibrary(hRunTime);
				hRunTime = NULL;
			}
			else
			{
				hRunTime = LoadLibrary(TEXT("RunTime.dll"));
				if (hRunTime)
					RunTime_Function = (RUNTIMEFUNCPTR)GetProcAddress(hRunTime, "RunTime_Function");
			}
			SetWindowText(hButton, hRunTime ? TEXT("Free Library") : TEXT("Load Library"));
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, uiMsg, wParam, lParam);
}

// Entry Point

int CALLBACK WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
	hInstance = hInst;

	WNDCLASSEX wcx = { 0 };
	wcx.cbSize = sizeof(WNDCLASSEX);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = WindowProc;
	wcx.hInstance = hInstance;
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcx.lpszClassName = TEXT("SomeClassName");
	if (RegisterClassEx(&wcx) == 0)
		return 1;

	HWND hWnd = CreateWindowEx(0,
		TEXT("SomeClassName"),
		TEXT("DLL Sample"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0,
		CW_USEDEFAULT, 0,
		NULL, NULL, hInstance, 0);
	if (NULL == hWnd)
		return 2;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
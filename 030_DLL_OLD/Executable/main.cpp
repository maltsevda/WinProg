#include <windows.h>
#include "resource.h"
#include "ImplicitDll.h"

HINSTANCE hInstance = NULL;
HMODULE hLazyDLL = NULL;
LPCTSTR (*GetSecondaryString)();

//
// WindowProc - procedure for main window
//

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uiMsg)
	{
	case WM_PAINT:
	{
		LPCTSTR szText = GetMainString(3.1415f);
		LPCTSTR szSecondaryText = GetSecondaryString ? GetSecondaryString() : TEXT("Library is not loaded.");

		RECT rc = { 0 };
		GetClientRect(hWnd, &rc);

		PAINTSTRUCT ps = { 0 };
		HDC hdc = BeginPaint(hWnd, &ps);
		DrawText(hdc, szText, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		rc.top += 48;
		DrawText(hdc, szSecondaryText, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		EndPaint(hWnd, &ps);
	}
	return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_LOADLIBRARY:
			if (!hLazyDLL)
			{
				hLazyDLL = LoadLibrary(TEXT("LazyDll.dll"));
				if (hLazyDLL)
					(FARPROC &)GetSecondaryString = GetProcAddress(hLazyDLL, "GetSecondaryString");
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		case IDM_FREELIBRARY:
			if (hLazyDLL)
			{
				GetSecondaryString = NULL;
				FreeLibrary(hLazyDLL);
				hLazyDLL = NULL;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_ABOUT:
			break;
		}
		break;
	}

	return DefWindowProc(hWnd, uiMsg, wParam, lParam);
}

//
// WinMain - entry point
//

int CALLBACK WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
	hInstance = hInst;

	WNDCLASSEX wcx = { 0 };
	wcx.cbSize = sizeof(WNDCLASSEX);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = WindowProc;
	wcx.hInstance = hInstance;
	wcx.hIcon = LoadIcon(NULL, IDI_INFORMATION);
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcx.lpszMenuName = MAKEINTRESOURCE(IDM_MAINMENU);
	wcx.lpszClassName = TEXT("SomeClassName");
	if (RegisterClassEx(&wcx) == 0)
		return 1;

	HWND hWnd = CreateWindowEx(0,
		TEXT("SomeClassName"),
		TEXT("WinProg: Dynamic Link Libraries."),
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

	if (hLazyDLL)
		FreeLibrary(hLazyDLL);

	return 0;
}
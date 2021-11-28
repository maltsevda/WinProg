#include <windows.h>
#include <stdio.h>		// sprintf_s (_MBCS)
#include <tchar.h>

#define IDT_INVALIDATETIMER 0x0001

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uiMsg)
	{
		case WM_PAINT:
			{
				static unsigned int nFrameNumber = 0;
				TCHAR szText[256] = { 0 };
				_stprintf_s(szText, 256, TEXT("Frame: %u"), ++nFrameNumber);

				RECT rc = { 0 };
				GetClientRect(hWnd, &rc);

				PAINTSTRUCT ps = { 0 };
				HDC hdc = BeginPaint(hWnd, &ps);

				// -- !!! ERORR !!! ---
				for (int i = 0; i < 100; ++i)
					CreateSolidBrush(WHITE_BRUSH);
				// -- !!! ERROR !!! ---

				LOGFONT lf = { 0 };
				HFONT hDefFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
				GetObject(hDefFont, sizeof(LOGFONT), &lf);
				lf.lfHeight = 48;

				HFONT hBigFont = CreateFontIndirect(&lf);
				HFONT hOldFont = (HFONT)SelectObject(hdc, hBigFont);

				DrawText(hdc, szText, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				SelectObject(hdc, hOldFont);
				DeleteObject(hBigFont);

				EndPaint(hWnd, &ps);
			}
			break;

		case WM_TIMER:
			if (wParam == IDT_INVALIDATETIMER)
				InvalidateRect(hWnd, 0, FALSE);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hWnd, uiMsg, wParam, lParam);
	}

	return 0;
}

int CALLBACK _tWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR szCmdLine, int nCmdShow)
{
	WNDCLASSEX wcx = { 0 };
	wcx.cbSize = sizeof(WNDCLASSEX);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = WindowProc;
	wcx.hInstance = hInst;
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcx.lpszClassName = TEXT("SomeClassName");
	if (RegisterClassEx(&wcx) == 0)
		return 1;

	HWND hWnd = CreateWindowEx(0,
					TEXT("SomeClassName"),
					TEXT("Error in GDI"),
					WS_OVERLAPPEDWINDOW,
					CW_USEDEFAULT, 0,
					320, 240,
					NULL, NULL, hInst, 0);
	if (NULL == hWnd)
		return 2;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	SetTimer(hWnd, IDT_INVALIDATETIMER, 1000 / 60, NULL);

	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
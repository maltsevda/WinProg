#include <windows.h>
#include <tchar.h>
#include <stdbool.h>	// bool from C++
#include "resource.h"

// project properties

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// common data

HINSTANCE hInstance = NULL;
TCHAR szUserText[1024] = TEXT("Hello world!");
int iColorButton = IDC_BLACK;
HWND hTextDlg = NULL;

// text modeless dialog procedure

void UpdateTextDialog()
{
	if (hTextDlg)
		SetDlgItemText(hTextDlg, IDC_TEXT, szUserText);
}

INT_PTR CALLBACK TextProc(HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uiMsg)
	{
	case WM_INITDIALOG:
		SetDlgItemText(hDlg, IDC_TEXT, szUserText);
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_TEXT:
			if (HIWORD(wParam) == EN_CHANGE)
			{
				GetDlgItemText(hDlg, IDC_TEXT, szUserText, 1024);
				InvalidateRect(GetParent(hDlg), 0, TRUE);
				return TRUE;
			}
			break;
		case IDCANCEL:
			DestroyWindow(hDlg);
			hTextDlg = NULL;
			return TRUE;
		}
		break;
	}
	return FALSE;
}


// options dialog procedure

INT_PTR CALLBACK OptionsProc(HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uiMsg)
	{
	case WM_INITDIALOG:
		SetDlgItemText(hDlg, IDC_TEXT, szUserText);
		CheckRadioButton(hDlg, IDC_RED, IDC_BLACK, iColorButton);
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			GetDlgItemText(hDlg, IDC_TEXT, szUserText, 1024);
			if (BST_CHECKED == IsDlgButtonChecked(hDlg, IDC_RED)) iColorButton = IDC_RED;
			if (BST_CHECKED == IsDlgButtonChecked(hDlg, IDC_GREEN)) iColorButton = IDC_GREEN;
			if (BST_CHECKED == IsDlgButtonChecked(hDlg, IDC_BLACK)) iColorButton = IDC_BLACK;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			return TRUE;

		case IDC_APPLY:
			GetDlgItemText(hDlg, IDC_TEXT, szUserText, 1024);
			if (BST_CHECKED == IsDlgButtonChecked(hDlg, IDC_RED)) iColorButton = IDC_RED;
			if (BST_CHECKED == IsDlgButtonChecked(hDlg, IDC_GREEN)) iColorButton = IDC_GREEN;
			if (BST_CHECKED == IsDlgButtonChecked(hDlg, IDC_BLACK)) iColorButton = IDC_BLACK;
			InvalidateRect(GetParent(hDlg), 0, TRUE);
			UpdateTextDialog();
			return TRUE;
		}
		break;
	}
	return FALSE;
}

// main dialog procedure

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uiMsg)
	{
	case WM_PAINT:
	{
		RECT rc = { 0 };
		GetClientRect(hWnd, &rc);

		PAINTSTRUCT ps = { 0 };
		HDC hdc = BeginPaint(hWnd, &ps);

		static int nFrame = 0;
		TCHAR szFrame[64] = { 0 };
		int nFrameLen = _stprintf_s(szFrame, 64, TEXT("%i"), nFrame);
		TextOut(hdc, 5, 5, szFrame, nFrameLen);
		nFrame++;

		COLORREF clrOld = GetTextColor(hdc);
		switch (iColorButton)
		{
		case IDC_RED: SetTextColor(hdc, RGB(255, 0, 0)); break;
		case IDC_GREEN: SetTextColor(hdc, RGB(0, 128, 0)); break;
		case IDC_BLACK: SetTextColor(hdc, RGB(0, 0, 0)); break;
		}
		DrawText(hdc, szUserText, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		SetTextColor(hdc, clrOld);

		EndPaint(hWnd, &ps);
	}
	return 0;

	case WM_COMMAND:
		// main menu
		switch (LOWORD(wParam))
		{
		case ID_FILE_OPEN:
		{
			TCHAR szFileName[_MAX_PATH] = { 0 };
			static OPENFILENAME ofn = { 0 };
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFile = szFileName;
			ofn.nMaxFile = _MAX_PATH;
			ofn.lpstrFilter = TEXT("All Files (*.*)\0*.*\0Source Files (*.cpp)\0*.cpp\0");
			ofn.nFilterIndex = 1;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			if (GetOpenFileName(&ofn))
			{
				_tcscpy_s(szUserText, _countof(szUserText), szFileName);
				InvalidateRect(hWnd, 0, TRUE);
				UpdateTextDialog();
			}
			break;
		}
		case ID_FILE_TEXT:
			if (hTextDlg)
			{
				DestroyWindow(hTextDlg);
				hTextDlg = NULL;
			}
			else
				hTextDlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_TEXT), hWnd, TextProc);
			break;
		case ID_FILE_OPTIONS:
			DialogBox(hInstance, MAKEINTRESOURCE(IDD_OPTIONS), hWnd, OptionsProc);
			InvalidateRect(hWnd, 0, TRUE);
			UpdateTextDialog();
			break;
		case ID_FILE_EXIT:
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, uiMsg, wParam, lParam);
}

// entry point

int CALLBACK _tWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR szCmdLine, int nCmdShow)
{
	hInstance = hInst;

	if (_tcslen(szCmdLine) > 0)
		_tcscpy_s(szUserText, _countof(szUserText), szCmdLine);

	WNDCLASSEX wcx = { 0 };
	wcx.cbSize = sizeof(WNDCLASSEX);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = WindowProc;
	wcx.hInstance = hInstance;
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcx.lpszClassName = TEXT("SomeClassName");
	wcx.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
	if (RegisterClassEx(&wcx) == 0)
		return 1;

	HWND hWnd = CreateWindowEx(0,
		TEXT("SomeClassName"),
		TEXT("Hello world!"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0,
		CW_USEDEFAULT, 0,
		NULL, NULL, hInstance, 0);
	if (NULL == hWnd)
		return 2;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	BOOL bRet = 0;
	MSG msg = { 0 };
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (bRet == -1)
			return 3;

		bool isDlgMessage = IsWindow(hTextDlg) && IsDialogMessage(hTextDlg, &msg);
		if (!isDlgMessage)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return 0;
}

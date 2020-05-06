#include <windows.h>
#include <tchar.h>
#include "resource.h"

// Magic

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// Common data

UINT uiHookMessage = WM_NULL;

HMODULE hHookDLL = NULL;
UINT(CALLBACK* GetInjectionsCount)() = NULL;
BOOL(CALLBACK* SetKeyboardHook)(HWND hParentWnd) = NULL;
VOID(CALLBACK* UnhookKeyboardHook)() = NULL;

// Entry point and window procedure

INT_PTR CALLBACK MainDlgProc(HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uiMsg)
	{
	case WM_INITDIALOG:
		uiHookMessage = RegisterWindowMessage(TEXT("HookSampleMessage"));
		return TRUE;

	case WM_COMMAND:
		if (IDCANCEL == LOWORD(wParam) && !HIWORD(wParam))
		{
			// Unhook
			if (UnhookKeyboardHook)
				UnhookKeyboardHook();
			// Then exit
			EndDialog(hDlg, 0);
			return TRUE;
		}
		if (IDC_SETHOOK == LOWORD(wParam) && !HIWORD(wParam))
		{
			if (hHookDLL)
			{
				// Unhook
				if (UnhookKeyboardHook)
					UnhookKeyboardHook();
				// Free Library
				GetInjectionsCount = NULL;
				SetKeyboardHook = NULL;
				UnhookKeyboardHook = NULL;
				FreeLibrary(hHookDLL);
				hHookDLL = NULL;
			}
			else
			{
				// Load Library
				hHookDLL = LoadLibrary(TEXT("Hook.dll"));
				if (hHookDLL)
				{
					(FARPROC&)GetInjectionsCount = GetProcAddress(hHookDLL, "GetInjectionsCount");
					(FARPROC&)SetKeyboardHook = GetProcAddress(hHookDLL, "SetKeyboardHook");
					(FARPROC&)UnhookKeyboardHook = GetProcAddress(hHookDLL, "UnhookKeyboardHook");
				}
				// Set Hook
				if (SetKeyboardHook)
					SetKeyboardHook(hDlg);
			}
			SetDlgItemText(hDlg, IDC_SETHOOK, hHookDLL ? TEXT("Unhooh Hook") : TEXT("Set Hook"));
			PostMessage(hDlg, uiHookMessage, 0, 0);
			return TRUE;
		}
		break;
	}

	if (uiMsg == uiHookMessage)
	{
		// Injections
		SetDlgItemInt(hDlg, IDC_INJECTIONS, GetInjectionsCount ? GetInjectionsCount() : 0, FALSE);
		// Virtual Key
		TCHAR szCode[64] = { 0 };
		_stprintf_s(szCode, TEXT("0x%02IX"), wParam);
		SetDlgItemText(hDlg, IDC_VKCODE, szCode);
		return TRUE;
	}

	return FALSE;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
{
	return (int)DialogBox(hInst, MAKEINTRESOURCE(IDD_MAIN), NULL, MainDlgProc);
}
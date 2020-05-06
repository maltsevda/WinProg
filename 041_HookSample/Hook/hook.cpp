#include <windows.h>

#pragma data_seg(".DENISEG")
UINT nInjectionsCount = 0;
#pragma data_seg()

// this section is exported in a def file, but you can also specify it here.
// #pragma comment(linker, "/section:.DENISEG,RWS")

HINSTANCE hInstance = NULL;
HWND hLauncherWnd = NULL;
UINT uiHookMessage = WM_NULL;
HHOOK hKeyboardHook = NULL;

UINT CALLBACK GetInjectionsCount()
{
	return nInjectionsCount;
}

LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
	if (code == HC_ACTION && hLauncherWnd)
	{
		// if first parameter is HWND_BROADCAST, the message
		// is sent to all top-level windows in the system
		PostMessage(hLauncherWnd, uiHookMessage, wParam, lParam);
	}
	return CallNextHookEx(hKeyboardHook, code, wParam, lParam);
}

BOOL CALLBACK SetKeyboardHook(HWND hParentWnd)
{
	hLauncherWnd = hParentWnd;
	if (!hKeyboardHook)
		hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)KeyboardProc, hInstance, 0);
	return hKeyboardHook ? TRUE : FALSE;
}

VOID CALLBACK UnhookKeyboardHook()
{
	if (hKeyboardHook)
		UnhookWindowsHookEx(hKeyboardHook);
	hKeyboardHook = NULL;
	hLauncherWnd = NULL;
}

BOOL WINAPI DllMain(HINSTANCE hDLL, DWORD dwReason, LPVOID lpvReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		hInstance = hDLL;
		uiHookMessage = RegisterWindowMessage(TEXT("HookSampleMessage"));
		InterlockedIncrement(&nInjectionsCount);
		break;
	case DLL_PROCESS_DETACH:
		InterlockedDecrement(&nInjectionsCount);
		break;
	}
	return TRUE;
}

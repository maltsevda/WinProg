#include <windows.h>
#include "resource.h"

extern HINSTANCE hInstance;
extern TCHAR szText[256];
extern int iBrushColor;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK OptionsProc(HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam);

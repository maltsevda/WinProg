#include "main.h"

HINSTANCE hInstance = NULL;

int CALLBACK WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
	hInstance = hInst;
	
	WNDCLASSEX wcx = { 0 };
	wcx.cbSize = sizeof(WNDCLASSEX);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = WindowProc;
	wcx.hInstance = hInstance;
	wcx.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAIN));
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcx.lpszMenuName = MAKEINTRESOURCE(IDM_MAINMENU);
	wcx.lpszClassName = TEXT("SomeClassName");
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

	// --- START  -- System Menu ---
	HMENU hSysMenu = GetSystemMenu(hWnd, FALSE);
	int iSysMenuCount = GetMenuItemCount(hSysMenu);
	MENUITEMINFO mii = { sizeof(MENUITEMINFO) };
	// separator
	mii.fMask = MIIM_TYPE;
	mii.fType = MFT_SEPARATOR;
	InsertMenuItem(hSysMenu, iSysMenuCount, TRUE, &mii);
	// about 
	LPTSTR szAbout = TEXT("About");
	mii.fMask = MIIM_TYPE | MIIM_ID;
	mii.fType = MFT_STRING;
	mii.dwTypeData = szAbout;
	mii.cch = lstrlen(szAbout);
	mii.wID = IDM_ABOUT;
	InsertMenuItem(hSysMenu, iSysMenuCount + 1, TRUE, &mii);
	// --- FINISH -- System Menu ---

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
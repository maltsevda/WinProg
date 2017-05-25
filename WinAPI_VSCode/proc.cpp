#include "main.h"

LPCTSTR szAboutText = 
	TEXT("\tЛабораторная работа №1\n")
	TEXT("\tВариант №1\n\n")
	TEXT("\tВыполнил: Фамилия Имя Отчество\n")
	TEXT("\tГруппа: ИС31\n\n")
	TEXT("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin sed neque aliquet, dignissim diam sed, sollicitudin sapien. Sed ultricies luctus rhoncus. Cras consectetur blandit massa, sed imperdiet leo auctor vitae. Morbi sem felis, placerat vitae augue ac, mattis dapibus dui. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Nulla tincidunt mattis nulla. Maecenas quis ex eget tortor sollicitudin aliquet eget nec erat. Ut luctus erat nisl, id suscipit ante blandit gravida. Aliquam pellentesque rutrum mollis.");

VOID ShowAboutDialog(HWND hParentWnd)
{
	MessageBox(hParentWnd, szAboutText, TEXT("About"), MB_OK | MB_ICONINFORMATION);
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uiMsg)
	{
		case WM_ERASEBKGND:
			return 1;
		case WM_PAINT:
			{
				RECT rc = { 0 };
				GetClientRect(hWnd, &rc);

				PAINTSTRUCT ps = { 0 };
				HDC hdc = BeginPaint(hWnd, &ps);
				HDC hMemDC = CreateCompatibleDC(hdc);
				HBITMAP hMemBMP = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
				HGDIOBJ hOldBMP = SelectObject(hMemDC, hMemBMP);
				
				HBRUSH hFillBrush = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
				FillRect(hMemDC, &rc, hFillBrush);
				DeleteObject(hFillBrush);

				int iOldBkMode = SetBkMode(hMemDC, TRANSPARENT);
				DrawText(hMemDC, TEXT("Привет мир!"), -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				SetBkMode(hMemDC, iOldBkMode);

				BitBlt(hdc, 0, 0, rc.right, rc.bottom, hMemDC, 0, 0, SRCCOPY);
				SelectObject(hMemDC, hOldBMP);
				DeleteObject(hMemBMP);
				DeleteDC(hMemDC);
				EndPaint(hWnd, &ps);
			}
			return 0;
			
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		case WM_COMMAND:
			switch ( LOWORD(wParam) )
			{
				case IDM_EXIT: 
					DestroyWindow(hWnd); 
					break;
				case IDM_ABOUT: 
					ShowAboutDialog(hWnd); 
					break;
			}
			break;
			
		case WM_SYSCOMMAND:
			if (wParam == IDM_ABOUT)
			{
				ShowAboutDialog(hWnd);
				return 0;
			}
			break;
	}
	
	return DefWindowProc(hWnd, uiMsg, wParam, lParam);
}

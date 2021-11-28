// ��� ����������� � MS Visual C++ 6.0
// ���������� - Intel C++ Compiler 9.1
// �������� ��� Win2K, WinXP, Vista (from time to time)

// Copyright (c) 2006 ���� :)
// �� ��������� ����������� ���� - ����� ������ �������� :)

#include <windows.h>	// ��������� �������� ������� � ���� �� Windows API

// ��� TCHAR � ������ TEXT � ����������� �� ���� ���������� �������� ������������ ������ ��� ������� ASCII
const TCHAR* szClassName = TEXT("ClassName");			// ��� ������ ������
const TCHAR* szWindowTitle = TEXT("Hello World!");		// ��������� � ��� ������ ������
RECT rect = {0};										// ������ ���������� �������

// ������� ������� ��������� ��������� ( ������ ������� ? :) )
LRESULT APIENTRY WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg) {
		case WM_PAINT:	// ���������� ��� ����������� ������
			{	// �.�. � ����� ������ ����������, �� ����� ��� ��������
				// �������� HDC � ����������� WM_PAINT ����� _������_ ��� ������ BeginPaint
				// ���������� ����� �� ������� ��� ������� � MSDN Library
				PAINTSTRUCT ps;									// ���� ����� �������� ���. ����������, �����������
																// ��� ��������� ����������� ����
				HDC hdc = BeginPaint(hWnd, &ps);				// �������� HDC - ����� �� "����������� ��������"
				// ������� �� ������ ���� �����: Hello world!
				DrawText(hdc, szWindowTitle, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				EndPaint(hWnd, &ps);							// ��������� �����������
			}
			break;
		case WM_SIZE:	// ���������� ��� ��������� �������� ����
			// ������������ ��� 2 ������� ���� ��� ���������� �� VC60
			rect.right = LOWORD(lParam);	// ���������� � rect ����� ������ ������
			rect.bottom = HIWORD(lParam);	// ���������� � rect ����� ������ ������
			break;
		case WM_DESTROY:	// ���� ����� �� �������, �� ��� ���� ������ ������� ������� ���� ����� ���������������
			PostQuitMessage(0);	// ������� ������ ������ :) ���� �� �� ��� ����
			break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

// ����� ����� � ���������
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR szCmdLine, int nShowCmd)
{
	// 1� ����
	// �������������� �����
	WNDCLASS wc = {0};						// � �� ������� ��� WNDCLASSEX ����� �� �����, �� MSDN ��������
	wc.style = CS_HREDRAW | CS_VREDRAW;		// ������� ���� ���������������� ��� ��������� �������� ����
    wc.lpfnWndProc = &WndProc;				// ��� ����� ������� :)
    wc.hInstance = hInst;					// ��� ����� ����� �������
    wc.hCursor = LoadCursor(0, IDC_ARROW);	// ������� � ������� � ���� �������
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // GetStockObject ���������� ����� �� ����� ��������, ��� ���� ����
    wc.lpszClassName = szClassName;		// ��� ����� ����� ����� �������
	if ( !RegisterClass(&wc) ) return(0);	// ������������ ( �� ���������� - ������ �� ��������� )

	// 2� ����
	// ��������� ����
	HWND hWnd = CreateWindow(
		szClassName,				// ��� ������
		szWindowTitle,				// ���������
		WS_OVERLAPPEDWINDOW,		// ��� ������
		CW_USEDEFAULT, 0,			// ���������
		CW_USEDEFAULT, 0,			// ������
		0, 0, hInst, 0);			// [censored]
	if ( !hWnd ) return(0);

	// ������ ���������� ������ ( nShowCmd - ��� ��� ��������? ����������������, ������� ��� ... )
	ShowWindow(hWnd, nShowCmd);
	// � ����� ��� �����? ��� �� ����, �� ��� ��� ������. �� �������� ��������� ���������
    UpdateWindow(hWnd);

	// 3� ����
	// ������� ���� ��������� ���������
	MSG msg = {0};					// �������, ������� ����� ������������
	while( GetMessage(&msg, 0, 0, 0) ) {	// ���� ���������
		TranslateMessage(&msg);		// ����������� ����������� ������� � ASCII-��� � �������� ��������� WM_CHAR (��� �� �����)
		DispatchMessage(&msg);		// �������� ��������� ��� ��������� � "������� ������� ��������� ���������"
	}

	return( (int)msg.wParam );		// �.�. ��� �������, �� ������ �������� WM_QUIT ��������� (��. PostQuitMessage)
}
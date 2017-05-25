#include <windows.h>
#include <tchar.h>
#include <time.h>

HWND hMainWnd = NULL;
HANDLE hMutex = NULL;
HANDLE hSemaphore = NULL;
HANDLE phThreads[2] = { 0 };
BOOL bStopTherads = FALSE;
int arrQueue[65536] = { 0 };
int nQueueSize = 0;
TCHAR szText[256] = { 0 };

//
// Helper's functions
//

void AddQueueItem(int iNumber)
{
	WaitForSingleObject(hMutex, INFINITE);
	arrQueue[nQueueSize++] = iNumber;
	ReleaseMutex(hMutex);
	ReleaseSemaphore(hSemaphore, 1, NULL);
}

BOOL GetQueueItem(int* pNumber, DWORD dwMilliseconds)
{
	DWORD dwResult = WaitForSingleObject(hSemaphore, dwMilliseconds);
	if (dwResult == WAIT_OBJECT_0)
	{
		WaitForSingleObject(hMutex, INFINITE);
		*pNumber = arrQueue[0];
		for (int i = 1; i < nQueueSize; ++i)
			arrQueue[i - 1] = arrQueue[i];
		nQueueSize--;
		ReleaseMutex(hMutex);
		return TRUE;
	}
	return FALSE;
}

//
// Thread procedures
//

DWORD WINAPI GenThreadProc(LPVOID lpParameter)
{
	while (!bStopTherads)
	{
		AddQueueItem(rand() % 100);
		Sleep(250 + rand() % 500);
	}
	return 0;
}

DWORD WINAPI ReadThreadProc(LPVOID lpParameter)
{
	while (!bStopTherads)
	{
		int iNumber = 0;
		if (GetQueueItem(&iNumber, 500))
		{
			_stprintf_s(szText, TEXT("Number: %i, Count: %i"), iNumber, nQueueSize);
			InvalidateRect(hMainWnd, 0, TRUE);
			Sleep(500);
		}
		else
		{
			_stprintf_s(szText, TEXT("Number is not Exists"));
			InvalidateRect(hMainWnd, 0, TRUE);
		}
	}
	return 0;
}

//
// WindowProc - procedure for main window
//

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uiMsg)
	{
	case WM_CREATE:
		hMutex = CreateMutex(NULL, FALSE, NULL);
		hSemaphore = CreateSemaphore(0, 0, LONG_MAX, 0);
		phThreads[0] = CreateThread(NULL, 0, GenThreadProc, NULL, 0, NULL);
		phThreads[1] = CreateThread(NULL, 0, ReadThreadProc, NULL, 0, NULL);
		break;

	case WM_DESTROY:
		bStopTherads = TRUE;
		WaitForMultipleObjects(2, phThreads, TRUE, INFINITE);
		CloseHandle(hSemaphore);
		CloseHandle(hMutex);
		PostQuitMessage(0);
		break;

	case WM_PAINT:
	{
		RECT rc = { 0 };
		GetClientRect(hWnd, &rc);

		PAINTSTRUCT ps = { 0 };
		HDC hdc = BeginPaint(hWnd, &ps);
		DrawText(hdc, szText, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		EndPaint(hWnd, &ps);
		return 0;
	} // WM_PAINT
	}

	return DefWindowProc(hWnd, uiMsg, wParam, lParam);
}

//
// WinMain - entry point
//

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	srand((unsigned int)time(0));

	WNDCLASSEX wcx = { 0 };
	wcx.cbSize = sizeof(WNDCLASSEX);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = WindowProc;
	wcx.hInstance = hInstance;
	wcx.hIcon = LoadIcon(NULL, IDI_INFORMATION);
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcx.lpszClassName = TEXT("SomeClassName");
	if (RegisterClassEx(&wcx) == 0)
		return 1;

	hMainWnd = CreateWindowEx(0,
		TEXT("SomeClassName"),
		TEXT("ProgWin: Threads testing"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0,
		CW_USEDEFAULT, 0,
		NULL, NULL, hInstance, 0);

	if (NULL == hMainWnd)
		return 2;
	ShowWindow(hMainWnd, nCmdShow);
	UpdateWindow(hMainWnd);

	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
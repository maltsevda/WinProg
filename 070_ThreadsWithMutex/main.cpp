#include <windows.h>
#include <stdio.h>		// sprintf_s (_MBCS)
#include <tchar.h>
#include <time.h>
#include "resource.h"

HWND hMainWnd = NULL;
HANDLE hMutexSort = NULL;
HANDLE hMutexCheck = NULL;

int* pArray = NULL;
const int nArraySize = 65536;
LONG nRunningThreads = 0;
const UINT WM_THREADFINISHED = WM_USER + 1;

LPCTSTR szThreadStates[]
{
	TEXT("not Started"),
	TEXT("not Started"),
	TEXT("not Started")
};

//
// Helper's functions
//

void FillArray(int* arr, int size)
{
	for (int i = 0; i < size; ++i)
		arr[i] = i;
	for (int k = 0; k < 1024; ++k)
	{
		int i = rand() % size;
		int j = rand() % size;
		int t = arr[i];
		arr[i] = arr[j];
		arr[j] = t;
	}
}

BOOL CheckArray(int* arr, int size)
{
	for (int i = 0; i < size; ++i)
		if (arr[i] != i)
			return FALSE;
	return TRUE;
}

void SelectSort(int* arr, int size)
{
	int tmp;
	for (int i = 0; i < size; ++i) // i - номер текущего шага
	{
		int pos = i;
		tmp = arr[i];
		for (int j = i + 1; j < size; ++j) // цикл выбора наименьшего элемента
		{
			if (arr[j] < tmp)
			{
				pos = j;
				tmp = arr[j];
			}
		}
		arr[pos] = arr[i];
		arr[i] = tmp; // мен€ем местами наименьший с a[i]
	}
}

void BubbleSort(int* arr, int size)
{
	int tmp;

	for (int i = 0; i < size - 1; ++i) // i - номер прохода
	{
		for (int j = 0; j < size - 1; ++j) // внутренний цикл прохода
		{
			if (arr[j + 1] < arr[j])
			{
				tmp = arr[j + 1];
				arr[j + 1] = arr[j];
				arr[j] = tmp;
			}
		}
	}
}

void QuickSort(int* a, int N) {
	// Ќа входе - массив a[], a[N] - его последний элемент.

	int i = 0, j = N;      // поставить указатели на исходные места
	int temp, p;

	p = a[N >> 1];      // центральный элемент

						// процедура разделени€
	do {
		while (a[i] < p) i++;
		while (a[j] > p) j--;

		if (i <= j) {
			temp = a[i];
			a[i] = a[j];
			a[j] = temp;
			i++; j--;
		}
	} while (i <= j);

	// рекурсивные вызовы, если есть, что сортировать
	if (j > 0) QuickSort(a, j);
	if (N > i) QuickSort(a + i, N - i);
}

//
// Thread procedures
//

DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	SIZE_T index = (SIZE_T)lpParameter;

	WaitForSingleObject(hMutexSort, INFINITE);
	szThreadStates[index] = TEXT("Running");
	InvalidateRect(hMainWnd, NULL, TRUE);

	FillArray(pArray, nArraySize);
	switch (index)
	{
	case 0:
		SelectSort(pArray, nArraySize);
		break;
	case 1:
		BubbleSort(pArray, nArraySize);
		break;
	case 2:
		QuickSort(pArray, nArraySize - 1);
		break;
	}

	ReleaseMutex(hMutexCheck);
	PostMessage(hMainWnd, WM_THREADFINISHED, (WPARAM)index, 0);
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
		hMutexSort = CreateMutex(NULL, FALSE, NULL);
		hMutexCheck = CreateMutex(NULL, TRUE, NULL);
		pArray = new int[nArraySize];
		break;

	case WM_DESTROY:
		delete[] pArray;
		CloseHandle(hMutexSort);
		CloseHandle(hMutexCheck);
		PostQuitMessage(0);
		break;

	case WM_PAINT:
	{
		static TCHAR szText[256] = { 0 };

		RECT rc = { 0 };
		GetClientRect(hWnd, &rc);
		rc.left += 8;
		rc.top += 8;

		PAINTSTRUCT ps = { 0 };
		HDC hdc = BeginPaint(hWnd, &ps);

		_stprintf_s(szText,
			TEXT("Selection Sort: %s\nBubble Sort: %s\nQuick Sort: %s"),
			szThreadStates[0],
			szThreadStates[1],
			szThreadStates[2]);
		DrawText(hdc, szText, -1, &rc, DT_LEFT | DT_TOP);

		EndPaint(hWnd, &ps);
		return 0;
	}

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_RUNTHREADS:
			if (nRunningThreads)
				break;
			nRunningThreads = 3;
			CreateThread(NULL, 0, ThreadProc, (LPVOID)0, 0, NULL);
			CreateThread(NULL, 0, ThreadProc, (LPVOID)1, 0, NULL);
			CreateThread(NULL, 0, ThreadProc, (LPVOID)2, 0, NULL);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		}
		break;

	case WM_THREADFINISHED:

		WaitForSingleObject(hMutexCheck, INFINITE);
		szThreadStates[wParam] = CheckArray(pArray, nArraySize) ? TEXT("Finished Successful") : TEXT("Finished with Fail");
		ReleaseMutex(hMutexSort);

		nRunningThreads--;
		InvalidateRect(hWnd, 0, TRUE);
		break;
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
	wcx.lpszMenuName = MAKEINTRESOURCE(IDM_MAINMENU);
	wcx.lpszClassName = TEXT("SomeClassName");
	if (RegisterClassEx(&wcx) == 0)
		return 1;

	hMainWnd = CreateWindowEx(0,
		TEXT("SomeClassName"),
		TEXT("WinProg: Threads with Mutex."),
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
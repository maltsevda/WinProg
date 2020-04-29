#include <windows.h>
#include "resource.h"

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

//
// Common Data
//

HWND hMainDlg = NULL;
const int nArraySize = 65536;
int nSortState[3] = { 0 };

// массив и его "охрана"
HANDLE hMutex = NULL;
int* pArray = new int[nArraySize];

// блокируем выход, если работают дочерние потоки
HANDLE hSortThread[3] = { NULL };
BOOL bEmergencyExit = FALSE;

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

BOOL IsArraySorted(int* arr, int size)
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
			// в случае аварийного выхода - прерываем поток с ошибкой
			if (bEmergencyExit)
				ExitThread(1);

			if (arr[j] < tmp)
			{
				pos = j;
				tmp = arr[j];
			}
		}
		arr[pos] = arr[i];
		arr[i] = tmp; // меняем местами наименьший с a[i]
	}
}

void BubbleSort(int* arr, int size)
{
	int tmp;

	for (int i = 0; i < size - 1; ++i) // i - номер прохода
	{
		for (int j = 0; j < size - 1; ++j) // внутренний цикл прохода
		{
			// в случае аварийного выхода - прерываем поток с ошибкой
			if (bEmergencyExit)
				ExitThread(1);

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
	// На входе - массив a[], a[N] - его последний элемент.

	int i = 0, j = N;      // поставить указатели на исходные места
	int temp, p;

	p = a[N >> 1];      // центральный элемент

						// процедура разделения
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

LPCTSTR StateToString(int nState)
{
	switch (nState)
	{
	case 0: return TEXT("READY");
	case 1: return TEXT("RUNNING!");
	case 2: return TEXT("SUCCESS");
	case 3: return TEXT("FAILED");
	}
	return TEXT("");
}

void UpdateDialog(int index, int state)
{
	nSortState[index] = state;
	PostMessage(hMainDlg, WM_APP, 0, 0);
}

//
// Thread Proc
//

DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	int index = (int)(LONG_PTR)lpParameter;

	UpdateDialog(index, 1);

	if (WaitForSingleObject(hMutex, INFINITE) != WAIT_OBJECT_0)
		return 1;

	FillArray(pArray, nArraySize);
	switch (index)
	{
	case 0: SelectSort(pArray, nArraySize); break;
	case 1: BubbleSort(pArray, nArraySize); break;
	case 2: QuickSort(pArray, nArraySize - 1); break;
	}
	BOOL bSorted = IsArraySorted(pArray, nArraySize);

	ReleaseMutex(hMutex);

	if (bSorted)
		UpdateDialog(index, 2);
	else
		UpdateDialog(index, 3);

	return 0;
}

//
// Entry Point and Dialog Procedure
//

INT_PTR CALLBACK MainDlgProc(HWND hDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uiMsg)
	{
	case WM_INITDIALOG:
		hMainDlg = hDlg;
		hMutex = CreateMutex(NULL, FALSE, NULL);
		PostMessage(hMainDlg, WM_APP, 0, 0);
		return TRUE;

	case WM_COMMAND:
		if (HIWORD(wParam))
			break;
		switch (LOWORD(wParam))
		{
		case IDC_RUN:
			hSortThread[0] = CreateThread(NULL, 0, ThreadProc, (LPVOID)0, 0, NULL);
			hSortThread[1] = CreateThread(NULL, 0, ThreadProc, (LPVOID)1, 0, NULL);
			hSortThread[2] = CreateThread(NULL, 0, ThreadProc, (LPVOID)2, 0, NULL);
			return TRUE;
		case IDCANCEL:
			bEmergencyExit = TRUE; // устанавливаем "аварийный" выход
			// и ждем завершения всех дочерних потоков (NULL - тоже хороший вариант)
			WaitForMultipleObjects(3, hSortThread, TRUE, INFINITE);
			EndDialog(hDlg, 0);
			return TRUE;
		}
		break;

	case WM_APP:
		SetDlgItemText(hDlg, IDC_SELECTSORT, StateToString(nSortState[0]));
		SetDlgItemText(hDlg, IDC_BUBBLESORT, StateToString(nSortState[1]));
		SetDlgItemText(hDlg, IDC_QUICKSORT, StateToString(nSortState[2]));
		EnableWindow(GetDlgItem(hDlg, IDC_RUN), !(nSortState[0] == 1 || nSortState[1] == 1 || nSortState[2] == 1));
		return TRUE;
	}

	return FALSE;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
{
	return (int)DialogBox(hInst, MAKEINTRESOURCE(IDD_MAIN), NULL, MainDlgProc);
}

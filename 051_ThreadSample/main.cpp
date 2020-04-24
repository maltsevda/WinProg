#include <windows.h>
#include "resource.h"

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

//
// Common Data
//

HWND hMainDlg = NULL;
const int nArraySize = 65535;
volatile int nSortState[3] = { 0 };

//
// Helper's functions
//

void FillArray(int* arr, int size)
{
	for (int i = 0; i < size; ++i)
		arr[i] = rand() % 1000;
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
	}
	return TEXT("");
}

void UpdateDialog(int index, int state)
{
	nSortState[index] = state;
	PostMessage(hMainDlg, WM_APP + 1, 0, 0);
}

//
// Threrad Proc
//

DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	int index = (int)(LONG_PTR)lpParameter;

	UpdateDialog(index, 1);

	int* array = new int[nArraySize];
	FillArray(array, nArraySize);

	switch (index)
	{
	case 0: SelectSort(array, nArraySize); break;
	case 1: BubbleSort(array, nArraySize); break;
	case 2: QuickSort(array, nArraySize - 1); break;
	}

	delete[] array;

	UpdateDialog(index, 0);
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
		PostMessage(hMainDlg, WM_APP + 1, 0, 0);
		return TRUE;

	case WM_COMMAND:
		if (HIWORD(wParam))
			break;
		switch (LOWORD(wParam))
		{
		case IDC_RUN:
			CreateThread(NULL, 0, ThreadProc, (LPVOID)0, 0, NULL);
			CreateThread(NULL, 0, ThreadProc, (LPVOID)1, 0, NULL);
			CreateThread(NULL, 0, ThreadProc, (LPVOID)2, 0, NULL);
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			return TRUE;
		}
		break;

	case WM_APP + 1:
		SetDlgItemText(hDlg, IDC_SELECTSORT, StateToString(nSortState[0]));
		SetDlgItemText(hDlg, IDC_BUBBLESORT, StateToString(nSortState[1]));
		SetDlgItemText(hDlg, IDC_QUICKSORT, StateToString(nSortState[2]));
		EnableWindow(GetDlgItem(hDlg, IDC_RUN), !(nSortState[0] || nSortState[1] || nSortState[2]));
		return TRUE;
	}

	return FALSE;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
{
	return (int)DialogBox(hInst, MAKEINTRESOURCE(IDD_MAIN), NULL, MainDlgProc);
}

// user1
// запускать первым
// записывает данные в MMF

#include <windows.h>
#include <tchar.h>

#define BUF_SIZE 256
LPCTSTR szName = TEXT("Local\\MyFileMappingObject");
LPCTSTR szMsg = TEXT("Message from first user!");

int _tmain(int argc, TCHAR* argv[])
{
	HANDLE hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security
		PAGE_READWRITE,          // read/write access
		0,                       // maximum object size (high-order DWORD)
		BUF_SIZE,                // maximum object size (low-order DWORD)
		szName);                 // name of mapping object

	if (hMapFile == NULL)
	{
		_tprintf(TEXT("Could not create file mapping object (%d).\n"), GetLastError());
		return 1;
	}

	LPVOID pBuf = MapViewOfFile(
		hMapFile,				// handle to map object
		FILE_MAP_ALL_ACCESS,	// read/write permission
		0,
		0,
		BUF_SIZE);

	if (pBuf == NULL)
	{
		_tprintf(TEXT("Could not map view of file (%d).\n"), GetLastError());
		CloseHandle(hMapFile);
		return 2;
	}

	CopyMemory(pBuf, szMsg, _tcslen(szMsg) * sizeof(TCHAR));
	_tprintf(TEXT("Press any key when file will be read.\n"));
	_gettch();					// 'зависаем' на время, пока второй процесс читает данные

	UnmapViewOfFile(pBuf);
	CloseHandle(hMapFile);

	return 0;
}
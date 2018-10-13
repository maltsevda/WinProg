// user2
// запускать после user1
// читает данные из MMF

#include <windows.h>
#include <stdio.h>		// sprintf_s (_MBCS)
#include <tchar.h>

#define BUF_SIZE 256
LPCTSTR szName = TEXT("Local\\MyFileMappingObject");

int _tmain(int argc, TCHAR* argv[])
{
	HANDLE hMapFile = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,   // read/write access
		FALSE,                 // do not inherit the name
		szName);               // name of mapping object

	if (hMapFile == NULL)
	{
		_tprintf(TEXT("Could not open file mapping object (%d).\n"), GetLastError());
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

	_tprintf(TEXT("File content: \"%s\".\n"), (LPCTSTR)pBuf);

	UnmapViewOfFile(pBuf);
	CloseHandle(hMapFile);

	return 0;
}

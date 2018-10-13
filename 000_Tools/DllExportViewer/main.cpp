#include <windows.h>
#include <stdio.h>

#define MAKE_PTR(Type, Base, Offset) ((Type)((BYTE*)(Base) + (SIZE_T)(Offset)))

int main(int argc, char* argv[])
{
	if (argc <= 1)
	{
		printf("Wrong attributes!\n");
		return 1;
	}

	HMODULE hLibrary = LoadLibraryExA(argv[1], NULL, DONT_RESOLVE_DLL_REFERENCES);
	if (!hLibrary)
	{
		printf("Library not found!\n");
		return 2;
	}

	PIMAGE_DOS_HEADER pDosHeader = MAKE_PTR(PIMAGE_DOS_HEADER, hLibrary, 0);
	if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		printf("Error in DOS Header signature MZ!\n");
		FreeLibrary(hLibrary);
		return 3;
	}

	PIMAGE_NT_HEADERS pNtHeader = MAKE_PTR(PIMAGE_NT_HEADERS, hLibrary, pDosHeader->e_lfanew);
	if (pNtHeader->Signature != IMAGE_NT_SIGNATURE
		&& pNtHeader->OptionalHeader.NumberOfRvaAndSizes != 0x10)
	{
		printf("Error in PE Header signature!\n");
		FreeLibrary(hLibrary);
		return 4;
	}

	PIMAGE_EXPORT_DIRECTORY pExportDir = MAKE_PTR(PIMAGE_EXPORT_DIRECTORY, hLibrary,
		pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	ULONG* pszArrayOfNames = MAKE_PTR(ULONG*, hLibrary, pExportDir->AddressOfNames);
	DWORD nNumberOfNames = pExportDir->NumberOfNames;

	for (DWORD i = 0; i < nNumberOfNames; i++)
	{
		LPSTR szName = MAKE_PTR(LPSTR, hLibrary, pszArrayOfNames[i]);
		printf("%4i. %s\n", i + 1, szName);
	}

	FreeLibrary(hLibrary);
	return 0;
}
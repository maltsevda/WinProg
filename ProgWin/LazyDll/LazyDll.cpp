#include "LazyDll.h"
#include <tchar.h>

LAZYDLL_API LPCTSTR APIENTRY GetSecondaryString(INT iNumber)
{
#ifndef _WIN64
#pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
#endif

	static TCHAR szResult[256] = { 0 };
	_stprintf_s(szResult, TEXT("--== Lazy DLL is loaded: %i ==--"), iNumber);
	return szResult;
}

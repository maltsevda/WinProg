#include "LazyDll.h"

LAZYDLL_API LPCTSTR APIENTRY GetSecondaryString(INT iNumber)
{
#pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
    
	static TCHAR szResult[256] = { 0 };
	_stprintf_s(szResult, TEXT("--== Lazy DLL is loaded: %i ==--"), iNumber);
	return szResult;
}

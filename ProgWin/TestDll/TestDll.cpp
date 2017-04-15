#include "TestDll.h"
#include <tchar.h>

TESTDLL_API UINT nCallCount = 0;

TESTDLL_API LPCTSTR APIENTRY GetMainString(FLOAT fNumber)
{
	static TCHAR szResult[256] = { 0 };
	_stprintf_s(szResult, TEXT("Hello world! Count: %i. Number: %.4f."), ++nCallCount, fNumber);
	return szResult;
}

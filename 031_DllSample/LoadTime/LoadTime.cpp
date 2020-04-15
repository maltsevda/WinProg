#include "LoadTime.h"
#include <tchar.h>

#ifdef _WIN64
#define EXPORTFUNC
#else
#define EXPORTFUNC comment(linker, "/export:" __FUNCTION__ "=" __FUNCDNAME__)
#endif

LOADTIME_API LPCTSTR CALLBACK LoadTime_Function(FLOAT fValue)
{
#pragma EXPORTFUNC
	static TCHAR szText[256] = { 0 };
	_stprintf_s(szText, TEXT("Value: %.3f"), fValue);
	return szText;
}

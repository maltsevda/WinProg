#include "RunTime.h"
#include <tchar.h>

LPCTSTR CALLBACK RunTime_Function(FLOAT fValue)
{
	static TCHAR szText[256] = { 0 };
	_stprintf_s(szText, TEXT("Value: %.3f"), fValue);
	return szText;
}

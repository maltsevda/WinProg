#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#ifdef TESTDLL_EXPORTS
#define TESTDLL_API __declspec(dllexport)
#else
#define TESTDLL_API __declspec(dllimport)
#endif

extern TESTDLL_API UINT nCallCount;

TESTDLL_API LPCTSTR APIENTRY GetMainString(FLOAT fNumber);

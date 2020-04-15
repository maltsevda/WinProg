#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#ifdef ImplicitDLL_EXPORTS
#define IMPLICITDLL_API __declspec(dllexport)
#else
#define IMPLICITDLL_API __declspec(dllimport)
#endif

extern IMPLICITDLL_API UINT nCallCount;

IMPLICITDLL_API LPCTSTR APIENTRY GetMainString(FLOAT fNumber);

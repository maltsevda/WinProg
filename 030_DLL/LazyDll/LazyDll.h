#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#ifdef LazyDLL_EXPORTS
#define LAZYDLL_API __declspec(dllexport)
#else
#define LAZYDLL_API __declspec(dllimport)
#endif

extern "C" LAZYDLL_API LPCTSTR APIENTRY GetSecondaryString();



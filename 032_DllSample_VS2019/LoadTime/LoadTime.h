#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#ifdef LOADTIME_EXPORTS
#define LOADTIME_API __declspec(dllexport)
#else
#define LOADTIME_API __declspec(dllimport)
#endif

extern "C" LOADTIME_API LPCTSTR CALLBACK LoadTime_Function(FLOAT fValue);

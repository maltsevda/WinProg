#include "LazyDll.h"

LAZYDLL_API LPCTSTR APIENTRY GetSecondaryString()
{
#pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
	return TEXT("--== Lazy DLL is loaded ==--");
}

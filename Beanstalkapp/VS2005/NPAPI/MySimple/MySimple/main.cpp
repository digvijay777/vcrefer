#include <windows.h>

HINSTANCE		gInst	= NULL;;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	gInst = hinstDLL;

	return TRUE;
}

#pragma once

#if !defined(str_ServiceName) || !defined(str_ServiceDisplay) || !defined(str_ServiceDescription)
#error "Can not define service info"
#endif

#define PARSE_PARAMETER(str, fun)\
	if((argc >= 2) && (stricmp((str), &argv[1][1]) == 0))\
	{\
	return fun();\
	}\


int				ServiceCreate();
int				ServiceStart();
void WINAPI		ServiceControlMain(DWORD dwArgc, LPTSTR* lpszArgv);
void WINAPI		ServiceControlHandler(DWORD fdwControl);
void			ServerMain(DWORD dwArgc, LPTSTR* lpszArgv);
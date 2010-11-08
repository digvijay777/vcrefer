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
int				ShowExplorer();
void WINAPI		ServiceControlMain(DWORD dwArgc, LPTSTR* lpszArgv);
void WINAPI		ServiceControlHandler(DWORD fdwControl);
void			ServerMain(DWORD dwArgc, LPTSTR* lpszArgv);

extern	DWORD			g_dwMainThreadID;

#define UM_SERVICEBASE			(WM_USER+0x100)
#define UM_UNHOOK				UM_SERVICEBASE+1
#define UM_HOOK					UM_SERVICEBASE+2
#define UM_REHOOK				UM_SERVICEBASE+3


/*
 *	IP¹ýÂËº¯Êý
 */
#include "stdafx.h"
#include "../Installer/Installer.h"
#include "IpFilter.h"


WSPPROC_TABLE		NextProcTable		= {0};

SOCKET WSPAPI WSPSocket(
						int			af,                               
						int			type,                             
						int			protocol,                         
						LPWSAPROTOCOL_INFOW lpProtocolInfo,   
						GROUP		g,                              
						DWORD		dwFlags,                        
						LPINT		lpErrno
						)
{
	CHAR		szPath[MAX_PATH]		= {0};

	GetModuleFileNameA(NULL, szPath, MAX_PATH);
	ATLTRACE("%s->LspHttpFilter.DLL: WSPSocket ...\n", szPath);

	return NextProcTable.lpWSPSocket(
		af, type, protocol, lpProtocolInfo, g, dwFlags, lpErrno);
}

int WSPAPI WSPStartup(
					  WORD				wVersionRequested,
					  LPWSPDATA			lpWSPData,
					  LPWSAPROTOCOL_INFOW	lpProtocolInfo,
					  WSPUPCALLTABLE		upcallTable,
					  LPWSPPROC_TABLE		lpProcTable
					  )
{
	TCHAR				sLibraryPath[MAX_PATH];
	LPWSPSTARTUP        WSPStartupFunc      = NULL;
	HMODULE				hLibraryHandle		= NULL;
	INT                 ErrorCode           = 0; 
	WSAPROTOCOL_INFOW	NextProtocolInfo;

	if (!GetHookProvider(lpProtocolInfo, sLibraryPath, &NextProtocolInfo)
		|| (hLibraryHandle = LoadLibrary(sLibraryPath)) == NULL
		|| (WSPStartupFunc = (LPWSPSTARTUP)GetProcAddress(
		hLibraryHandle, "WSPStartup")) == NULL
		)
	{
		return WSAEPROVIDERFAILEDINIT;
	}

	if ((ErrorCode = WSPStartupFunc(wVersionRequested, lpWSPData
		, &NextProtocolInfo, upcallTable, lpProcTable)) != ERROR_SUCCESS)
		return ErrorCode;

	NextProcTable = *lpProcTable;

	lpProcTable->lpWSPSocket = WSPSocket;

	return 0;
}

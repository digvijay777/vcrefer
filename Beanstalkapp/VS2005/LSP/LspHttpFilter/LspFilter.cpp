#include "stdafx.h"

#include <ws2spi.h>
#include <io.h>				
#include <sporder.h>
#include <TCHAR.H>

//
// 全局变量，用来保存系统服务提供者30个服务函数指针
//
WSPPROC_TABLE		NextProcTable   ;

//
// 全局变量，用来临时保存全部服务提供者的信息
//
LPWSAPROTOCOL_INFOW ProtocolInfo		= NULL;
DWORD				ProtocolInfoSize	= 0;
INT					TotalProtocols		= 0;

BOOL GetProviders()
{
	INT ErrorCode;

	ProtocolInfo		= NULL;
	ProtocolInfoSize	= 0;
	TotalProtocols		= 0;

	if (WSCEnumProtocols(NULL, ProtocolInfo, &ProtocolInfoSize
		, &ErrorCode) == SOCKET_ERROR && ErrorCode != WSAENOBUFS)
	{
//		printf("First WSCEnumProtocols failed %d\n", ErrorCode);
		return(FALSE);
	}

	if ((ProtocolInfo = (LPWSAPROTOCOL_INFOW) 
		GlobalAlloc(GPTR, ProtocolInfoSize)) == NULL)
	{
//		printf("Cannot enumerate Protocols %d\n", GetLastError());
		return(FALSE);
	}

	if ((TotalProtocols = WSCEnumProtocols(NULL
		, ProtocolInfo, &ProtocolInfoSize, &ErrorCode)) == SOCKET_ERROR)
	{
//		printf("Second WSCEnumProtocols failed %d\n", ErrorCode);
		return(FALSE);
	}

//	printf("Found %d protocols\n",TotalProtocols); 
	return(TRUE);
}

void FreeProviders(void)
{
	GlobalFree(ProtocolInfo);
}

BOOL GetHookProvider(
					 IN	WSAPROTOCOL_INFOW	*pProtocolInfo, 
					 OUT	TCHAR				*sPathName,
					 OUT WSAPROTOCOL_INFOW	*pNextProtocolInfo
					 )
{
	if(pProtocolInfo->ProtocolChain.ChainLen <= 1)
		return FALSE;
	GetProviders();
	__try
	{
		for(int i = pProtocolInfo->ProtocolChain.ChainLen - 1; i > 0; i--)
		{
			for(int j = 0; j < TotalProtocols; j++)
			{
				if(pProtocolInfo->ProtocolChain.ChainEntries[i]	
				== ProtocolInfo[j].dwCatalogEntryId)
				{
					INT iErrno, iProviderPathLen = MAX_PATH;
					if(WSCGetProviderPath(&ProtocolInfo[j].ProviderId
						, sPathName, &iProviderPathLen, &iErrno) == SOCKET_ERROR)
						return FALSE;
					if (!ExpandEnvironmentStrings(sPathName, sPathName, MAX_PATH))
						return FALSE;
					memcpy(pNextProtocolInfo
						, pProtocolInfo, sizeof(WSAPROTOCOL_INFOW));
					return TRUE;
				}
			}
		}
	}
	__finally
	{
		FreeProviders();
	}

	return FALSE;
}

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
	OutputDebugString(_T("MinLSP.DLL: WSPSocket ...\n"));

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
	OutputDebugString(_T("MinLSP.DLL: WSPStartup...\n"));

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
		return WSAEPROVIDERFAILEDINIT;

	if ((ErrorCode = WSPStartupFunc(wVersionRequested, lpWSPData
		, &NextProtocolInfo, upcallTable, lpProcTable)) != ERROR_SUCCESS)
		return ErrorCode;

	NextProcTable = *lpProcTable;

	lpProcTable->lpWSPSocket = WSPSocket;

	return 0;
}
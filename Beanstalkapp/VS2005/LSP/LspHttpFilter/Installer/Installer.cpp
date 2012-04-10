/*
 *	安装LSP和销LSP组件
 */
#include "stdafx.h"
#include "Installer.h"

#pragma comment(lib, "Ws2_32.lib")


//
// 默认安装的服务提供者文件名和路径变量
//
#define		PROVIDER_PATH	L"MinLSP.dll"
extern TCHAR		gsProviderPath[MAX_PATH];

// {2D1A12B3-474D-48dc-A942-56D48C978E88}
static GUID ProviderGuid = 
{ 0x2d1a12b3, 0x474d, 0x48dc, { 0xa9, 0x42, 0x56, 0xd4, 0x8c, 0x97, 0x8e, 0x88 } };
// {98E5AAD3-6CBE-4ec5-B25D-74B234859FEA}
static GUID ProviderChainGuid = 
{ 0x98e5aad3, 0x6cbe, 0x4ec5, { 0xb2, 0x5d, 0x74, 0xb2, 0x34, 0x85, 0x9f, 0xea } };

//
// 函数申名
// 
WCHAR* S2W(char* ansi);
BOOL GetProviders();
//
// 全局变量，用来临时保存全部服务提供者的信息
//
LPWSAPROTOCOL_INFOW ProtocolInfo		= NULL;
DWORD				ProtocolInfoSize	= 0;
INT					TotalProtocols		= 0;

WCHAR* S2W(char* ansi)
{
	int i;
	WCHAR* unicode;
	unicode = (WCHAR*) malloc ((strlen(ansi)+2)*sizeof(WCHAR));
	for (i = 0; i<(signed)strlen(ansi)+1; i++)
		unicode[i] = ansi[i];
	unicode[i] = 0;
	return unicode;
}

BOOL GetProviders()
{
	INT ErrorCode;

	ProtocolInfo		= NULL;
	ProtocolInfoSize	= 0;
	TotalProtocols		= 0;

	if (WSCEnumProtocols(NULL, ProtocolInfo, &ProtocolInfoSize
		, &ErrorCode) == SOCKET_ERROR && ErrorCode != WSAENOBUFS)
	{
		printf("First WSCEnumProtocols failed %d\n", ErrorCode);
		return(FALSE);
	}

	if ((ProtocolInfo = (LPWSAPROTOCOL_INFOW) 
		GlobalAlloc(GPTR, ProtocolInfoSize)) == NULL)
	{
		printf("Cannot enumerate Protocols %d\n", GetLastError());
		return(FALSE);
	}

	if ((TotalProtocols = WSCEnumProtocols(NULL
		, ProtocolInfo, &ProtocolInfoSize, &ErrorCode)) == SOCKET_ERROR)
	{
		printf("Second WSCEnumProtocols failed %d\n", ErrorCode);
		return(FALSE);
	}

	printf("Found %d protocols\n",TotalProtocols); 
	return(TRUE);
}

void FreeProviders(void)
{
	GlobalFree(ProtocolInfo);
}

void SetProtocolInfo(
					 OUT	BOOL	&IsSet,
					 OUT DWORD	&CatalogId,
					 IN	WSAPROTOCOL_INFOW &InProtocolInfo,
					 OUT WSAPROTOCOL_INFOW &OutProtocolInfo1,
					 OUT WSAPROTOCOL_INFOW *pOutProtocolInfo2
					 )
{
	IsSet		= TRUE;
	CatalogId	= InProtocolInfo.dwCatalogEntryId;

	memcpy(&OutProtocolInfo1, &InProtocolInfo, sizeof(WSAPROTOCOL_INFOW));
	OutProtocolInfo1.dwServiceFlags1 
		= InProtocolInfo.dwServiceFlags1 & (~XP1_IFS_HANDLES); 
	if(pOutProtocolInfo2 != NULL)
		memcpy(pOutProtocolInfo2, &OutProtocolInfo1, sizeof(WSAPROTOCOL_INFOW));
}

void SetProtocolChain(
					  IN		TCHAR *sName,
					  IN OUT	WSAPROTOCOL_INFOW &ProtocolInfo,
					  IN		DWORD LayeredCatalogId,
					  IN		DWORD NextCatalogId,
					  OUT		WSAPROTOCOL_INFOW &OutProtocolInfo
					  )
{
	WCHAR	ChainName[WSAPROTOCOL_LEN+1];
	swprintf(ChainName, L"%s [%s]", sName, ProtocolInfo.szProtocol);
	wcscpy(ProtocolInfo.szProtocol, ChainName);

	ProtocolInfo.ProtocolChain.ChainEntries[0]	= LayeredCatalogId;
	ProtocolInfo.ProtocolChain.ChainEntries[ProtocolInfo.ProtocolChain.ChainLen] 
	= NextCatalogId;
	ProtocolInfo.ProtocolChain.ChainLen++;

	memcpy(&OutProtocolInfo, &ProtocolInfo, sizeof(WSAPROTOCOL_INFOW));
}

/*
 *	安装
 */
void InstallProvider(void)
{
	INT					ErrorCode;
	LPDWORD				CatalogEntries;
	INT					i;
	INT					CatIndex;
	DWORD				LayeredCatalogId, RawOrigCatalogId;
	DWORD				TcpOrigCatalogId, UdpOrigCatalogId;
	WSAPROTOCOL_INFOW	TCPChainInfo, UDPChainInfo;
	WSAPROTOCOL_INFOW	RAWChainInfo, IPLayeredInfo, ChainArray[3];
	BOOL				RawIP	= FALSE;
	BOOL				UdpIP	= FALSE;
	BOOL				TcpIP	= FALSE;
	INT					ProvCnt = 0;

	GetProviders();

	for (i = 0; i < TotalProtocols; i++)
	{
		if(ProtocolInfo[i].iAddressFamily == AF_INET)
		{
			if (!RawIP && ProtocolInfo[i].iProtocol == IPPROTO_IP)
				SetProtocolInfo(RawIP, RawOrigCatalogId
				, ProtocolInfo[i], IPLayeredInfo, &RAWChainInfo);

			if (!TcpIP && ProtocolInfo[i].iProtocol == IPPROTO_TCP)
				SetProtocolInfo(TcpIP, TcpOrigCatalogId
				, ProtocolInfo[i], TCPChainInfo, NULL);

			if (!UdpIP && ProtocolInfo[i].iProtocol == IPPROTO_UDP)
				SetProtocolInfo(UdpIP, UdpOrigCatalogId
				, ProtocolInfo[i], UDPChainInfo, NULL);
		}
	}

	wcscpy(IPLayeredInfo.szProtocol, L"Layered IP");
	IPLayeredInfo.ProtocolChain.ChainLen = LAYERED_PROTOCOL;
	if (WSCInstallProvider(&ProviderGuid
		, gsProviderPath, &IPLayeredInfo, 1, &ErrorCode) == SOCKET_ERROR)
	{
		printf("WSCInstallProvider failed %d\n", ErrorCode);
		return;
	}

	FreeProviders();
	GetProviders();

	for (i = 0; i < TotalProtocols; i++)
	{
		if (memcmp (&ProtocolInfo[i].ProviderId, &ProviderGuid, sizeof (GUID))==0)
		{
			LayeredCatalogId = ProtocolInfo[i].dwCatalogEntryId;
			break;
		}
	}

	if (TcpIP)
		SetProtocolChain(L"Layered TCP/IP over", TCPChainInfo
		, LayeredCatalogId, TcpOrigCatalogId, ChainArray[ProvCnt++]);
	if (UdpIP)
		SetProtocolChain(L"Layered UDP/IP over", UDPChainInfo
		, LayeredCatalogId, UdpOrigCatalogId, ChainArray[ProvCnt++]);
	if (RawIP)
		SetProtocolChain(L"Layered RAW/IP over", RAWChainInfo
		, LayeredCatalogId, RawOrigCatalogId, ChainArray[ProvCnt++]);

	if (WSCInstallProvider(&ProviderChainGuid
		, gsProviderPath, ChainArray, ProvCnt, &ErrorCode) == SOCKET_ERROR)
	{
		printf("WSCInstallProvider for protocol chain failed %d\n", ErrorCode);
		return;
	}

	FreeProviders();
	GetProviders();

	if ((CatalogEntries = (LPDWORD) GlobalAlloc(
		GPTR, TotalProtocols * sizeof(DWORD))) == NULL)
	{
		printf("GlobalAlloc failed %d\n", GetLastError());
		return;
	}
	CatIndex = 0;
	for (i = 0; i < TotalProtocols; i++)
	{
		if (memcmp (&ProtocolInfo[i].ProviderId
			, &ProviderGuid, sizeof (GUID))==0 
			|| memcmp (&ProtocolInfo[i].ProviderId
			, &ProviderChainGuid, sizeof (GUID))==0)
		{
			CatalogEntries[CatIndex++] = ProtocolInfo[i].dwCatalogEntryId;
		}
	}
	for (i = 0; i < TotalProtocols; i++)
	{
		if (memcmp (&ProtocolInfo[i].ProviderId
			, &ProviderGuid, sizeof (GUID))!=0 
			&& memcmp (&ProtocolInfo[i].ProviderId
			, &ProviderChainGuid, sizeof (GUID))!=0)
		{
			CatalogEntries[CatIndex++] = ProtocolInfo[i].dwCatalogEntryId;
		}
	}
	if ((ErrorCode = WSCWriteProviderOrder(CatalogEntries, TotalProtocols)) 
		!= ERROR_SUCCESS)
	{
		printf("WSCWriteProviderOrder failed %d\n", ErrorCode);
		return;
	}

	FreeProviders();
}

/*
 *	卸载
 */
void RemoveProvider(void)
{
	INT ErrorCode;
	if (WSCDeinstallProvider(&ProviderGuid, &ErrorCode) == SOCKET_ERROR)
		printf("WSCDeistallProvider for Layer failed %d\n", ErrorCode);
	if (WSCDeinstallProvider(&ProviderChainGuid, &ErrorCode) == SOCKET_ERROR)
		printf("WSCDeistallProvider for Chain failed %d\n", ErrorCode);
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
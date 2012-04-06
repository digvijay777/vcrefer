// LspHttpFilter.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <Ws2spi.h>
#include <Sporder.h>
#include <tchar.h>
#include <strsafe.h>


#pragma comment(lib, "Ws2_32.lib")



int						totalprotos				= 0;
DWORD					protoinfosize			= 0;
LPWSAPROTOCOL_INFOW		protoinfo				= NULL;
HINSTANCE				gInst					= NULL;

// {2D1A12B3-474D-48dc-A942-56D48C978E88}
static const GUID GUID_LSPHTTPFILTER = 
	{ 0x2d1a12b3, 0x474d, 0x48dc, { 0xa9, 0x42, 0x56, 0xd4, 0x8c, 0x97, 0x8e, 0x88 } };
// {98E5AAD3-6CBE-4ec5-B25D-74B234859FEA}
static const GUID GUID_LSPHTTPFILTER_CHAIN = 
	{ 0x98e5aad3, 0x6cbe, 0x4ec5, { 0xb2, 0x5d, 0x74, 0xb2, 0x34, 0x85, 0x9f, 0xea } };


/*
 *	DLL 入口函数
 */
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	gInst = hModule;
    return TRUE;
}

BOOL getfilter()
{
	int  errorcode;

	protoinfo=NULL;
	totalprotos=0;
	protoinfosize=0;

	if(WSCEnumProtocols(NULL,protoinfo,&protoinfosize,&errorcode)==SOCKET_ERROR)
	{
		if(errorcode!=WSAENOBUFS)
		{
			//_tprintf(_T("First WSCEnumProtocols Error: %d\n"),errorcode);
			return FALSE;
		}
	}

	if((protoinfo=(LPWSAPROTOCOL_INFOW)GlobalAlloc(GPTR,protoinfosize))==NULL)
	{
		//_tprintf(_T("GlobalAlloc in getfilter Error: %d\n"),GetLastError());
		return FALSE;
	}

	if((totalprotos=WSCEnumProtocols(NULL,protoinfo,&protoinfosize,&errorcode))==SOCKET_ERROR)
	{
		//_tprintf(_T("Second WSCEnumProtocols Error: %d\n"),GetLastError());
		return FALSE;
	}

//	_tprintf(_T("Found %d protocols!\n"),totalprotos); 
	return TRUE;
}

void freefilter()
{
	GlobalFree(protoinfo);
}
/*
 *	注册LSP
 */
STDAPI DllRegisterServer(void)
{
	HRESULT			   hr				= S_OK;
	int                i;
	int                provcnt;
	int                cataindex;
	int                errorcode;
	BOOL               rawip=FALSE;
	BOOL               udpip=FALSE;
	DWORD              iplayercataid=0,udporigcataid=0;
	TCHAR              filter_path[MAX_PATH];            
	TCHAR              filter_name[MAX_PATH];
	TCHAR              chainname[WSAPROTOCOL_LEN+1];      
	LPDWORD            cataentries;
	WSAPROTOCOL_INFOW  iplayerinfo,udpchaininfo,chainarray[1];

	getfilter();

	for(i=0;i<totalprotos;i++)
	{
		if(!rawip
			&& protoinfo[i].iAddressFamily==AF_INET
			&& protoinfo[i].iProtocol==IPPROTO_IP)
		{
			rawip=TRUE;
			memcpy(&iplayerinfo,&protoinfo[i],sizeof(WSAPROTOCOL_INFOW));
			iplayerinfo.dwServiceFlags1=protoinfo[i].dwServiceFlags1 & (~XP1_IFS_HANDLES);
		}

		if(!udpip
			&& protoinfo[i].iAddressFamily==AF_INET
			&& protoinfo[i].iProtocol==IPPROTO_UDP)
		{
			udpip=TRUE;
			udporigcataid=protoinfo[i].dwCatalogEntryId;
			memcpy(&udpchaininfo,&protoinfo[i],sizeof(WSAPROTOCOL_INFOW));
			udpchaininfo.dwServiceFlags1=protoinfo[i].dwServiceFlags1 & (~XP1_IFS_HANDLES);
		}
	}

	_tcscpy(iplayerinfo.szProtocol,_T("LspHttpFilter"));
	iplayerinfo.ProtocolChain.ChainLen=LAYERED_PROTOCOL;

	GetModuleFileName(gInst, filter_path, MAX_PATH);

	if(WSCInstallProvider((LPGUID)&GUID_LSPHTTPFILTER, filter_path, &iplayerinfo, 1, &errorcode)==SOCKET_ERROR)
	{
		_tprintf(_T("WSCInstallProvider Error: %d\n"), errorcode);
		return errorcode;
	}

	freefilter();

	getfilter();

	for(i=0;i<totalprotos;i++)
	{
		if(memcmp(&protoinfo[i].ProviderId, &GUID_LSPHTTPFILTER, sizeof(GUID))==0)
		{
			iplayercataid = protoinfo[i].dwCatalogEntryId;
			break;
		}
	}

	provcnt=0;
	if(udpip)
	{
		_tcscpy(chainname,_T("LspHttpFilter"));
		_tcscpy(udpchaininfo.szProtocol,chainname);

		if(udpchaininfo.ProtocolChain.ChainLen==BASE_PROTOCOL)
		{
			udpchaininfo.ProtocolChain.ChainEntries[1]=udporigcataid;
		}
		else
		{
			for(i=udpchaininfo.ProtocolChain.ChainLen;i>0;i--)
			{
				udpchaininfo.ProtocolChain.ChainEntries[i+1]=udpchaininfo.ProtocolChain.ChainEntries[i];
			}
		}

		udpchaininfo.ProtocolChain.ChainLen++;
		udpchaininfo.ProtocolChain.ChainEntries[0]=iplayercataid;

		memcpy(&chainarray[provcnt++],&udpchaininfo,sizeof(WSAPROTOCOL_INFOW));
	}

	if(WSCInstallProvider((LPGUID)&GUID_LSPHTTPFILTER_CHAIN, filter_path, chainarray, provcnt, &errorcode)==SOCKET_ERROR)
	{
		_tprintf(_T("WSCInstallProvider for chain Error: %d\n"),errorcode);
		return errorcode;
	}

	freefilter();

	getfilter();

	if((cataentries=(LPDWORD)GlobalAlloc(GPTR,totalprotos*sizeof(WSAPROTOCOL_INFOW)))==NULL)
	{
		_tprintf(_T("GlobalAlloc int installfilter Error: %d\n"),errorcode);
		return errorcode ;
	}

	cataindex=0;
	for(i=0;i<totalprotos;i++)
	{
		if(memcmp(&protoinfo[i].ProviderId,&GUID_LSPHTTPFILTER,sizeof(GUID))==0 
			|| memcmp(&protoinfo[i].ProviderId,&GUID_LSPHTTPFILTER_CHAIN,sizeof(GUID))==0)
		{
			cataentries[cataindex++]=protoinfo[i].dwCatalogEntryId;
		}
	}

	for(i=0;i<totalprotos;i++)
	{
		if(memcmp(&protoinfo[i].ProviderId,&GUID_LSPHTTPFILTER,sizeof(GUID))!=0 
			&& memcmp(&protoinfo[i].ProviderId,&GUID_LSPHTTPFILTER_CHAIN,sizeof(GUID))!=0)
		{
			cataentries[cataindex++]=protoinfo[i].dwCatalogEntryId;
		}
	}

	if((errorcode==WSCWriteProviderOrder(cataentries, totalprotos))!=ERROR_SUCCESS)
	{
		_tprintf(_T("WSCWriteProviderOrder Error: %d\n"),GetLastError());
		return errorcode;
	}

	freefilter();

	_tprintf(_T("\nInstall IP Filter Successfully"));
	return hr;
}

/*
 *	注销LSP
 */
STDAPI DllUnregisterServer(void)
{
	int  errorcode;
	BOOL signal=TRUE;

	if(WSCDeinstallProvider((LPGUID)&GUID_LSPHTTPFILTER, &errorcode)==SOCKET_ERROR)
	{
		_tprintf(_T("WSCDeinstall filterguid Error: %d\n"),errorcode);
		signal=FALSE;
	}

	if(WSCDeinstallProvider((LPGUID)&GUID_LSPHTTPFILTER_CHAIN, &errorcode)==SOCKET_ERROR)
	{
		_tprintf(_T("WSCDeinstall filterchainguid Error: %d\n"),errorcode);
		signal=FALSE;
	}

	if(signal)
	{
		_tprintf(_T("Deinstall IP Filter Successfully"));
	}
	return 0;
}

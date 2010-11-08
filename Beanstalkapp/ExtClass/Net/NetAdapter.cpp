/*
 *	������Ϣ������
 */
#include "stdafx.h"
#include "NetAdapter.h"

#pragma comment(lib, "Iphlpapi.lib")

CNetAdapterManager::CNetAdapterManager()
{

}

CNetAdapterManager::~CNetAdapterManager()
{
	m_vctNetAdapter.clear();
}

/*
 *	�õ�������Ϣ
 */
BOOL CNetAdapterManager::EnumAdapterInfo()
{
	// clear
	m_vctNetAdapter.clear();

	PIP_ADAPTER_INFO			pAdapterInfo		= NULL;
	ULONG						ulInfoSize			= 0;
	DWORD						dwError				= 0;
	BOOL						bRet				= FALSE;

	dwError = GetAdaptersInfo(pAdapterInfo, &ulInfoSize);
	if(ERROR_BUFFER_OVERFLOW != dwError)
		return FALSE;

	pAdapterInfo = (PIP_ADAPTER_INFO)malloc(ulInfoSize);
	dwError = GetAdaptersInfo(pAdapterInfo, &ulInfoSize);
	if(ERROR_SUCCESS != dwError)
		return FALSE;

	bRet = InitAdpterInfo(pAdapterInfo, m_vctNetAdapter);

	free(pAdapterInfo);
	return bRet;
}

/*
 *	��ʼ��������Ϣ
 */
BOOL CNetAdapterManager::InitAdpterInfo(PIP_ADAPTER_INFO pAdapter, std::vector<NETADAPTER>& vctNetAdapter)
{
	PIP_ADAPTER_INFO		pAdapterNext		= NULL;
	BOOL					bRet				= FALSE;

	for(pAdapterNext = pAdapter; NULL != pAdapterNext; pAdapterNext = pAdapterNext->Next)
	{
		// ����ÿһ����������Ϣ
		NETADAPTER			adapter;
		
		adapter.AdapterName	= pAdapterNext->AdapterName;
		adapter.Description = pAdapterNext->Description;
		adapter.Index = pAdapterNext->Index;
		adapter.AdapterType = pAdapterNext->Type;
		
		// MACֵ
		CHAR					szMAC[20]		= {0};

		sprintf(szMAC, "%02X-%02X-%02X-%02X-%02X-%02X",
			pAdapterNext->Address[0],
			pAdapterNext->Address[1],
			pAdapterNext->Address[2],
			pAdapterNext->Address[3],
			pAdapterNext->Address[4],
			pAdapterNext->Address[5]);
		adapter.MAC = szMAC;

		// IP��ַ
		PIP_ADDR_STRING			pIPNext		= NULL;
		for(pIPNext = &(pAdapterNext->IpAddressList); NULL != pIPNext; pIPNext = pIPNext->Next)
		{
			adapter.vIP.push_back(pIPNext->IpAddress.String);
			adapter.vMask.push_back(pIPNext->IpMask.String);
		}

		// �õ�����
		for(pIPNext = &(pAdapterNext->GatewayList); NULL != pIPNext; pIPNext = pIPNext->Next)
		{
			adapter.vGetway.push_back(pIPNext->IpAddress.String);
		}

		// �õ�DNS
		InitDNSInfo(pAdapterNext->Index, adapter.vDNS);

		vctNetAdapter.push_back(adapter);
	}

	return TRUE;
}

/*
 *	��ʼ��DNS��Ϣ
 */
BOOL CNetAdapterManager::InitDNSInfo(DWORD dwIndex, std::vector<std::string>& vctDNS)
{

	PIP_PER_ADAPTER_INFO	pPerAdapterInfo		= NULL;
	PIP_ADDR_STRING			pIPNext				= NULL;
	ULONG					ulPerAdpterSize		= 0;
	DWORD					dwRes				= 0;

	dwRes = GetPerAdapterInfo(dwIndex, pPerAdapterInfo, &ulPerAdpterSize);
	if(ERROR_BUFFER_OVERFLOW != dwRes)
		return FALSE;
	pPerAdapterInfo = (PIP_PER_ADAPTER_INFO)malloc(ulPerAdpterSize);
	if(NULL == pPerAdapterInfo)
		return FALSE;

	dwRes = GetPerAdapterInfo(dwIndex, pPerAdapterInfo, &ulPerAdpterSize);
	if(ERROR_SUCCESS == dwRes)
	{
		for(pIPNext = &(pPerAdapterInfo->DnsServerList); NULL != pIPNext; pIPNext = pIPNext->Next)
		{
			vctDNS.push_back(pIPNext->IpAddress.String);
		}				
	}
	free(pPerAdapterInfo);
	
	return TRUE;
}

/*
 *	��������IP
 */
BOOL CNetAdapterManager::EnableStatic(LPCSTR lpAdapterName, std::vector<std::string>& vctIP, std::vector<std::string>& vctMask)
{

	return FALSE;
}
/*
 *	��������DNS
 */
BOOL CNetAdapterManager::EnableDNS(LPCSTR lpAdapterName, std::vector<std::string>& vctDNS)
{
	return FALSE;
}


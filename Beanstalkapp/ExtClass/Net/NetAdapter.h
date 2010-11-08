#pragma once
#include <Iphlpapi.h>
#include <vector>
#include <string>

typedef struct tagNetAdapter
{
	std::vector<std::string>		vIP;
	std::vector<std::string>		vMask;
	std::vector<std::string>		vGetway;
	std::vector<std::string>		vDNS;

	std::string						AdapterName;
	std::string						Description;
	std::string						MAC;

	DWORD							Index;
	UINT							AdapterType;
}NETADAPTER;

class CNetAdapterManager
{
public:
	CNetAdapterManager();
	~CNetAdapterManager();
public:
	std::vector<NETADAPTER>		m_vctNetAdapter;
protected:
	BOOL InitAdpterInfo(PIP_ADAPTER_INFO pAdapter, std::vector<NETADAPTER>& vctNetAdapter);
	BOOL InitDNSInfo(DWORD dwIndex, std::vector<std::string>& vctDNS);
public:
	BOOL EnumAdapterInfo();
	BOOL EnableStatic(LPCSTR lpAdapterName, std::vector<std::string>& vctIP, std::vector<std::string>& vctMask);
	BOOL EnableDNS(LPCSTR lpAdapterName, std::vector<std::string>& vctDNS);
	
};
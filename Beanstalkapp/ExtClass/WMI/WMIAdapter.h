#pragma once
#include "WMIBase.h"
#include <vector>
#include <string>

class CWMIAdapter : public CWMIBase
{
public:
	CWMIAdapter(void);
	virtual ~CWMIAdapter(void);
public:
	typedef struct tagAdapterInfo
	{
		int				nDeviceID;
		std::wstring	sMac;
		std::wstring	sNetConnection;
		std::wstring	sPNPDeviceID;
		int				nState;

		std::wstring	sIP;
		std::wstring	sSubIP;
		std::wstring	sDNS;
		std::wstring	sGateway;
	}ADAPTERINFO;
private:
	static WCHAR*					s_pNetAdapterState[];
	BOOL							m_bConnect;
public:
	std::vector<ADAPTERINFO>		m_vctAdapter;
public:
	BOOL		InitAdapterInfo();
	std::wstring ConvertArrayToString(VARIANT varArr, WCHAR* lpSplit = L";");
	VARIANT		ConvertStringToArray(std::wstring strArr, WCHAR* lpSplit = L";");
	BOOL		GetAdapterIPInfo(int nDeviceID, ADAPTERINFO& adapter);
	int			GetAdapterCount();
	BOOL		IsConnect();

	BOOL		SetGateWay(int nDeviceID, LPWSTR lpGateWay, BOOL* pBRestart = NULL);
	BOOL		SetIP(int nDeviceID, LPWSTR lpIP, LPWSTR lpSubIP, BOOL* pBRestart = NULL);
	BOOL		SetDNS(int nDeviceID, LPWSTR lpDNS, BOOL* pBRestart = NULL);
private:
	BOOL		GetAdapterConfigObject(IWbemClassObject** pObject, int nIndex);
};

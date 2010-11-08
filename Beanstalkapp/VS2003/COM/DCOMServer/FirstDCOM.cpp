// FirstDCOM.cpp : CFirstDCOM 的实现

#include "stdafx.h"
#include "FirstDCOM.h"
#include ".\firstdcom.h"


// CFirstDCOM

STDMETHODIMP CFirstDCOM::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IFirstDCOM
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CFirstDCOM::GetServerName(BSTR* lpServer)
{
	// TODO: 在此添加实现代码
	WCHAR		szCompterName[MAX_COMPUTERNAME_LENGTH]	= {0};
	DWORD		dwSize = MAX_COMPUTERNAME_LENGTH;

	GetComputerNameW(szCompterName, &dwSize);

	*lpServer = ::SysAllocString(szCompterName);
	return S_OK;
}

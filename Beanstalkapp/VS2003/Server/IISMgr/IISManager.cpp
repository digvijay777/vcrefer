#include "StdAfx.h"
#include ".\iismanager.h"
#include <Iads.h>
#include <Adshlp.h>


#pragma comment(lib, "ActiveDS.lib")
#pragma comment(lib,"adsiid.lib")

CIISManager::CIISManager(void)
{
}

CIISManager::~CIISManager(void)
{
}

void CIISManager::Test()
{
	IADsContainer*		iContainer;
	IADs*				iAds;
	HRESULT				hres;

	hres = ADsGetObject(L"IIS://localhost/", IID_IADsContainer, (void **)&iContainer);
	if(S_OK != hres)
		return;

	hres = iContainer->Create(L"IIsWebServer", L"1", (IDispatch**)&iAds);
	if(S_OK != hres)
		return;

	VARIANT		var;

	iAds->Get(L"ServerComment", &var);
	TRACE("ServerComment: %S\n", var.bstrVal);
}
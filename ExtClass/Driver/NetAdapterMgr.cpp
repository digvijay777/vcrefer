#include "stdafx.h"
#include "NetAdapterMgr.h"
#include "driver.h"
#if WINVER < 0x500 || _WIN32_WINNT < 0x500 || _WIN32_WINDOWS < 0x510
#error "Windows version is low."
#endif
#include <Iphlpapi.h>
#include <Setupapi.h>
// #include <wdm.h>

#pragma comment(lib, "Setupapi.lib")
#pragma comment(lib, "Iphlpapi.lib")

CNetAdapterMgr::CNetAdapterMgr()
{
	m_wstrAdapterGUID = L"{4D36E972-E325-11CE-BFC1-08002BE10318}";
};

CNetAdapterMgr::~CNetAdapterMgr()
{

}

int CNetAdapterMgr::GetAdapterCount()
{

	EnumNetAdapter();

	return 0;
}

BOOL CNetAdapterMgr::EnumNetAdapter()
{
	HDEVINFO							hDevInfoSet		= NULL;
	SP_DEVICE_INTERFACE_DATA			ifdata			= {0};
	int									nCount			= 0;
	BOOL								bResult			= FALSE;
	GUID								guid			= {0};
	HRESULT								hres			= S_FALSE;

	m_vctNetAdapter.clear();
	// 得到GUID
	hres = CLSIDFromString((LPOLESTR)m_wstrAdapterGUID.c_str(), &guid);
	if(FAILED(hres))
	{
		SetLastError(hres);
		return FALSE;
	}
	// 取得一个该GUID相关的设备信息集句柄
	hDevInfoSet = ::SetupDiGetClassDevs(&guid,  // class GUID
		NULL,     // 无关键字
		NULL,     // 不指定父窗口句柄
		DIGCF_PROFILE/*DIGCF_PRESENT*/); // 目前存在的设备

	// 失败...
	if(hDevInfoSet == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	nCount = 0;
	bResult = TRUE;

	SP_DEVINFO_DATA				info				= {0};
	CHAR						szProp[1024]		= {0};	
	DWORD						dwPropSize			= 0;
	DWORD						dwType				= 0;
	BOOL						bRes				= FALSE;
	NETADAPTER					adapterData;

	ifdata.cbSize = sizeof(ifdata);
	info.cbSize = sizeof(info);
	// 设备序号=0,1,2... 逐一测试设备接口，到失败为止
	for(int i = 0
		; ::SetupDiEnumDeviceInfo(hDevInfoSet, (DWORD)i, &info)
		; i++)
	{
// 		memset(szProp, 0, sizeof(szProp));
// 		dwPropSize = sizeof(szProp);
// 		bRes = SetupDiGetDeviceRegistryPropertyA(hDevInfoSet, &info, SPDRP_PHYSICAL_DEVICE_OBJECT_NAME
// 			, &dwType, (BYTE *)szProp, dwPropSize, &dwPropSize);
// 
// 		std::wstring		pdoName;// = szProp;
// 		GetAdapterInfo(&adapterData, pdoName.c_str());
// 		continue;
		memset(szProp, 0, sizeof(szProp));
		dwPropSize = sizeof(szProp);
		bRes = SetupDiGetDeviceRegistryPropertyA(hDevInfoSet, &info, SPDRP_ADDRESS
			, &dwType, (BYTE *)szProp, dwPropSize, &dwPropSize);

		memset(szProp, 0, sizeof(szProp));
		dwPropSize = sizeof(szProp);
		bRes = SetupDiGetDeviceRegistryPropertyA(hDevInfoSet, &info, SPDRP_BUSTYPEGUID
			, &dwType, (BYTE *)szProp, dwPropSize, &dwPropSize);

		memset(szProp, 0, sizeof(szProp));
		dwPropSize = sizeof(szProp);
		bRes = SetupDiGetDeviceRegistryPropertyA(hDevInfoSet, &info, SPDRP_CLASSGUID  
			, &dwType, (BYTE *)szProp, dwPropSize, &dwPropSize);

		memset(szProp, 0, sizeof(szProp));
		dwPropSize = sizeof(szProp);
		bRes = SetupDiGetDeviceRegistryPropertyA(hDevInfoSet, &info, SPDRP_SERVICE
			, &dwType, (BYTE *)szProp, dwPropSize, &dwPropSize);

		memset(szProp, 0, sizeof(szProp));
		dwPropSize = sizeof(szProp);
		bRes = SetupDiGetDeviceRegistryPropertyA(hDevInfoSet, &info, SPDRP_UPPERFILTERS
			, &dwType, (BYTE *)szProp, dwPropSize, &dwPropSize);

		memset(szProp, 0, sizeof(szProp));
		dwPropSize = sizeof(szProp);
		bRes = SetupDiGetDeviceRegistryPropertyA(hDevInfoSet, &info, SPDRP_BUSNUMBER 
			, &dwType, (BYTE *)szProp, dwPropSize, &dwPropSize);

		memset(szProp, 0, sizeof(szProp));
		dwPropSize = sizeof(szProp);
		bRes = SetupDiGetDeviceRegistryPropertyA(hDevInfoSet, &info, SPDRP_DEVTYPE
			, &dwType, (BYTE *)szProp, dwPropSize, &dwPropSize);

		memset(szProp, 0, sizeof(szProp));
		dwPropSize = sizeof(szProp);
		bRes = SetupDiGetDeviceRegistryPropertyA(hDevInfoSet, &info, SPDRP_PHYSICAL_DEVICE_OBJECT_NAME
			, &dwType, (BYTE *)szProp, dwPropSize, &dwPropSize);

		memset(szProp, 0, sizeof(szProp));
		dwPropSize = sizeof(szProp);
		bRes = SetupDiGetDeviceRegistryPropertyA(hDevInfoSet, &info, SPDRP_FRIENDLYNAME
			, &dwType, (BYTE *)szProp, dwPropSize, &dwPropSize);
		// 得到类型，看是否为802.3以太网
		memset(szProp, 0, sizeof(szProp));
		dwPropSize = sizeof(szProp);
		bRes = SetupDiGetDeviceRegistryPropertyA(hDevInfoSet, &info, SPDRP_CHARACTERISTICS 
			, &dwType, (BYTE *)szProp, dwPropSize, &dwPropSize);
// 		if(FALSE == bRes || 0 != (DWORD)szProp)
// 			continue;

		memset(szProp, 0, sizeof(szProp));
		dwPropSize = sizeof(szProp);
		SetupDiGetDeviceRegistryPropertyA(hDevInfoSet, &info, SPDRP_DEVICEDESC,
			&dwType, (BYTE *)szProp, dwPropSize, &dwPropSize);

		szProp[0] = 0;
		dwPropSize = sizeof(szProp);
		SetupDiGetDeviceRegistryProperty(hDevInfoSet, &info,  SPDRP_HARDWAREID,
			&dwType, (BYTE *)szProp, dwPropSize, &dwPropSize);
		// 		OneCtrl.sHardId = szProp;

// 		SP_DEVICE_INTERFACE_DETAIL_DATA*			pDetail		= (SP_DEVICE_INTERFACE_DETAIL_DATA*)szProp;
// 
// 		memset(szProp, 0, sizeof(szProp));
// 		pDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
// 		ifdata.cbSize = sizeof(ifdata);
// 		SetupDiGetDeviceInterfaceDetailA(hDevInfoSet, &ifdata
// 			, pDetail, sizeof(szProp), NULL, NULL);
		int ii = 0;
// 		szProp[0] = 0;
// 		pDetail = (SP_DEVICE_INTERFACE_DETAIL_DATA_A*)szProp;
// 		pDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA_A);
// 		SetupDiGetDeviceInterfaceDetailA(hDevInfoSet, &ifdata, pDetail, sizeof(szProp), NULL, NULL);
	}

	SetupDiDestroyDeviceInfoList(hDevInfoSet);
	return TRUE;
}

BOOL CNetAdapterMgr::GetAdapterInfo(PNETADAPTER pAdapter, LPCWSTR lpPDOName)
{
// 	DRIVER_OBJECT			pdo;
// 	CHAR					szBuff[1024]	= {0};
// 	LONG					lSize			= sizeof(szBuff);
// 
// 	pdo.DriverName = lpPDOName;
// 	IoGetDeviceProperty(&pdo, DevicePropertyDriverKeyName
// 		, sizeof(szBuff), szBuff, &lSize);
	SetupDiOpenClassRegKey(NULL, NULL);
	SetupDiGetDeviceRegistryPropertyW(0, 0, 0, 0, 0, 0, 0);
	return TRUE;
}
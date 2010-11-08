#include "StdAfx.h"
#include ".\usbcontrol.h"

#include <Setupapi.h>
#pragma comment(lib, "Setupapi.lib")

GUID	g_usbGUID = {0xA5DCBF10L, 0x6530, 0x11D2, 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED};

#if defined(UNICODE) || defined(_UNICODE)
#error "Can not complate in unicode.\r\n"
#endif

//////////////////////////////////////////////////////////////////////////
CUSBControl::CUSBControl(void)
{

}

CUSBControl::~CUSBControl(void)
{
}

BOOL CUSBControl::EnableUSB(GUID& guid, BOOL bEnable, _CtrlFUN fnCtrl)
{
	ASSERT( fnCtrl != NULL);
	DWORD				NewState;
	DWORD				i;
	HDEVINFO			hDevInfo;
	SP_DEVINFO_DATA		DevInfoData;
	BOOL				bRetVal;
	int					nError;
	BOOL				bRet			= FALSE;

	SP_PROPCHANGE_PARAMS	PropChangeParams;

	nError = ERROR_SUCCESS;

	if(bEnable == FALSE)
	{
		NewState = DICS_DISABLE;//禁用
	}
	else
	{
		NewState = DICS_ENABLE;//启用
	}

	//获取对象集合
	hDevInfo = SetupDiGetClassDevs(&guid, NULL, NULL, DIGCF_PRESENT|DIGCF_INTERFACEDEVICE);
	if(INVALID_HANDLE_VALUE == hDevInfo)
	{
		return FALSE;
	}

	//枚举接口
	memset( &DevInfoData, 0, sizeof(DevInfoData));
	DevInfoData.cbSize = sizeof(DevInfoData);

	SP_DEVICE_INTERFACE_DATA		deviceInfoData = {0};
	deviceInfoData.cbSize			=sizeof(deviceInfoData);  

	for(i=0; 
		SetupDiEnumDeviceInfo(hDevInfo, i, &DevInfoData);
		i++)
	{
		BYTE			bzProp[1024];
		DWORD			dwPropSize;
		DWORD			dwType;

		dwPropSize = sizeof(bzProp);
		bRetVal = SetupDiGetDeviceRegistryProperty(hDevInfo, &DevInfoData, SPDRP_DEVICEDESC,
			&dwType, bzProp, dwPropSize, &dwPropSize);
		if(bRetVal == FALSE)
		{
			continue;
		}

		if(!(this->*fnCtrl)(bzProp))
			continue;
		//禁用该设备
		PropChangeParams.ClassInstallHeader.cbSize = sizeof(SP_CLASSINSTALL_HEADER);
		PropChangeParams.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE;
		PropChangeParams.Scope = DICS_FLAG_GLOBAL;
		PropChangeParams.StateChange = NewState;//禁用：DICS_DISABLE，DICS_ENABLE启用

		bRetVal = SetupDiSetClassInstallParams(hDevInfo, &DevInfoData,
			(SP_CLASSINSTALL_HEADER *)&PropChangeParams, sizeof(PropChangeParams));
		if(bRetVal == FALSE)
		{
			nError = GetLastError();
			continue;
		}

		bRetVal = SetupDiCallClassInstaller(DIF_PROPERTYCHANGE, hDevInfo, &DevInfoData);
		if(bRetVal == FALSE)
		{
			nError = GetLastError();
			continue;
		}
		bRet = TRUE;
	}


	SetupDiDestroyDeviceInfoList(hDevInfo);
	SetLastError(nError);

	return bRet;
}

// 是否为磁盘驱动器
BOOL CUSBControl::IsUSBStorage(VOID* pDisplay)
{
	CHAR		szDispaly[1024];

	memset(szDispaly, 0, sizeof(szDispaly));
	strncpy(szDispaly, strupr((CHAR*)pDisplay), 1024);
	if(NULL == strstr(szDispaly, "USB"))
		return FALSE;
	if(NULL == strstr(szDispaly, "STORAGE") 
		&& NULL == strstr(szDispaly, "DISK"))
		return FALSE;

	return TRUE;
}

// 禁用磁盘驱动器
BOOL CUSBControl::EnableUSBStorage(BOOL bEnable)
{
	return EnableUSB(g_usbGUID, bEnable, CUSBControl::IsUSBStorage);
}
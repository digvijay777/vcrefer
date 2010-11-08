#include "stdafx.h"
#include "USB.h"
#include <devguid.h>

#pragma comment(lib, "Setupapi.lib")

// 弹出U盘
BOOL usb::EjectUdisk(CHAR cDisk, int nWaitTime /* = 1000 */)
{
	cDisk &= ~0x20;	// 转化为大写
	if ( cDisk < 'A' || cDisk > 'Z' ) 
	{
		SetLastError(ERROR_PATH_NOT_FOUND);
		return FALSE;
	}

	char		szVolumeAccessPath[]		= {"\\\\.\\X:"};
	char		szRootPath[]				= {"X:\\"};
	long		DiskNumber					= -1;
	HANDLE		hVolume;

	szVolumeAccessPath[4] = cDisk;
	szRootPath[0] = cDisk;
	// 获取Device Number
	STORAGE_DEVICE_NUMBER		sdn						= {0};
	DWORD						dwBytesReturned			= 0;
	long						res						= 0;

	hVolume = CreateFile(szVolumeAccessPath, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
	if (hVolume == INVALID_HANDLE_VALUE) 
		return FALSE;
	res = DeviceIoControl(hVolume, IOCTL_STORAGE_GET_DEVICE_NUMBER, NULL, 0, &sdn, sizeof(sdn), &dwBytesReturned, NULL);
	if (res) 
		DiskNumber = sdn.DeviceNumber;
	CloseHandle(hVolume);
	if ( -1 == DiskNumber ) 
	{
		SetLastError(ERROR_ACCESS_DENIED);
		return FALSE;
	}

	// 获取DEVINST
	UINT			DriveType	= GetDriveType(szRootPath);
	DEVINST			DevInst		= GetDrivesDevInstByDiskNumber(DiskNumber, DriveType);

	if (0 == DevInst)
	{
		SetLastError(ERROR_ACCESS_DENIED);
		return FALSE;
	}

	// 禁用设备
	ULONG			Status					= 0;
	ULONG			ProblemNumber			= 0;
	PNP_VETO_TYPE	VetoType				= PNP_VetoTypeUnknown; 
	WCHAR			VetoNameW[MAX_PATH]		= {0};
	BOOL			bSuccess				= FALSE;
	BOOL			IsRemovable				= FALSE;

	res = CM_Get_Parent(&DevInst, DevInst, 0);  // disk's parent, e.g. the USB bridge, the SATA port....
	res = CM_Get_DevNode_Status(&Status, &ProblemNumber, DevInst, 0);
	IsRemovable = ((Status & DN_REMOVABLE) != 0);
	for (int tries = nWaitTime / 200; tries >= 0; tries--) 
	{ 
		VetoNameW[0] = 0;
		if ( IsRemovable ) 
		{
			res = CM_Request_Device_EjectW(DevInst, &VetoType, VetoNameW, sizeof(VetoNameW), 0); 
		} 
		else 
		{
			res = CM_Query_And_Remove_SubTreeW(DevInst, &VetoType, VetoNameW, sizeof(VetoNameW), 0); // CM_Query_And_Remove_SubTreeA is not implemented under W2K!
		}
		bSuccess = (CR_SUCCESS == res && PNP_VetoTypeUnknown == VetoType);
		if ( bSuccess )  
			break;

		Sleep(200);
	}

	return bSuccess;
}

DEVINST usb::GetDrivesDevInstByDiskNumber(long DiskNumber, UINT DriveType) 
{
	GUID*				guid;
	DEVINST				retDevInst		= 0;

	switch (DriveType)
	{
	case DRIVE_REMOVABLE:
		//break;
	case DRIVE_FIXED:
		guid = (GUID*)(void*)&GUID_DEVINTERFACE_DISK;
		break;
	case DRIVE_CDROM:
		guid = (GUID*)(void*)&GUID_DEVINTERFACE_CDROM;
		break;
	default:
		return 0;
	}

	// Get device interface info set handle for all devices attached to system
	HDEVINFO hDevInfo = SetupDiGetClassDevs(guid, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

	if (hDevInfo == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	SP_DEVICE_INTERFACE_DATA			devInterfaceData		= {0};
	BOOL								bRet					= FALSE;
	BYTE								Buf[1024];
	PSP_DEVICE_INTERFACE_DETAIL_DATA	pspdidd = (PSP_DEVICE_INTERFACE_DETAIL_DATA)Buf;
	SP_DEVICE_INTERFACE_DATA			spdid;
	SP_DEVINFO_DATA						spdd;
	DWORD								dwSize;

	devInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
	spdid.cbSize = sizeof(spdid);
	for(DWORD dwIndex = 0; TRUE; dwIndex++)
	{
		bRet = SetupDiEnumDeviceInterfaces(hDevInfo, NULL, guid, dwIndex, &devInterfaceData);
		if (!bRet)
			break;

		SetupDiEnumInterfaceDevice(hDevInfo, NULL, guid, dwIndex, &spdid);
		dwSize = 0;
		SetupDiGetDeviceInterfaceDetail(hDevInfo, &spdid, NULL, 0, &dwSize, NULL);
		if ( dwSize!=0 && dwSize<=sizeof(Buf) ) 
		{
			pspdidd->cbSize = sizeof(*pspdidd); // 5 Bytes!

			ZeroMemory((PVOID)&spdd, sizeof(spdd));
			spdd.cbSize = sizeof(spdd);

			long res = SetupDiGetDeviceInterfaceDetail(hDevInfo, &spdid, pspdidd, dwSize, &dwSize, &spdd);
			if ( res ) 
			{
				HANDLE hDrive = CreateFile(pspdidd->DevicePath, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
				if ( hDrive != INVALID_HANDLE_VALUE ) 
				{
					STORAGE_DEVICE_NUMBER sdn;
					DWORD dwBytesReturned = 0;
					res = DeviceIoControl(hDrive, IOCTL_STORAGE_GET_DEVICE_NUMBER, NULL, 0, &sdn, sizeof(sdn), &dwBytesReturned, NULL);
					CloseHandle(hDrive);
					if (res && (DiskNumber == (long)sdn.DeviceNumber))
					{
						retDevInst = spdd.DevInst;
						break;
					}
				}
			}
		}
	}

	SetupDiDestroyDeviceInfoList(hDevInfo);
	return retDevInst;
}
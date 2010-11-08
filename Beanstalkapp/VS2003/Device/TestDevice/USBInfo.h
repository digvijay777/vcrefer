#pragma once
#include <Shlwapi.h>
#include "usbhead.h"
#include "usbioctl.h"
#include "usbscan.h"

typedef struct _STORAGE_IDENTIFICATION {
	DWORD dwSize;
	DWORD dwFlags;
	DWORD dwManufactureIDOffset;
	DWORD dwSerialNumOffset;
} STORAGE_IDENTIFICATION, *PSTORAGE_IDENTIFICATION;

const GUID DiskClassGuid = {0x53f56307L, 0xb6bf, 0x11d0, {0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b}};

BOOL GetDisksProperty(HANDLE hDevice, PSTORAGE_DEVICE_DESCRIPTOR pDevDesc)
{
	STORAGE_PROPERTY_QUERY	Query;	
	DWORD dwOutBytes;				
	BOOL bResult		= FALSE;					

	Query.PropertyId =  StorageDeviceProperty;
	Query.QueryType = PropertyStandardQuery;

	bResult = ::DeviceIoControl(hDevice,			
		IOCTL_STORAGE_QUERY_PROPERTY,			
		&Query, sizeof(STORAGE_PROPERTY_QUERY),	
		pDevDesc, pDevDesc->Size,				
		&dwOutBytes,							
		(LPOVERLAPPED)NULL);		

	USB_DESCRIPTOR_REQUEST *	pDevReq;
	BYTE						bzData[1024];
	CHAR						szText[1024];
	BOOL						bRetVal;
	DWORD						dwRead;
	USB_STRING_DESCRIPTOR *		pDevDescripRead;

	memset(bzData, 0, sizeof(bzData));
	pDevReq = (USB_DESCRIPTOR_REQUEST *)bzData;
	pDevDescripRead = (USB_STRING_DESCRIPTOR *)(bzData + sizeof(USB_DESCRIPTOR_REQUEST));

	pDevReq->ConnectionIndex = 2;
	pDevReq->SetupPacket.wValue = (USB_STRING_DESCRIPTOR_TYPE << 8) | 3;
	pDevReq->SetupPacket.wIndex = 1003;
	pDevReq->SetupPacket.wLength = 0xFF;

	bRetVal = DeviceIoControl(hDevice,  IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION, 
		pDevReq, sizeof(bzData), pDevReq, sizeof(bzData), &dwRead, NULL);

// 	DWORD   dwError,dwSize,dwInDataSize;
// 	CString   strInfo;
// 	USBSCAN_GET_DESCRIPTOR   UsbscanGetDescriptor   =   {   0   };
// // 	USB_CONFIGURATION_DESCRIPTOR   UsbDeviceDescriptor   =   {   0   };
// 	USB_DEVICE_DESCRIPTOR	UsbDeviceDescriptor = {0};
// 
// 	dwSize   =   sizeof   (   UsbDeviceDescriptor   );
// 	UsbscanGetDescriptor.DescriptorType   =   USB_DEVICE_DESCRIPTOR_TYPE;
// 	UsbscanGetDescriptor.Index   =   1;
// 	UsbscanGetDescriptor.LanguageId   =   0;
// 
// 	dwError   =     (   DWORD   )   DeviceIoControl   (   hDevice,   IOCTL_GET_USB_DESCRIPTOR,   &UsbscanGetDescriptor,  
// 		sizeof(UsbscanGetDescriptor),  
// 		&UsbDeviceDescriptor,  
// 		dwSize,   &dwInDataSize,  
// 		NULL   );

// 	STORAGE_DEVICE_NUMBER sdn;
// 	DWORD dwBytesReturned = 0;
// 	//通過這樣一個句柄，用同樣的方法也可以得到一個設備號。
// 	bResult = DeviceIoControl(hDevice, IOCTL_STORAGE_GET_DEVICE_NUMBER,
// 		NULL, 0, &sdn, sizeof(sdn),
// 		&dwBytesReturned, NULL);
// 	return FALSE;

/*	DeviceIoControl(hDevice, IOCTL_DISK_GET_STORAGEID)*/

// 	BYTE tmp_buf1[513] = {0};   
// 	BYTE tmp_buf2[513] = {0};
// 	DWORD data_len;
// 
// 	USB_NODE_CONNECTION_INFORMATION_EX* conn_info = (USB_NODE_CONNECTION_INFORMATION_EX*)tmp_buf1;
// 	conn_info->ConnectionIndex = port_idx;
// 
// 	if(! DeviceIoControl(hDevice, IOCTL_USB_GET_NODE_CONNECTION_INFORMATION_EX, conn_info, 513, conn_info, 513, &data_len, 0))
// 	{
// 		return false;
// 	} 
// 	USB_NODE_INFORMATION *	pNodInfo;
// 	BYTE				bzData[1024];
// 	DWORD				dwRead;
// 
// 	pNodInfo = (USB_NODE_INFORMATION *)bzData;
// 	DeviceIoControl(hDevice, IOCTL_USB_GET_NODE_INFORMATION, NULL, 0,
// 		pNodInfo, 1024, &dwRead, NULL);


	return bResult;
}

CString GetDiskInfo(LPSTR lpRoot)
{
	CHAR		szPath[MAX_PATH]	= {0};
	HANDLE		hDevice;

	strcpy(szPath, "\\\\?\\");
	strcat(szPath, lpRoot);

	hDevice = CreateFile(szPath, GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
	if(INVALID_HANDLE_VALUE == hDevice)
		return "";

	CString							strRet;
	PSTORAGE_DEVICE_DESCRIPTOR		pDevDesc;

	pDevDesc = (PSTORAGE_DEVICE_DESCRIPTOR)new BYTE[sizeof(STORAGE_DEVICE_DESCRIPTOR) + 1024 - 1];

	pDevDesc->Size = sizeof(STORAGE_DEVICE_DESCRIPTOR) + 1024 - 1;

	if(GetDisksProperty(hDevice, pDevDesc))
	{
		char* p;
		strRet += "\r\n找到U盘，名称: ";
		p = (char*)pDevDesc;
		strRet += (pDevDesc->VendorIdOffset ? &p[pDevDesc->VendorIdOffset]:"(NULL)");
		
		strRet += ("\r\n产品ID: ");
		strRet += (pDevDesc->ProductIdOffset ? &p[pDevDesc->ProductIdOffset]:"(NULL)");
		strRet += ("\r\n产品版本: ");
		strRet += (pDevDesc->ProductRevisionOffset ? &p[pDevDesc->ProductRevisionOffset] : "(NULL)");
		strRet += ("\r\n序列号: ");
		strRet += (pDevDesc->SerialNumberOffset ? &p[pDevDesc->SerialNumberOffset] : "(NULL)");
	}

	delete pDevDesc;

	CloseHandle(hDevice);
	return strRet;
}

#define INTERFACE_DETAIL_SIZE (1024)

int GetDevicePath(LPGUID lpGuid, LPTSTR* pszDevicePath)
{
	HDEVINFO hDevInfoSet;
	SP_DEVICE_INTERFACE_DATA ifdata;
	PSP_DEVICE_INTERFACE_DETAIL_DATA pDetail;
	int nCount;
	BOOL bResult;

	// 取得一个该GUID相关的设备信息集句柄
	hDevInfoSet = ::SetupDiGetClassDevs(lpGuid,  // class GUID
		NULL,     // 无关键字
		NULL,     // 不指定父窗口句柄
		DIGCF_PRESENT | DIGCF_DEVICEINTERFACE); // 目前存在的设备

	// 失败...
	if(hDevInfoSet == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	// 申请设备接口数据空间
	pDetail = (PSP_DEVICE_INTERFACE_DETAIL_DATA)::GlobalAlloc(LMEM_ZEROINIT, INTERFACE_DETAIL_SIZE);

	pDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

	nCount = 0;
	bResult = TRUE;

	// 设备序号=0,1,2... 逐一测试设备接口，到失败为止
	while (bResult)
	{
		ifdata.cbSize=sizeof(ifdata);

		// 枚举符合该GUID的设备接口
		bResult = ::SetupDiEnumDeviceInterfaces(
			hDevInfoSet,  // 设备信息集句柄
			NULL,   // 不需额外的设备描述
			lpGuid,   // GUID
			(ULONG)nCount,  // 设备信息集里的设备序号
			&ifdata);  // 设备接口信息

		if(bResult)
		{
			// 取得该设备接口的细节(设备路径)
			bResult = SetupDiGetInterfaceDeviceDetail(
				hDevInfoSet,  // 设备信息集句柄
				&ifdata,  // 设备接口信息
				pDetail,  // 设备接口细节(设备路径)
				INTERFACE_DETAIL_SIZE, // 输出缓冲区大小
				NULL,   // 不需计算输出缓冲区大小(直接用设定值)
				NULL);   // 不需额外的设备描述

			if(bResult)
			{
				// 复制设备路径到输出缓冲区
				::strcpy(pszDevicePath[nCount], pDetail->DevicePath);

				// 调整计数值
				nCount++;
			}
		}
	}

	// 释放设备接口数据空间
	::GlobalFree(pDetail);

	// 关闭设备信息集句柄
	::SetupDiDestroyDeviceInfoList(hDevInfoSet);

	return nCount;
}
/*
				   
				   HANDLE   OpenDevice(LPGUID   pGuid,   char   *outNameBuf)
				   {
				   	HANDLE   hOut   =   INVALID_HANDLE_VALUE;
				   	HDEVINFO   hardwareDeviceInfo;
				   	SP_INTERFACE_DEVICE_DATA   deviceInfoData;
				   
				   	hardwareDeviceInfo   =   SetupDiGetClassDevs   (pGuid,   NULL,   NULL,   (DIGCF_PRESENT   |   DIGCF_INTERFACEDEVICE));
				   
				   	deviceInfoData.cbSize   =   sizeof   (SP_INTERFACE_DEVICE_DATA);
				   
				   	if   (SetupDiEnumDeviceInterfaces   (hardwareDeviceInfo,   0,   pGuid,   0,   &deviceInfoData))
				   	{
				   		hOut   =   OpenOneDevice   (hardwareDeviceInfo,   &deviceInfoData,   outNameBuf);
				   
				   		if   (hOut==INVALID_HANDLE_VALUE)
				   			return   INVALID_HANDLE_VALUE;
				   	}
				   
				   	DWORD   reErr   =   GetLastError(   );
				   
				   	SetupDiDestroyDeviceInfoList   (hardwareDeviceInfo);
				   
				   	return   hOut;
				   }
				   
				   HANDLE   OpenOneDevice(HDEVINFO   HardwareDeviceInfo,   PSP_INTERFACE_DEVICE_DATA   DeviceInfoData,   char   *devName)
				   {
				   	PSP_INTERFACE_DEVICE_DETAIL_DATA   functionClassDeviceData   =   NULL;
				   	ULONG   predictedLength   =   0;
				   	ULONG   requiredLength   =   0;
				   	HANDLE   hOut   =   INVALID_HANDLE_VALUE;
				   
				   	//   allocate   a   function   class   device   data   structure   to   receive   the   goods   about   this   particular   device.
				   	SetupDiGetInterfaceDeviceDetail   (HardwareDeviceInfo,   DeviceInfoData,   NULL,   0,   &requiredLength,   NULL);
				   
				   	predictedLength   =   requiredLength;
				   	//   sizeof   (SP_FNCLASS_DEVICE_DATA)   +   512;
				   
				   	functionClassDeviceData   =   (PSP_INTERFACE_DEVICE_DETAIL_DATA)malloc   (predictedLength);
				   	functionClassDeviceData-> cbSize   =   sizeof   (SP_INTERFACE_DEVICE_DETAIL_DATA);
				   
				   	//   Retrieve   the   information   from   Plug   and   Play.
				   	if   (!SetupDiGetInterfaceDeviceDetail(
				   		HardwareDeviceInfo,
				   		DeviceInfoData,
				   		functionClassDeviceData,
				   		predictedLength,
				   		&requiredLength,
				   		NULL))
				   	{
				   		free(functionClassDeviceData);
				   		return   INVALID_HANDLE_VALUE;
				   	}
				   
				   	strcpy(devName,functionClassDeviceData-> DevicePath);
				   	printf( "Attempting   to   open   %s\n ",   devName);
				   
				   	hOut   =   CreateFile   (functionClassDeviceData-> DevicePath,
				   		GENERIC_READ   |   GENERIC_WRITE,
				   		FILE_SHARE_READ   |   FILE_SHARE_WRITE,
				   		NULL,
				   		OPEN_EXISTING,
				   		0,
				   		NULL);
				   
				   	if   (INVALID_HANDLE_VALUE   ==   hOut)
				   		printf( "FAILED   to   open   %s\n ",   devName);
				   
				   	free(functionClassDeviceData);
				   
				   	return   hOut;
				   }
				   
				   
				   HANDLE   hDEV   =   OpenDevice((LPGUID)&GUID_BUS_UMSS,   completeDeviceName);
				   if   (hDEV==INVALID_HANDLE_VALUE)
				   return;
				   
				   DeviceIoControl   (
				   				   hDEV,
				   				   IOCTL_Vender_VCMD_WRITE,
				   				   buf,
				   				   31,   //CBW
				   				   buf,
				   				   13,   //   Data   +   CSW
				   				   (LPDWORD)&nBytes,
				   				   NULL
				   				   ); */
				   
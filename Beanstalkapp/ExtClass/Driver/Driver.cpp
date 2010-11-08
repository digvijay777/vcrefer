#include "stdafx.h"
#include "Driver.h"

#if WINVER < 0x500 || _WIN32_WINNT < 0x500 || _WIN32_WINDOWS < 0x510
#error "Windows version is low."
#endif
#include <Setupapi.h>

#pragma comment(lib, "Setupapi.lib")

#define INTERFACE_DETAIL_SIZE			1024
//////////////////////////////////////////////////////////////////////////
/*
 *	获取设备路径
 */
int XDriver::GetDevicePath2(LPGUID lpGuid, std::vector<std::string>& vctPath)
{
	int nCount = 0;

	return nCount;
}

/*
*	获取设备路径
*/
int XDriver::GetDevicePath(LPGUID lpGuid, std::vector<std::string>& vctPath, int nMaxCount)
{
	HDEVINFO							hDevInfoSet		= NULL;
	SP_DEVICE_INTERFACE_DATA			ifdata			= {0};
	PSP_DEVICE_INTERFACE_DETAIL_DATA	pDetail			= NULL;
	int									nCount			= 0;
	BOOL								bResult			= FALSE;

	// 取得一个该GUID相关的设备信息集句柄
	hDevInfoSet = ::SetupDiGetClassDevs(lpGuid,  // class GUID
		NULL,     // 无关键字
		NULL,     // 不指定父窗口句柄
		DIGCF_PROFILE/*DIGCF_PRESENT*/); // 目前存在的设备

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
	while (nMaxCount > 0)
	{
		ifdata.cbSize=sizeof(ifdata);

		// 		// 枚举符合该GUID的设备接口
		bResult = ::SetupDiEnumDeviceInterfaces(
			hDevInfoSet,  // 设备信息集句柄
			NULL,   // 不需额外的设备描述
			lpGuid,   // GUID
			(ULONG)nMaxCount,  // 设备信息集里的设备序号
			&ifdata);  // 设备接口信息

		nMaxCount--;
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
				// ::strcpy(pszDevicePath[nCount], pDetail->DevicePath);
				vctPath.push_back(pDetail->DevicePath);
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
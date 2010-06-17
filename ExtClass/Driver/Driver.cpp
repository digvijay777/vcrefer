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
 *	��ȡ�豸·��
 */
int XDriver::GetDevicePath2(LPGUID lpGuid, std::vector<std::string>& vctPath)
{
	int nCount = 0;

	return nCount;
}

/*
*	��ȡ�豸·��
*/
int XDriver::GetDevicePath(LPGUID lpGuid, std::vector<std::string>& vctPath, int nMaxCount)
{
	HDEVINFO							hDevInfoSet		= NULL;
	SP_DEVICE_INTERFACE_DATA			ifdata			= {0};
	PSP_DEVICE_INTERFACE_DETAIL_DATA	pDetail			= NULL;
	int									nCount			= 0;
	BOOL								bResult			= FALSE;

	// ȡ��һ����GUID��ص��豸��Ϣ�����
	hDevInfoSet = ::SetupDiGetClassDevs(lpGuid,  // class GUID
		NULL,     // �޹ؼ���
		NULL,     // ��ָ�������ھ��
		DIGCF_PROFILE/*DIGCF_PRESENT*/); // Ŀǰ���ڵ��豸

	// ʧ��...
	if(hDevInfoSet == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	// �����豸�ӿ����ݿռ�
	pDetail = (PSP_DEVICE_INTERFACE_DETAIL_DATA)::GlobalAlloc(LMEM_ZEROINIT, INTERFACE_DETAIL_SIZE);

	pDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

	nCount = 0;
	bResult = TRUE;

	// �豸���=0,1,2... ��һ�����豸�ӿڣ���ʧ��Ϊֹ
	while (nMaxCount > 0)
	{
		ifdata.cbSize=sizeof(ifdata);

		// 		// ö�ٷ��ϸ�GUID���豸�ӿ�
		bResult = ::SetupDiEnumDeviceInterfaces(
			hDevInfoSet,  // �豸��Ϣ�����
			NULL,   // ���������豸����
			lpGuid,   // GUID
			(ULONG)nMaxCount,  // �豸��Ϣ������豸���
			&ifdata);  // �豸�ӿ���Ϣ

		nMaxCount--;
		if(bResult)
		{
			// ȡ�ø��豸�ӿڵ�ϸ��(�豸·��)
			bResult = SetupDiGetInterfaceDeviceDetail(
				hDevInfoSet,  // �豸��Ϣ�����
				&ifdata,  // �豸�ӿ���Ϣ
				pDetail,  // �豸�ӿ�ϸ��(�豸·��)
				INTERFACE_DETAIL_SIZE, // �����������С
				NULL,   // ������������������С(ֱ�����趨ֵ)
				NULL);   // ���������豸����

			if(bResult)
			{
				// �����豸·�������������
				// ::strcpy(pszDevicePath[nCount], pDetail->DevicePath);
				vctPath.push_back(pDetail->DevicePath);
				// ��������ֵ
				nCount++;
			}
		}
	}

	// �ͷ��豸�ӿ����ݿռ�
	::GlobalFree(pDetail);

	// �ر��豸��Ϣ�����
	::SetupDiDestroyDeviceInfoList(hDevInfoSet);

	return nCount;
}
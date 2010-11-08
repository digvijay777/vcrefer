// DetourHTTP.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include <afxdllx.h>
#include "HTTPDetours.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE DetourHTTPDLL = { NULL, NULL };
CHTTPDetours		g_httpDetours;

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// ���ʹ�� lpReserved���뽫���Ƴ�
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("DetourHTTP.DLL ���ڳ�ʼ����\n");
		
		// ��չ DLL һ���Գ�ʼ��
		if (!AfxInitExtensionModule(DetourHTTPDLL, hInstance))
			return 0;

		// ���� DLL ���뵽��Դ����
		// ע��: �������չ DLL ��
		//  MFC ���� DLL(�� ActiveX �ؼ�)��ʽ���ӵ�
		//  �������� MFC Ӧ�ó������ӵ�������Ҫ
		//  �����д� DllMain ���Ƴ������������һ��
		//  �Ӵ���չ DLL �����ĵ����ĺ����С�ʹ�ô���չ DLL ��
		//  ���� DLL Ȼ��Ӧ��ʽ
		//  ���øú����Գ�ʼ������չ DLL������
		//  CDynLinkLibrary ���󲻻ḽ�ӵ�
		//  ���� DLL ����Դ���������������ص�
		//  ���⡣

		new CDynLinkLibrary(DetourHTTPDLL);

		// �Զ������
		DisableThreadLibraryCalls(hInstance);

		g_httpDetours.AttachDetours();
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("DetourHTTP.DLL ������ֹ��\n");

		g_httpDetours.DetachDetours();
		// �ڵ�����������֮ǰ��ֹ�ÿ�
		AfxTermExtensionModule(DetourHTTPDLL);
	}
	return 1;   // ȷ��
}

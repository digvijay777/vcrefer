// DCOMServer.cpp : WinMain ��ʵ��

#include "stdafx.h"
#include "resource.h"
#include "DCOMServer.h"

class CDCOMServerModule : public CAtlServiceModuleT< CDCOMServerModule, IDS_SERVICENAME >
{
public :
	DECLARE_LIBID(LIBID_DCOMServerLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_DCOMSERVER, "{8C7B35A4-2698-4A14-A212-D6E5E763F6B4}")
	HRESULT InitializeSecurity() throw()
	{
		// TODO : ���� CoInitializeSecurity ��Ϊ�����ṩ�ʵ��� 
		// ��ȫ����
		// ���� - PKT ����������֤��
		// RPC_C_IMP_LEVEL_IDENTIFY ��ģ�⼶��
		// �Լ��ʵ��ķǿհ�ȫ˵������

		return S_OK;
	}
};

CDCOMServerModule _AtlModule;


//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
                                LPTSTR /*lpCmdLine*/, int nShowCmd)
{
    return _AtlModule.WinMain(nShowCmd);
}


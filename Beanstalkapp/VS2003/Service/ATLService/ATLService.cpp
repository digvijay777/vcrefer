// ATLService.cpp : WinMain ��ʵ��

#include "stdafx.h"
#include "resource.h"
#include "ATLService.h"

class CATLServiceModule : public CAtlServiceModuleT< CATLServiceModule, IDS_SERVICENAME >
{
public :
	DECLARE_LIBID(LIBID_ATLServiceLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_ATLSERVICE, "{B434FC52-7041-4A5A-A996-9E24B37B563B}")
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

CATLServiceModule _AtlModule;


//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
                                LPTSTR /*lpCmdLine*/, int nShowCmd)
{
    return _AtlModule.WinMain(nShowCmd);
}


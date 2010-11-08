#include "stdafx.h"
#include "ComReg.h"
#include <shlwapi.h>

#pragma comment(lib, "shlwapi.lib")

// �û��Ƿ���޸�IE��ҳ
BOOL ComReg::IsIEStartPageEnabled()
{
	DWORD		dwType;
	DWORD		dwData		= 0;
	DWORD		dwSize		= sizeof(dwData);
	DWORD		dwRet;

	// ��Ȿ������
	dwRet = SHGetValue(HKEY_LOCAL_MACHINE
		, "Software\\Policies\\Microsoft\\Internet Explorer\\Control Panel"
		, "HomePage", &dwType, &dwData, &dwSize);
	if(ERROR_SUCCESS == dwRet && 1 == dwData && REG_DWORD == dwType)
		return FALSE;

	// ��⵱ǰ����
	dwRet = SHGetValue(HKEY_CURRENT_USER
		, "Software\\Policies\\Microsoft\\Internet Explorer\\Control Panel"
		, "HomePage", &dwType, &dwData, &dwSize);
	if(ERROR_SUCCESS == dwRet && 1 == dwData && REG_DWORD == dwType)
		return FALSE;

	return TRUE;
}

// �����û��޸�IE�Ĺ���
BOOL ComReg::SetIEStartPageEnabled(BOOL bEnabled /* = TRUE */)
{
	DWORD		dwData		= 0;
	DWORD		dwRet;
	BOOL		bRet		= FALSE;

	if(FALSE != bEnabled)
		dwData = 0;
	else
		dwData = 1;
	// ���ñ����û�
	dwRet = SHSetValue(HKEY_LOCAL_MACHINE
		, "Software\\Policies\\Microsoft\\Internet Explorer\\Control Panel"
		, "HomePage", REG_DWORD, &dwData, sizeof(dwData));
	if(ERROR_SUCCESS == dwRet)
		bRet = TRUE;
	
	// ���õ�ǰ�û�
	dwRet = SHSetValue(HKEY_CURRENT_USER
		, "Software\\Policies\\Microsoft\\Internet Explorer\\Control Panel"
		, "HomePage", REG_DWORD, &dwData, sizeof(dwData));
	if(ERROR_SUCCESS == dwRet)
		bRet = TRUE;

	return bRet;
}

// �Ƿ���������
BOOL ComReg::IsRestrictGpedit()
{
	DWORD		dwType;
	DWORD		dwData		= 0;
	DWORD		dwSize		= sizeof(dwData);
	DWORD		dwRet;

	// ��Ȿ������
	dwRet = SHGetValue(HKEY_CURRENT_USER
		, "Software\\Policies\\Microsoft\\MMC\\{8FC0B734-A0E1-11D1-A7D3-0000F87571E3}"
		, "Restrict_Run", &dwType, &dwData, &dwSize);
	if(ERROR_SUCCESS == dwRet && 1 == dwData && REG_DWORD == dwType)
		return FALSE;
	
	return TRUE;
}

BOOL ComReg::SetRestrictGpedit(BOOL bRun /* = TRUE */)
{
	DWORD		dwData		= 0;
	DWORD		dwRet;
	BOOL		bRet		= FALSE;

	if(FALSE != bRun)
		dwData = 0;
	else
		dwData = 1;
	// ���ñ����û�
	dwRet = SHSetValue(HKEY_CURRENT_USER
		, "Software\\Policies\\Microsoft\\MMC\\{8FC0B734-A0E1-11D1-A7D3-0000F87571E3}"
		, "Restrict_Run", REG_DWORD, &dwData, sizeof(dwData));
	if(ERROR_SUCCESS == dwRet)
		bRet = TRUE;

	return bRet;
}
// ��ֹע���
BOOL ComReg::IsRegistryToolEnabled()
{
	DWORD		dwType;
	DWORD		dwData		= 0;
	DWORD		dwSize		= sizeof(dwData);
	DWORD		dwRet;

	// ��Ȿ������
	dwRet = SHGetValue(HKEY_CURRENT_USER
		, "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System"
		, "DisableRegistryTools", &dwType, &dwData, &dwSize);
	if(ERROR_SUCCESS == dwRet && 1 == dwData && REG_DWORD == dwType)
		return FALSE;

	return TRUE;
}

BOOL ComReg::SetRegistryToolEnabled(BOOL bEnabled)
{
	DWORD		dwData		= 0;
	DWORD		dwRet;
	BOOL		bRet		= FALSE;

	if(FALSE != bEnabled)
		dwData = 0;
	else
		dwData = 1;
	// ���ñ����û�
	dwRet = SHSetValue(HKEY_CURRENT_USER
		, "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System"
		, "DisableRegistryTools", REG_DWORD, &dwData, sizeof(dwData));
	if(ERROR_SUCCESS == dwRet)
		bRet = TRUE;

	return bRet;
}
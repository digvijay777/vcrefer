#include "stdafx.h"
#include "ComReg.h"
#include <shlwapi.h>

#pragma comment(lib, "shlwapi.lib")

// 用户是否可修改IE首页
BOOL ComReg::IsIEStartPageEnabled()
{
	DWORD		dwType;
	DWORD		dwData		= 0;
	DWORD		dwSize		= sizeof(dwData);
	DWORD		dwRet;

	// 检测本地配置
	dwRet = SHGetValue(HKEY_LOCAL_MACHINE
		, "Software\\Policies\\Microsoft\\Internet Explorer\\Control Panel"
		, "HomePage", &dwType, &dwData, &dwSize);
	if(ERROR_SUCCESS == dwRet && 1 == dwData && REG_DWORD == dwType)
		return FALSE;

	// 检测当前配置
	dwRet = SHGetValue(HKEY_CURRENT_USER
		, "Software\\Policies\\Microsoft\\Internet Explorer\\Control Panel"
		, "HomePage", &dwType, &dwData, &dwSize);
	if(ERROR_SUCCESS == dwRet && 1 == dwData && REG_DWORD == dwType)
		return FALSE;

	return TRUE;
}

// 设置用户修复IE的功能
BOOL ComReg::SetIEStartPageEnabled(BOOL bEnabled /* = TRUE */)
{
	DWORD		dwData		= 0;
	DWORD		dwRet;
	BOOL		bRet		= FALSE;

	if(FALSE != bEnabled)
		dwData = 0;
	else
		dwData = 1;
	// 设置本地用户
	dwRet = SHSetValue(HKEY_LOCAL_MACHINE
		, "Software\\Policies\\Microsoft\\Internet Explorer\\Control Panel"
		, "HomePage", REG_DWORD, &dwData, sizeof(dwData));
	if(ERROR_SUCCESS == dwRet)
		bRet = TRUE;
	
	// 设置当前用户
	dwRet = SHSetValue(HKEY_CURRENT_USER
		, "Software\\Policies\\Microsoft\\Internet Explorer\\Control Panel"
		, "HomePage", REG_DWORD, &dwData, sizeof(dwData));
	if(ERROR_SUCCESS == dwRet)
		bRet = TRUE;

	return bRet;
}

// 是否禁用组策略
BOOL ComReg::IsRestrictGpedit()
{
	DWORD		dwType;
	DWORD		dwData		= 0;
	DWORD		dwSize		= sizeof(dwData);
	DWORD		dwRet;

	// 检测本地配置
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
	// 设置本地用户
	dwRet = SHSetValue(HKEY_CURRENT_USER
		, "Software\\Policies\\Microsoft\\MMC\\{8FC0B734-A0E1-11D1-A7D3-0000F87571E3}"
		, "Restrict_Run", REG_DWORD, &dwData, sizeof(dwData));
	if(ERROR_SUCCESS == dwRet)
		bRet = TRUE;

	return bRet;
}
// 禁止注册表
BOOL ComReg::IsRegistryToolEnabled()
{
	DWORD		dwType;
	DWORD		dwData		= 0;
	DWORD		dwSize		= sizeof(dwData);
	DWORD		dwRet;

	// 检测本地配置
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
	// 设置本地用户
	dwRet = SHSetValue(HKEY_CURRENT_USER
		, "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System"
		, "DisableRegistryTools", REG_DWORD, &dwData, sizeof(dwData));
	if(ERROR_SUCCESS == dwRet)
		bRet = TRUE;

	return bRet;
}
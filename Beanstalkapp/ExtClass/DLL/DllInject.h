#pragma once

namespace dll
{
	BOOL		CreateProcessWithInject(LPCTSTR lpExe, LPTSTR lpCommand, LPCSTR lpDll);
	BOOL		EnableDebugPrivilege();
};
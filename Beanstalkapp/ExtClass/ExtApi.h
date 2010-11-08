#pragma once

namespace ext
{
	// 带消息循环的等待
	DWORD			MessageLoop(HANDLE hHandle, DWORD dwMilliseconds);
	// 从DLL文件得到接口
	HRESULT			CreateInstanceFromDll(LPCTSTR lpDllName, REFCLSID rclsid, REFIID riid, LPVOID * ppv);
	// 释放没有使用DLL
	void			FreeUnusedLibraries(LPCTSTR lpDllName);

};

#pragma once

namespace ext
{
	// ����Ϣѭ���ĵȴ�
	DWORD			MessageLoop(HANDLE hHandle, DWORD dwMilliseconds);
	// ��DLL�ļ��õ��ӿ�
	HRESULT			CreateInstanceFromDll(LPCTSTR lpDllName, REFCLSID rclsid, REFIID riid, LPVOID * ppv);
	// �ͷ�û��ʹ��DLL
	void			FreeUnusedLibraries(LPCTSTR lpDllName);

};

#pragma once

namespace DCOM
{
	HRESULT			CreateInstance(REFCLSID rclsid, REFIID riid
		, LPCSTR lpSvrIP, LPCSTR lpUser, LPCSTR lpPasswd, void** ppUnknow);

}
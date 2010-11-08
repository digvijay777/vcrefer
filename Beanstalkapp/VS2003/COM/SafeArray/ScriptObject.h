#pragma once
#include <DispEx.h>

class CScriptObject
{
public:
	CScriptObject(VARIANT vScript);
	virtual ~CScriptObject(void);
private:
	CComPtr<IDispatchEx>		m_spDispEx;
	CComPtr<IDispatchEx>		m_spScript;

	HRESULT		AddMember(BSTR name, VARIANT* pVar);
	HRESULT		DeleteMember(BSTR name);
	HRESULT		GetMember(BSTR name, VARIANT* pVar);
	BOOL		CreateObject();
public:
	HRESULT		Set(BSTR name, VARIANT* pVar);
	HRESULT		Set(BSTR name, VARIANT& var);
	VARIANT		Get(BSTR name);
	HRESULT		GetInterface(IDispatchEx** ppDispEx);
};

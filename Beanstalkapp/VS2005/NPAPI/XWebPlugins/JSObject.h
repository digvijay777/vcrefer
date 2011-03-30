#pragma once
#include <DispEx.h>

class CJSObject
{
public:
	CJSObject(void);
	~CJSObject(void);
private:
	CComPtr<IDispatchEx>		m_spDispatchEx;

public:
	BOOL	AttachDispatch(IDispatch* pDispatch);
	BOOL	AttachDispatch(VARIANT var);

	// �õ�����
	BOOL	Get(BSTR name, VARIANT* pValue);
	BOOL	GetString(LPCSTR name, BSTR* pValue);
	BOOL	GetObject(LPCSTR name, CJSObject* pObj);
	// ö������
	BOOL	GetNextProputy(DISPID& dipid, BSTR* pName, BSTR* pValue);

};

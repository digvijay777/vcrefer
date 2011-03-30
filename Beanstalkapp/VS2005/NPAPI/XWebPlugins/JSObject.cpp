#include "StdAfx.h"
#include ".\jsobject.h"

CJSObject::CJSObject(void)
{
}

CJSObject::~CJSObject(void)
{
}

// �ҽ�Dispatch
BOOL CJSObject::AttachDispatch(VARIANT var)
{
	if(VT_DISPATCH != var.vt)
		return FALSE;
	if(m_spDispatchEx != NULL)
		m_spDispatchEx.Release();

	return AttachDispatch(var.pdispVal);
}
BOOL CJSObject::AttachDispatch(IDispatch* pDispatch)
{
	if(NULL == pDispatch)
		return FALSE;

	return SUCCEEDED(pDispatch->QueryInterface(IID_IDispatchEx, (void **)&m_spDispatchEx));
}

// ��ȡ����
BOOL CJSObject::Get(BSTR name, VARIANT* pValue)
{
	if(m_spDispatchEx == NULL)
		return FALSE;

	DISPID			pid;
	DISPPARAMS		dispparamsNoArgs	= {NULL, NULL, 0, 0};
	HRESULT			hres;

	// name ������BSTR�ִ�
	hres = m_spDispatchEx->GetDispID(name, DISPID_UNKNOWN, &pid);
	if(FAILED(hres))
		return FALSE;

	hres = m_spDispatchEx->InvokeEx(pid, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET
		, &dispparamsNoArgs, pValue, NULL, NULL);
	if(FAILED(hres))
		return FALSE;
	
	return TRUE;
}

// �õ��ַ���
BOOL CJSObject::GetString(LPCSTR name, BSTR* pValue)
{
	_bstr_t			sName		= name;
	_variant_t		var;

	if(FALSE == Get(sName.GetBSTR(), var.GetAddress()))
		return FALSE;

	*pValue = ((_bstr_t)var).Detach();
	return TRUE;
}

// �õ�JS����
BOOL CJSObject::GetObject(LPCSTR name, CJSObject* pObj)
{
	_bstr_t			sName		= name;
	_variant_t		var;
	BOOL			bret;

	if(FALSE == Get(sName.GetBSTR(), var.GetAddress()))
		return FALSE;

	if(VT_DISPATCH != var.vt)
		return FALSE;

	bret = pObj->AttachDispatch(var.pdispVal);
	var.Detach();
	return bret;
}

BOOL CJSObject::GetNextProputy(DISPID& dipid, BSTR* pName, BSTR* pValue)
{
	HRESULT			hres;

	do 
	{
		hres = m_spDispatchEx->GetNextDispID(fdexEnumDefault, dipid, &dipid);
		if(S_OK != hres)
			return FALSE;
		
		_variant_t		var;
		DISPPARAMS		dispparamsNoArgs	= {NULL, NULL, 0, 0};
		
		hres = m_spDispatchEx->InvokeEx(dipid, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET
			, &dispparamsNoArgs, var.GetAddress(), NULL, NULL);
		if(FAILED(hres))
			continue;
		if(VT_DISPATCH == var.vt)
			continue;
		// ����
		*pValue = ((_bstr_t)var).Detach();
		m_spDispatchEx->GetMemberName(dipid, pName);
		break;
	} while (true);

	return TRUE;
}



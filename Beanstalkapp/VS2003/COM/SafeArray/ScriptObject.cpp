#include "StdAfx.h"
#include ".\scriptobject.h"
#include <comutil.h>

CScriptObject::CScriptObject(VARIANT vScript)
{
	if(vScript.vt == VT_DISPATCH && vScript.pdispVal != NULL)
	{
		vScript.pdispVal->QueryInterface(IID_IDispatchEx, (void **)&m_spScript);
	}
}

CScriptObject::~CScriptObject(void)
{
}
// 创建接口
BOOL CScriptObject::CreateObject()
{
	if(m_spDispEx != NULL)
		return TRUE;

	if(m_spScript == NULL)
		return FALSE;

	// 得到Object
	DISPID			dispid		= 0;
	HRESULT			hres;

	hres = m_spScript->GetDispID(_bstr_t(L"Object"), 0, &dispid);
	if(FAILED(hres))
		return FALSE;
	// 创建Object
	DISPPARAMS		dispparamsNoArgs	= {0};
	VARIANT			var;

	VariantInit(&var);
	hres = m_spScript->InvokeEx(dispid, LOCALE_USER_DEFAULT, 
		DISPATCH_CONSTRUCT, &dispparamsNoArgs, 
		&var, NULL, NULL);
	// 得到接口
	if(var.vt = VT_DISPATCH && var.pdispVal)
	{
		var.pdispVal->QueryInterface(IID_IDispatchEx, (void **)&m_spDispEx);
		var.pdispVal->Release();
	}

	return m_spDispEx != NULL;
}
// 添加成员
HRESULT CScriptObject::AddMember(BSTR name, VARIANT* pVar)
{
	if(FALSE == CreateObject())
		return E_NOINTERFACE;

	// 创建属性
	DISPID		pid		= NULL;
	HRESULT		hres;

	hres = m_spDispEx->GetDispID(_bstr_t(name), fdexNameEnsure, &pid);
	if(FAILED(hres))
		return hres;

	// 构造参数
	DISPPARAMS			params		= {0};
	DISPID				putid		= DISPID_PROPERTYPUT;

	params.cArgs = 1;
	params.cNamedArgs = 1;
	params.rgdispidNamedArgs = &putid;
	params.rgvarg = pVar;

	// 设置属性
	hres = m_spDispEx->InvokeEx(pid, LOCALE_USER_DEFAULT
		, DISPATCH_PROPERTYPUTREF, &params, NULL, NULL, NULL);

	return hres;
}

// 删除成员
HRESULT CScriptObject::DeleteMember(BSTR name)
{
	if(FALSE == CreateObject())
		return E_NOINTERFACE;
	
	HRESULT		hres;

	return m_spDispEx->DeleteMemberByName(_bstr_t(name), fdexNameCaseSensitive);
}

// 得到成员
HRESULT CScriptObject::GetMember(BSTR name, VARIANT* pVar)
{
	if(FALSE == CreateObject())
		return E_NOINTERFACE;
	// 得到属性
	DISPID		pid		= NULL;
	HRESULT		hres;

	hres = m_spDispEx->GetDispID(_bstr_t(name), 0, &pid);
	if(FAILED(hres))
		return hres;

	// 构造参数
	DISPPARAMS			params		= {0};

	params.cArgs = 0;
	params.cNamedArgs = 0;
	params.rgdispidNamedArgs = NULL;
	params.rgvarg = NULL;

	// 得到属性
	hres = m_spDispEx->InvokeEx(pid, LOCALE_USER_DEFAULT
		, DISPATCH_PROPERTYGET, &params, pVar, NULL, NULL);

	return hres;
}

// 设置
HRESULT CScriptObject::Set(BSTR name, VARIANT* pVar)
{
	if(NULL == pVar)
		return DeleteMember(name);

	return AddMember(name, pVar);
}
HRESULT CScriptObject::Set(BSTR name, VARIANT& var)
{
	return AddMember(name, &var);
}
// 得到值
VARIANT CScriptObject::Get(BSTR name)
{
	VARIANT		var;

	VariantInit(&var);
	GetMember(name, &var);

	return var;
}

// 得到接口
HRESULT CScriptObject::GetInterface(IDispatchEx** ppDispEx)
{
	if(m_spDispEx == NULL)
		return E_NOINTERFACE;

	return m_spDispEx->QueryInterface(IID_IDispatchEx, (void **)ppDispEx);
}

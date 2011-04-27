#include "stdafx.h"
#include "IENAPAI.h"
#include "TNPObject.h"

NPClass		gNPDispatch		= {
	NP_CLASS_STRUCT_VERSION
	, TNPAllocate<CNPDispatchEx> 
	, TNPDeallocate<CNPDispatchEx> 
	, NULL
	, TNPHasMethod<CNPDispatchEx> 
	, TNPInvoke<CNPDispatchEx>
	, TNPInvokeDefault<CNPDispatchEx>
	, TNPHasProperty<CNPDispatchEx> 
	, TNPGetProperty<CNPDispatchEx> 
	, TNPSetProperty<CNPDispatchEx> 
	, TNPRemoveProperty<CNPDispatchEx>
	, TNPEnumeration<CNPDispatchEx>
	, NULL
};

CNPDispatchEx::CNPDispatchEx(NPP npp)
{
	m_pDispatchEx = NULL;

	if(NULL != npp && NULL != npp->ndata)
		((IDispatch *)npp->ndata)->QueryInterface(IID_IDispatchEx, (void **)&m_pDispatchEx);
}

CNPDispatchEx::~CNPDispatchEx()
{
	if(NULL != m_pDispatchEx)
		m_pDispatchEx->Release();
}

HRESULT CNPDispatchEx::QueryInterface(REFIID riid, void** ppunk)
{
	if(NULL == m_pDispatchEx)
		return E_NOTIMPL;

	return m_pDispatchEx->QueryInterface(riid, ppunk);
}

bool CNPDispatchEx::HasMethod(NPIdentifier methodName)
{
	if(NULL == m_pDispatchEx)
		return false;

	BSTR		bstr	= ::SysAllocString(IE_GetIdentifierString(methodName));
	DISPID		dispid	= DISPID_UNKNOWN;

	m_pDispatchEx->GetDispID(bstr, fdexNameCaseSensitive, &dispid);
	::SysFreeString(bstr);
	if(DISPID_UNKNOWN != dispid)
		return true;

	return false;
}

bool CNPDispatchEx::InvokeDefault(const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	if(NULL == m_pDispatchEx)
		return false;

	DISPPARAMS		param		= {0};
	VARIANT			var;
	HRESULT			hres;

	// 构建参数
	param.cArgs = argCount;
	if(param.cArgs > 0)
	{
		param.rgvarg = new VARIANT[param.cArgs];
		for(int i = 0; i < argCount; i++)
			IENPVariantToVariant(param.rgvarg[i], args[argCount-i-1]);
	}
	VariantInit(&var);
	// 调用
	hres = m_pDispatchEx->InvokeEx(0, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &param, &var, NULL, NULL);
	if(NULL != result)
		IEVariantToNPVariant(*result, var);
	VariantClear(&var);
	// 清除参数
	if(param.cArgs > 0)
	{
		for(int i = 0;i < param.cArgs; i++)
			VariantClear(&param.rgvarg[i]);
		delete [] param.rgvarg;
	}
	
	return false;
}
bool CNPDispatchEx::Invoke(NPIdentifier methodName, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	if(NULL == m_pDispatchEx)
		return false;
	// 得到DISPID
	BSTR			bstr		= ::SysAllocString(IE_GetIdentifierString(methodName));
	DISPID			dispid		= DISPID_UNKNOWN;
	
	m_pDispatchEx->GetDispID(bstr, fdexNameCaseSensitive, &dispid);
	::SysFreeString(bstr);
	if(DISPID_UNKNOWN != dispid)
		return false;

	// 调用过程
	DISPPARAMS		param		= {0};
	VARIANT			var;
	HRESULT			hres		= E_FAIL;

	// 构建参数
	param.cArgs = argCount;
	if(param.cArgs > 0)
	{
		param.rgvarg = new VARIANT[param.cArgs];
		for(int i = 0; i < argCount; i++)
			IENPVariantToVariant(param.rgvarg[i], args[argCount-i-1]);
	}
	VariantInit(&var);
	// 调用
	hres = m_pDispatchEx->InvokeEx(dispid, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &param, &var, NULL, NULL);
	if(NULL != result)
		IEVariantToNPVariant(*result, var);
	VariantClear(&var);
	// 清除参数
	if(param.cArgs > 0)
	{
		for(int i = 0;i < param.cArgs; i++)
			VariantClear(&param.rgvarg[i]);
		delete [] param.rgvarg;
	}

	return SUCCEEDED(hres);
}
// 判断属性
bool CNPDispatchEx::HasProperty(NPIdentifier propertyName)
{
	if(NULL == m_pDispatchEx)
		return false;

	BSTR		bstr	= ::SysAllocString(IE_GetIdentifierString(propertyName));
	DISPID		dispid	= DISPID_UNKNOWN;

	m_pDispatchEx->GetDispID(bstr, fdexNameCaseSensitive, &dispid);
	::SysFreeString(bstr);
	if(DISPID_UNKNOWN != dispid)
		return true;

	return false;
}
bool CNPDispatchEx::GetProperty(NPIdentifier propertyName, NPVariant *result)
{
	if(NULL == m_pDispatchEx)
		return false;
	// 得到DISPID
	BSTR			bstr		= ::SysAllocString(IE_GetIdentifierString(propertyName));
	DISPID			dispid		= DISPID_UNKNOWN;

	m_pDispatchEx->GetDispID(bstr, fdexNameCaseSensitive, &dispid);
	::SysFreeString(bstr);
	if(DISPID_UNKNOWN == dispid)
		return false;

	// 调用过程
	DISPPARAMS		param		= {0};
	VARIANT			var;
	HRESULT			hres		= E_FAIL;

	// 调用
	hres = m_pDispatchEx->InvokeEx(dispid, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &param, &var, NULL, NULL);
	if(NULL != result)
		IEVariantToNPVariant(*result, var);
	VariantClear(&var);
	
	return SUCCEEDED(hres);
}
bool CNPDispatchEx::SetProperty(NPIdentifier name, const NPVariant *value)
{
	if(NULL == m_pDispatchEx)
		return false;
	// 得到DISPID
	BSTR			bstr		= ::SysAllocString(IE_GetIdentifierString(name));
	DISPID			dispid		= DISPID_UNKNOWN;

	m_pDispatchEx->GetDispID(bstr, fdexNameCaseSensitive, &dispid);
	::SysFreeString(bstr);
	if(DISPID_UNKNOWN != dispid)
		return false;

	// 调用过程
	DISPPARAMS		param		= {0};
	VARIANT			var;
	HRESULT			hres		= E_FAIL;

	// 构建参数
	IENPVariantToVariant(var, *value);
	param.cArgs = 1;
	param.rgvarg = &var;
	// 调用
	hres = m_pDispatchEx->InvokeEx(dispid, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET, &param, NULL, NULL, NULL);
	VariantClear(&var);

	return SUCCEEDED(hres);
}

// 移除属性
bool CNPDispatchEx::RemoveProperty(NPIdentifier name)
{
	if(NULL == m_pDispatchEx)
		return false;
	// 得到DISPID
	BSTR			bstr		= ::SysAllocString(IE_GetIdentifierString(name));
	HRESULT			hres		= E_FAIL;

	// 调用过程
	hres = m_pDispatchEx->DeleteMemberByName(bstr, fdexNameCaseSensitive);
	::SysFreeString(bstr);

	return SUCCEEDED(hres);
}

// 枚举属性
bool CNPDispatchEx::Enumeration(NPIdentifier **value, uint32_t *count)
{
	if(NULL == m_pDispatchEx)
		return false;

	DISPID		dispid		= DISPID_STARTENUM;
	HRESULT		hres		= S_OK;
	std::vector<DISPID>		vctid;

	while(true)
	{
		hres = m_pDispatchEx->GetNextDispID(fdexEnumDefault, dispid, &dispid);
		if(S_OK != hres)
			break;
		vctid.push_back(dispid);
	}

	if(0 == vctid.size())
	{
		*value = NULL;
		*count = 0;
		return false;
	}

	*value = (NPIdentifier *)IE_NPN_MemAlloc(sizeof(NPIdentifier) * vctid.size());
	*count = (uint32_t)vctid.size();

	for(int i = 0; i < (int)vctid.size(); i++)
	{
		BSTR		bstr;

		m_pDispatchEx->GetMemberName(vctid[i], &bstr);
		(*value)[i] = IE_GetStringIdentifier(bstr);
		::SysFreeString(bstr);
	}

	return true;
}
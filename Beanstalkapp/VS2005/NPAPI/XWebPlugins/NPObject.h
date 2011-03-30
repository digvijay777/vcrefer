/*
 *	NPObject与IDispatchEx转换
 */
#pragma once
#include <dispex.h>
#include <string>

inline std::string	WideCharToUTF8(LPCWSTR lpWStr);
inline std::wstring UTF8ToWideChar(LPCSTR lpUtf8);
inline BOOL			ConvertVARIANTtoNPVariant(const VARIANT& var, NPVariant& npvar, NPNetscapeFuncs* pnpf = NULL, NPP npp = 0);
inline BOOL			ConvertNPVariantToVARIANT(const NPVariant& npvar, VARIANT& var, NPNetscapeFuncs* pnpf = NULL, NPP npp = 0);

#define W2U(x)			WideCharToUTF8((x)).c_str()	
#define U2W(x)			UTF8ToWideChar((x)).c_str()

//////////////////////////////////////////////////////////////////////////
// 对像
template <class T>
class TDispatchNPObject : public IDispatchEx
{
public:
	TDispatchNPObject(NPNetscapeFuncs* pnpf, NPP npp, NPObject* pObject)
	{
		m_pnpf = pnpf;
		m_npp = npp;
		m_pobject = pObject;
		m_nRef = 0;
	}

private:
	LONG					m_nRef;
	NPNetscapeFuncs*		m_pnpf;
	NPP						m_npp;
	NPObject*				m_pobject;
	
public:
	STDMETHOD(QueryInterface)(REFIID riid, void **ppvObject)
	{
		if(IID_IDispatchEx != riid)
			return E_NOINTERFACE;

		*ppvObject = this;
		AddRef();
		return S_OK;
	}

	STDMETHOD_(ULONG, AddRef)()
	{
		return InterlockedIncrement(&m_nRef);
	}

	STDMETHOD_(ULONG, Release)()
	{
		LONG	n	= InterlockedDecrement(&m_nRef);

		if(n <= 0)
			delete this;

		return n;
	}

	STDMETHOD(GetTypeInfoCount)(UINT *pctinfo)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(GetTypeInfo)(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(GetIDsOfNames)(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId)
	{
		if(IID_NULL != riid || NULL == m_pnpf || NULL == m_pobject)
			return E_NOTIMPL;

		for(UINT i = 0; i < cNames; i++)
		{
			rgDispId[i] = (DISPID)(__int64)m_pnpf->getstringidentifier( W2U(rgszNames[i]) );
		}

		return S_OK;
	}


	STDMETHOD(Invoke)(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams
		, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr)
	{
		if(IID_NULL != riid || NULL == m_pnpf || NULL == m_pobject)
			return E_NOTIMPL;

		if(DISPATCH_METHOD != wFlags && DISPATCH_PROPERTYGET != wFlags && DISPATCH_PROPERTYPUT != wFlags)
			return E_NOTIMPL;

		if(DISPATCH_PROPERTYGET == wFlags)
		{
			NPVariant		var;
			bool			bret;

			if(NULL == pVarResult)
				return ERROR_INVALID_PARAMETER;
			// 获取属性
			bret = m_pnpf->getproperty(m_npp, m_pobject, (NPIdentifier)(__int64)dispIdMember, &var);
			if(false == bret)
				return E_FAIL;
			if(FALSE == ConvertNPVariantToVARIANT(var, *pVarResult, m_pnpf, m_npp))
				return E_FAIL;

			return S_OK;
		}
		else if(DISPATCH_PROPERTYPUT == wFlags)
		{
			// 设置属性
			NPVariant		var;
			bool			bret;

			if(NULL == pDispParams || pDispParams->cArgs != 1)
				return ERROR_INVALID_PARAMETER;

			ConvertVARIANTtoNPVariant(pDispParams->rgvarg[0], var);
			bret = m_pnpf->setproperty(m_npp, m_pobject, (NPIdentifier)(__int64)dispIdMember, &var);
			if(false == bret)
				return E_FAIL;

			return S_OK;
		}

		// 调用方法
		NPVariant*		pVar		= NULL;
		int				nSize		= 0;
		NPVariant		vRet;
		bool			bret		= false;

		if(NULL != pDispParams)
		{
			nSize = pDispParams->cArgs;
			if(0 < nSize)
			{
				pVar = new NPVariant[nSize];
				for(int i = 0; i < nSize; i++)
				{
					ConvertVARIANTtoNPVariant(pDispParams->rgvarg[i], pVar[i], m_pnpf, m_npp);
				}
			}
		}
		bret = m_pnpf->invoke(m_npp, m_pobject, (NPIdentifier)(__int64)dispIdMember, pVar, nSize, &vRet);
		if(NULL != pVar)
			delete [] pVar;

		if(false == bret)
			return E_NOTIMPL;

		return S_OK;
	}

	STDMETHOD(GetDispID)(BSTR bstrName, DWORD grfdex, DISPID *pid)
	{
		if(NULL == m_pnpf || NULL == m_pobject)
			return E_NOTIMPL;

		*pid = (DISPID)(__int64)m_pnpf->getstringidentifier( W2U(bstrName) );

		return S_OK;
	}

	STDMETHOD(InvokeEx)(DISPID id, LCID lcid, WORD wFlags, DISPPARAMS *pdp, VARIANT *pvarRes
		, EXCEPINFO *pei, IServiceProvider *pspCalle)
	{
		return Invoke(id, IID_NULL, lcid, wFlags, pdp, pvarRes, pei, NULL);
	}

	STDMETHOD(DeleteMemberByName)(BSTR bstrName, DWORD grfdex)
	{
		if(NULL == m_pnpf || NULL == m_pobject)
			return E_NOTIMPL;

		NPIdentifier		id		= m_pnpf->getstringidentifier( W2U(bstrName) );
		
		if(0 == id)
			return E_FAIL;
		if(false == m_pnpf->removeproperty(m_npp, m_pobject, id))
			return E_FAIL;

		return S_OK;
	}

	STDMETHOD(DeleteMemberByDispID)(DISPID id)
	{
		if(NULL == m_pnpf || NULL == m_pobject)
			return E_NOTIMPL;

		if(false == m_pnpf->removeproperty(m_npp, m_pobject, (NPIdentifier)(__int64)id))
			return E_FAIL;

		return S_OK;
	}

	STDMETHOD(GetMemberProperties)(DISPID id, DWORD grfdexFetch, DWORD *pgrfdex)
	{
		return E_FAIL;
	}

	STDMETHOD(GetMemberName)(DISPID id, BSTR *pbstrName)
	{
		if(NULL == m_pnpf || NULL == m_pobject)
			return E_NOTIMPL;

		LPCSTR		pstr = m_pnpf->utf8fromidentifier((NPIdentifier)(__int64)id);

		if(NULL == pstr)
			return NULL;

		*pbstrName = ::SysAllocString( U2W(pstr) );
		return S_OK;
	}

	STDMETHOD(GetNextDispID)(DWORD grfdex, DISPID id, DISPID *pid)
	{
		if(NULL == m_pnpf || NULL == m_pobject)
			return E_NOTIMPL;

		NPIdentifier*		pNames		= NULL;
		uint32_t			nSize		= 0;
		bool				bRet;

		bRet = m_pnpf->enumerate(m_npp, m_pobject, &pNames, &nSize);
		if(0 == nSize)
			return E_FAIL;

		if(0 == id)
		{
			*pid = (DISPID)(__int64)pNames[0];
		}
		for(int i = 0; i < (int)nSize; i++)
		{
			if(id == (DISPID)(__int64)pNames[i])
			{
				if(i < (int)(nSize-1))
				{
					*pid = (DISPID)(__int64)pNames[i+1];
					return S_OK;
				}
			}
		}
		return E_FAIL;
	}

	STDMETHOD(GetNameSpaceParent)(IUnknown **ppunk)
	{
		return E_NOTIMPL;
	}

};

typedef TDispatchNPObject<void>		CDispatchNPObject;

//////////////////////////////////////////////////////////////////////////
// 宽字到UTF8
inline std::string WideCharToUTF8(LPCWSTR lpWStr)
{
	std::string			str;
	int					nSize		= 0;

	nSize = WideCharToMultiByte(CP_UTF8, 0, lpWStr, (int)wcslen(lpWStr), NULL, 0, NULL, NULL);
	if(0 == nSize)
		return str;
	str.resize(nSize+1, 0);
	WideCharToMultiByte(CP_UTF8, 0, lpWStr, (int)wcslen(lpWStr), (LPSTR)str.c_str(), nSize+1, NULL, 0);

	return str;
}
inline std::wstring UTF8ToWideChar(LPCSTR lpUtf8)
{
	std::wstring		str;
	int					nSize		= 0;

	nSize = MultiByteToWideChar(CP_UTF8, 0, lpUtf8, (int)strlen(lpUtf8), NULL, 0);
	if(0 == nSize)
		return str;
	str.resize(nSize+1, 0);
	MultiByteToWideChar(CP_UTF8, 0, lpUtf8, (int)strlen(lpUtf8), (LPWSTR)str.c_str(), nSize+1);
	
	return str;
}
// 转换类型
inline BOOL ConvertVARIANTtoNPVariant(const VARIANT& var, NPVariant& npvar, NPNetscapeFuncs* pnpf /* = NULL */, NPP npp /* = 0 */)
{
	switch(var.vt)
	{
	case VT_BOOL:
		{
			npvar.type = NPVariantType_Bool;
			npvar.value.boolValue = (VARIANT_FALSE != var.boolVal)?true:false;
		}
		break;
	case VT_I1:
	case VT_I2:
	case VT_I4:
	case VT_I8:
	case VT_UI1:
	case VT_UI2:
	case VT_UI4:
	case VT_UI8:
		{
			npvar.type = NPVariantType_Int32;
			npvar.value.intValue = var.lVal;
		}
		break;
	case VT_BSTR:
		{
			NPString		npstr;
			LPCSTR			lpstr		= W2U(var.bstrVal);

			npstr.UTF8Length = (uint32_t)strlen(lpstr);
			if(NULL == pnpf)
				return FALSE;
			npstr.UTF8Characters = (NPUTF8 *)pnpf->memalloc(npstr.UTF8Length);
			memcpy((void *)npstr.UTF8Characters, lpstr, npstr.UTF8Length);
			npvar.type = NPVariantType_String;
			npvar.value.stringValue = npstr;
		}
		break;
	case VT_R4:
	case VT_R8:
		{
			npvar.type = NPVariantType_Double;
			npvar.value.doubleValue = var.dblVal;
		}
		break;
	case VT_NULL:
	case VT_EMPTY:
		{
			npvar.type = NPVariantType_Null;
		}
		break;
	default:
		return FALSE;
	}

	return TRUE;
}

inline BOOL ConvertNPVariantToVARIANT(const NPVariant& npvar, VARIANT& var, NPNetscapeFuncs* pnpf /*= NULL*/, NPP npp /*= 0*/)
{
	VariantInit(&var);


	if(NPVARIANT_IS_OBJECT(npvar))
	{
		if(NULL == pnpf)
			return FALSE;

		var.vt = VT_DISPATCH;
		var.pdispVal = (IDispatchEx*)new CDispatchNPObject(pnpf, npp, NPVARIANT_TO_OBJECT(npvar));
	}
	else if(NPVARIANT_IS_BOOLEAN(npvar))
	{
		var.vt = VT_BOOL;
		var.boolVal = (FALSE != NPVARIANT_TO_BOOLEAN(npvar))?VARIANT_TRUE:VARIANT_FALSE;
	}
	else if(NPVARIANT_IS_DOUBLE(npvar))
	{
		var.vt = VT_R8;
		var.dblVal = NPVARIANT_TO_DOUBLE(npvar);
	}
	else if(NPVARIANT_IS_INT32(npvar))
	{
		var.vt = VT_I4;
		var.intVal = NPVARIANT_TO_INT32(npvar);
	}
	else if(NPVARIANT_IS_STRING(npvar))
	{
		LPCSTR		lpData		= NPVARIANT_TO_STRING(npvar).UTF8Characters;
		LPWSTR		lpBstr		= NULL;
		LONG		nSize		= 0;

		var.vt = VT_BSTR;
		nSize = MultiByteToWideChar(CP_UTF8, 0, lpData, NPVARIANT_TO_STRING(npvar).UTF8Length, NULL, 0);
		if(0 == nSize)
			var.bstrVal = ::SysAllocString(L"");
		else
		{
			nSize++;
			lpBstr = new WCHAR[nSize];
			memset(lpBstr, 0, nSize * sizeof(WCHAR));
			MultiByteToWideChar(CP_UTF8, 0, lpData, NPVARIANT_TO_STRING(npvar).UTF8Length, lpBstr, nSize);
			var.bstrVal = ::SysAllocString(lpBstr);
			delete [] lpBstr;
		}
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}
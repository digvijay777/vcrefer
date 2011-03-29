/*
 *	NPObject与IDispatchEx转换
 */
#pragma once
#include <dispex.h>
#include <string>

inline std::string WideCharToUTF8(LPCWSTR lpWStr);

#define W2U(x)			WideCharToUTF8((x)).c_str()	

//////////////////////////////////////////////////////////////////////////
// 对像
template <class T>
class TDispatchNPObject/* : public IDispatchEx*/
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
		return S_OK;
	}

	STDMETHOD(GetTypeInfo)(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo)
	{
		return S_OK;
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
		return S_OK;
	}

	STDMETHOD(GetDispID)(BSTR bstrName, DWORD grfdex, DISPID *pid)
	{
		return S_OK;
	}

	STDMETHOD(InvokeEx)(DISPID id, LCID lcid, WORD wFlags, DISPPARAMS *pdp, VARIANT *pvarRes
		, EXCEPINFO *pei, IServiceProvider *pspCalle)
	{
		return S_OK;
	}

	STDMETHOD(DeleteMemberByName)(BSTR bstrName, DWORD grfdex)
	{
		return S_OK;
	}

	STDMETHOD(DeleteMemberByDispID)(DISPID id)
	{
		return S_OK;
	}

	STDMETHOD(GetMemberProperties)(DISPID id, DWORD grfdexFetch, DWORD *pgrfdex)
	{
		return S_OK;
	}

	STDMETHOD(GetMemberName)(DISPID id, BSTR *pbstrName)
	{
		return S_OK;
	}

	STDMETHOD(GetNextDispID)(DWORD grfdex, DISPID id, DISPID *pid)
	{
		return S_OK;
	}

	STDMETHOD(GetNameSpaceParent)(IUnknown **ppunk)
	{
		return S_OK;
	}

};

typedef TDispatchNPObject<void>		CDispatchNPObject;

//////////////////////////////////////////////////////////////////////////
// 宽字到UTF8
inline std::string WideCharToUTF8(LPCWSTR lpWStr)
{
	std::string			str;
	int					nSize		= 0;

	nSize = WideCharToMultiByte(CP_UTF8, 0, lpWStr, wcslen(lpWStr), NULL, 0, NULL, NULL);
	if(0 == nSize)
		return str;
	str.resize(nSize+1, 0);
	WideCharToMultiByte(CP_UTF8, 0, lpWStr, wcslen(lpWStr), (LPSTR)str.c_str(), nSize+1, NULL, 0);

	return str;
}

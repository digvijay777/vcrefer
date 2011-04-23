// IEWebPlugin.cpp : Implementation of CIEWebPlugin
#include "stdafx.h"
#include "IEWebPlugin.h"


#include <vector>
#include <string>

#pragma comment(lib, "comsuppw.lib")
// CIEWebPlugin
STDMETHODIMP CIEWebPlugin::GetIDsOfNames(REFIID riid, OLECHAR FAR *FAR *rgszNames, unsigned int cNames, LCID lcid, DISPID FAR *rgDispId)
{
	if(S_OK == __super::GetIDsOfNames(riid, rgszNames, cNames, lcid, rgDispId))
		return S_OK;

	NPObject*		pObject			= NULL;

	gIEPlugFuncs.getvalue(&m_nppt, NPPVpluginScriptableNPObject, (void **)&pObject);
	if(NULL == pObject)
		return E_INVALIDARG;

	if(NULL == pObject->_class || NULL == pObject->_class->hasMethod)
	{
		gIENpFuncs.releaseobject(pObject);
		return E_INVALIDARG;
	}

	for(int i = 0; i < (int)cNames; i++)
	{
		NPIdentifier		id		= IE_GetStringIdentifier(rgszNames[i]);

		if(IE_NPHasMethod(pObject, id))
			rgDispId[i] = (DISPID)id;
		else
			rgDispId[i] = DISPID_UNKNOWN;
	}

	gIENpFuncs.releaseobject(pObject);
	return S_OK;
}
STDMETHODIMP CIEWebPlugin::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS FAR *pDispParams
								 , VARIANT FAR *pVarResult, EXCEPINFO FAR *pExcepInfo, unsigned int FAR *puArgErr)
{
	if(S_OK == __super::Invoke(dispIdMember, riid, lcid, wFlags, pDispParams
		, pVarResult, pExcepInfo, puArgErr))
		return S_OK;

	NPObject*		pObject			= NULL;
	HRESULT			hRet			= S_OK;
	bool			bRet			= false;

	gIEPlugFuncs.getvalue(&m_nppt, NPPVpluginScriptableNPObject, (void **)&pObject);
	if(NULL == pObject)
		return Error(pExcepInfo, L"未初始化实例");

 
	if( DISPATCH_METHOD == (DISPATCH_METHOD & wFlags) )
	{
		// 构建参数
		NPVariant		npRet;
		NPVariant*		npParam		= NULL;
		uint32_t		nCount		= 0;

		if(NULL != pDispParams)
		{
			nCount = pDispParams->cArgs;
			if(0 < nCount)
			{
				npParam = new NPVariant[nCount];
				for(int i = 0; i < nCount; i++)
					IEVariantToNPVariant(npParam[i], pDispParams->rgvarg[nCount-i-1]);
			}
		}
		// 调用
		bRet = IE_NPInvoke(pObject, (NPIdentifier)dispIdMember, npParam, nCount, &npRet);
		for(int i = 0; i < nCount; i++)
		{
			IEClearNPVariant(npParam[i]);
			VariantClear(&pDispParams->rgvarg[i]);
		}
		if(NULL != npParam)
			delete [] npParam;
		// 返回
		if(NULL != pVarResult)
			IENPVariantToVariant(*pVarResult, npRet);
		IEClearNPVariant(npRet);		
		// 清理
		if(false == bRet)
		{
			hRet = Error(pExcepInfo, IE_GetLastError(pObject));
		}
	}
 	else if(DISPATCH_PROPERTYGET == wFlags)
	{
		NPVariant		npRet;

		// 调用
		bRet = IE_NPGetProperty(pObject, (NPIdentifier)dispIdMember, &npRet);
		// 返回
		if(NULL != pVarResult)
			IENPVariantToVariant(*pVarResult, npRet);
		IEClearNPVariant(npRet);
		// 异常处理
		if(false == bRet)
		{
			hRet = Error(pExcepInfo, IE_GetLastError(pObject));
		}
	}
	else if(DISPATCH_PROPERTYPUT == wFlags)
	{
		NPVariant		npVal;

		if(NULL == pDispParams || 1 != pDispParams->cArgs)
			return Error(pExcepInfo, L"参数不正确.");

		IEVariantToNPVariant(npVal, pDispParams->rgvarg[0]);
		VariantClear(&pDispParams->rgvarg[0]);
		// 调用
		bRet = IE_NPSetProperty(pObject, (NPIdentifier)dispIdMember, &npVal);
		// 返回
		IEClearNPVariant(npVal);
		// 异常处理
		if(false == bRet)
		{
			hRet = Error(pExcepInfo, IE_GetLastError(pObject));
		}
	}
	else
	{
		hRet = Error(pExcepInfo, L"未知的调用过程");
	}

	gIENpFuncs.releaseobject(pObject);
	return hRet;
}

LRESULT CIEWebPlugin::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// 初始化对像
	GetHtmlDocument(&m_spHtmlDoc);
	// 初始化参数
	CComPtr<IHTMLObjectElement>				spObject;
	CComQIPtr<IHTMLObjectElement>			spNode;

	GetHtmlObjectTag(m_spHtmlDoc, &spObject);
	// 获取属性
	std::vector<std::string>				vctName;
	std::vector<std::string>				vctValue;
	LPSTR									strType			= NULL;
	int										nSize			= 0;
	
	GetHtmlTagAttribs((IHTMLObjectElement *)spObject, vctName, vctValue);


	LPSTR*		pNames		= new LPSTR[vctName.size()];
	LPSTR*		pValues		= new LPSTR[vctName.size()];
	for(int i = 0; i < vctName.size(); i++)
	{
		if(stricmp("type", vctName[i].c_str()) == 0)
		{
			strType = (LPSTR)vctValue[i].c_str();
		}
		else if(stricmp("data", vctName[i].c_str()) != 0
			&& stricmp("clsid", vctName[i].c_str()) != 0
			&& stricmp("width", vctName[i].c_str()) != 0
			&& stricmp("height", vctName[i].c_str()) != 0)
		{
			pNames[nSize] = (LPSTR)vctName[i].c_str();
			pValues[nSize] = (LPSTR)vctValue[i].c_str();
			nSize++;
		}
	}
	// 调用组件
	gIEPlugFuncs.newp(strType, &m_nppt, NP_FULL, nSize, pNames, pValues, NULL);
	delete [] pNames;
	delete [] pValues;

	return 0;
}

LRESULT CIEWebPlugin::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	gIEPlugFuncs.destroy(&m_nppt, NULL);
	return 0;
}

LRESULT CIEWebPlugin::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	NPWindow		window		= {0};
	RECT			rect;

	GetClientRect(&rect);
	window.clipRect.bottom = rect.bottom;
	window.clipRect.left = rect.left;
	window.clipRect.right = rect.right;
	window.clipRect.top = rect.top;
	window.x = rect.left;
	window.y = rect.top;
	window.width = rect.right - rect.left;
	window.height = rect.bottom - rect.top;
	window.type = NPWindowTypeWindow;
	window.window = (void *)m_hWnd;
	gIEPlugFuncs.setwindow(&m_nppt, &window);
	return 0;
}

// IEWebPlugin.cpp : Implementation of CIEWebPlugin
#include "stdafx.h"
#include "IEWebPlugin.h"

#include <comutil.h>
#include <vector>
#include <string>

#pragma comment(lib, "comsuppw.lib")
// CIEWebPlugin
STDMETHODIMP CIEWebPlugin::GetIDsOfNames(REFIID riid, OLECHAR FAR *FAR *rgszNames, unsigned int cNames, LCID lcid, DISPID FAR *rgDispId)
{
	if(S_OK == __super::GetIDsOfNames(riid, rgszNames, cNames, lcid, rgDispId))
		return S_OK;

// 	if(NULL != m_pPlugObject)
// 	{
// 		*rgDispId = m_pPlugObject->GetIDOfName(*rgszNames);
// 		return S_OK;
// 	}

	return E_INVALIDARG;
}
STDMETHODIMP CIEWebPlugin::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS FAR *pDispParams
								 , VARIANT FAR *pVarResult, EXCEPINFO FAR *pExcepInfo, unsigned int FAR *puArgErr)
{
	if(S_OK == __super::Invoke(dispIdMember, riid, lcid, wFlags, pDispParams
		, pVarResult, pExcepInfo, puArgErr))
		return S_OK;

// 	if(NULL == m_pPlugObject)
// 		return Error(L"未初始化实例");
// 
// 	if( DISPATCH_METHOD == (DISPATCH_METHOD & wFlags) )
// 	{
// 		VARIANT*		pVal		= NULL;
// 		int				nSize		= 0;
// 
// 		if(NULL != pDispParams)
// 		{
// 			nSize = pDispParams->cArgs;
// 			pVal = pDispParams->rgvarg;
// 		}
// 
// 		if(FALSE != m_pPlugObject->CallMethod(dispIdMember, pVal, nSize, pVarResult))
// 			return S_OK;
// 		return Error(m_pPlugObject->GetLastError());
// 	}
// 	else if(DISPATCH_PROPERTYGET == wFlags)
// 	{
// 		if(FALSE != m_pPlugObject->GetProperty(dispIdMember, pVarResult))
// 			return S_OK;
// 
// 		return Error(m_pPlugObject->GetLastError());
// 	}
// 	else if(DISPATCH_PROPERTYPUT == wFlags)
// 	{
// 		if(NULL == pDispParams || pDispParams->cArgs != 0)
// 			return Error(L"参数错误");
// 
// 		if(FALSE != m_pPlugObject->SetProperty(dispIdMember, pDispParams->rgvarg[0]))
// 			return S_OK;
// 
// 		return Error(m_pPlugObject->GetLastError());
// 	}
// 	else
// 	{
// 		return Error(L"未知的调用过程");
// 	}

	return Error(L"错误的调用过程");
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

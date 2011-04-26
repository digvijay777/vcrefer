#pragma once
#include <vector>
#include <string>

template <class T>
class TAtlIECom
{
public:
	// 得到网页接口
	HRESULT		GetHtmlDocument(IHTMLDocument2** pDoc)
	{
		HRESULT							hres;
		CComPtr<IOleClientSite>			spOleSite;
		T*								pT				= static_cast<T*>(this);

		// 得到停靠点
		//hres = pT->GetSite(IID_IOleClientSite, (void **)&spOleSite);
		hres = pT->GetClientSite(&spOleSite);
		if(FAILED(hres) || spOleSite == NULL)
		{
			ATLASSERT(FALSE);
			return E_NOINTERFACE;
		}
		// 得到网页接口
 		CComPtr<IOleContainer>			spContainer;

		hres = spOleSite->GetContainer(&spContainer);
		if(FAILED(hres) || spContainer == NULL)
		{
			ATLASSERT(FALSE);
			return E_NOINTERFACE;
		}

		hres = spContainer->QueryInterface(IID_IHTMLDocument2, (void **)pDoc);
		ATLASSERT(SUCCEEDED(hres));
		return hres;
	}
	// 得到标OBJECT标签
	HRESULT		GetHtmlObjectTag(IHTMLDocument2* pDoc, IHTMLObjectElement** pObject)
	{
		if(NULL == pDoc)
			return E_NOTIMPL;
		
		CComQIPtr<IHTMLDocument3>			spDoc3		= pDoc;
		BSTR								strObject	= ::SysAllocString(L"OBJECT");
		CComPtr<IHTMLElementCollection>		spElemets;
		HRESULT								hres;
		LONG								lLen		= 0;
		IDispatch*							pDisp		= (IDispatch *)static_cast<T *>(this);

		if(NULL == spDoc3)
			return E_NOTIMPL;
		hres = spDoc3->getElementsByTagName(strObject, &spElemets);
		ATLASSERT(SUCCEEDED(hres));
		spElemets->get_length(&lLen);
		for(long i = 0; i < lLen; i++)
		{
			VARIANT				var;
			CComPtr<IDispatch>	spElemet;
			CComQIPtr<IHTMLObjectElement>		spObject;
			CComPtr<IDispatch>	spPlugins;

			var.vt = VT_I4;
			var.lVal = i;
			spElemets->item(var, var, &spElemet);
			spObject = spElemet;
			if(NULL == spObject)
				continue;
			spObject->get_object(&spPlugins);
			if(spPlugins == pDisp)
			{
				spObject->QueryInterface(IID_IHTMLObjectElement, (void **)pObject);
				return S_OK;
			}
		}
		return E_NOINTERFACE;
	}
	// 得到属性
	HRESULT GetHtmlTagAttribs(CComQIPtr<IHTMLDOMNode> spNode, std::vector<std::string>& vctName, std::vector<std::string>& vctValue)
	{
		CComPtr<IDispatch>						spAttribs;
		CComQIPtr<IHTMLAttributeCollection>		spAttribColl;
		LONG									lCount				= 0;
		_bstr_t									bstrType;

		if(NULL == spNode)
			return E_NOTIMPL;
		spNode->get_attributes(&spAttribs);
		spAttribColl = spAttribs;
		if(NULL == spAttribColl)
			return E_NOTIMPL;
		spAttribColl->get_length(&lCount);
		for(long i = 0; i < lCount; i++)
		{
			CComPtr<IDispatch>					spAtt;
			CComQIPtr<IHTMLDOMAttribute>		spDomAtt;
			VARIANT								var;
			_bstr_t								bstrName;
			_variant_t							bstrVaue;
			VARIANT_BOOL						bspi			= VARIANT_FALSE;

			var.vt = VT_I4;
			var.lVal = i;
			spAttribColl->item(&var, &spAtt);
			spDomAtt = spAtt;
			if(NULL == spDomAtt)
				continue;
			spDomAtt->get_specified(&bspi);
			if(VARIANT_FALSE == bspi)
				continue;
			spDomAtt->get_nodeName(bstrName.GetAddress());
			spDomAtt->get_nodeValue(bstrVaue.GetAddress());

			vctName.push_back((LPCSTR)bstrName);
			vctValue.push_back((LPCSTR)(_bstr_t)bstrVaue);
		}
	}
};
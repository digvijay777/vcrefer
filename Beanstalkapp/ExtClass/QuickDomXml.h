#pragma once

#ifndef ATLTRACE
#ifdef TRACE
#define ATLTRACE		TRACE
#else
#define ATLTRACE
#endif
#endif
#ifndef ATLASSERT
#ifdef ASSERT
#define ATLASSERT		ASSERT
#else
#define ATLASSERT
#endif
#endif

template <typename TDom>
_bstr_t GetDomXMLValueT(TDom& spDom, _bstr_t bstrPath, _bstr_t def)
{
	try
	{
		MSXML2::IXMLDOMNodePtr		spNode;
		_bstr_t						bstrText;

		spNode = spDom->selectSingleNode(bstrPath);
		if(NULL == spNode)
			return def;
		spNode->get_text(bstrText.GetAddress());
		return bstrText;
	}
	catch(_com_error& e)
	{
		ATLASSERT(FALSE);
	}

	return def;
}

#define GetDomXMLStringA(spDom, path)		(LPCSTR)GetDomXMLValueT(spDom, path, L"")
#define GetDomXMLStringW(spDom, path)		(LPCWSTR)GetDomXMLValueT(spDom, path, L"")
#define GetDomXMLLong(spDom, path)			_wtol((LPCWSTR)GetDomXMLValueT(spDom, path, L"0"))




// ArrayTest.cpp : CArrayTest 的实现

#include "stdafx.h"
#include "ArrayTest.h"
#include ".\arraytest.h"
#include "ScriptObject.h"
#include <comutil.h>
// CArrayTest
#pragma comment(lib, "comsupp.lib")


STDMETHODIMP CArrayTest::TestBSTRArray(VARIANT* pVal)
{
	// TODO: 在此添加实现代码
	SAFEARRAY*		sa;
	SAFEARRAYBOUND	bound;
	bound.cElements = 3;
	bound.lLbound = 0;
// 	sa = SafeArrayCreate(VT_BSTR, 1, &bound);
	sa = SafeArrayCreateEx(VT_BSTR, 1, &bound, NULL);
	if(NULL == sa)
		return S_OK;
	LONG i = 0;
	SafeArrayPutElement(sa, &i, ::SysAllocString(L"Test1"));
	i = 2;
	SafeArrayPutElement(sa, &i, ::SysAllocString(L"Test3"));

	//VARIANT vt;
	//VariantInit(&vt);
	VariantInit(pVal);
	pVal->vt = VT_BSTR | VT_ARRAY;
	pVal->parray = sa;
	SafeArrayUnlock (sa);
	//AccessCheck();
	return S_OK;
}

STDMETHODIMP CArrayTest::GetObject(VARIANT vScript, VARIANT* lpVal)
{
	CScriptObject		object(vScript);

	object.Set(L"name", _variant_t(L"yuanjie"));
	object.Set(L"name", _variant_t(L"lm"));
	object.Set(L"age", _variant_t(20L));

	lpVal->vt = VT_DISPATCH;
	object.GetInterface((IDispatchEx**)&lpVal->pdispVal);

	return S_OK;
}

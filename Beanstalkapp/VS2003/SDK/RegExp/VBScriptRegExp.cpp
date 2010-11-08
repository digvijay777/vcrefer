#include "stdafx.h"

#import "C:\Windows\System32\vbscript.dll" tlbid(2) no_namespace named_guids raw_interfaces_only
// #import "X:\\regexp.lib" no_namespace named_guids raw_interfaces_only
// #import "progid:VBScript.RegExp" no_namespace named_guids raw_interfaces_only

void TestVBScriptRegExp()
{
	if(S_OK != CoInitialize(NULL))
		return;
	//VBScript_RegExp_10::
	IRegExp*		pRegExp;
	HRESULT			hr;

	hr = CoCreateInstance(CLSID_RegExp, NULL, CLSCTX_INPROC_SERVER, 
		IID_IRegExp, (void **)&pRegExp);
	do 
	{
		if(FAILED(hr))
			break;
		IMatchCollection*		pMatchCollection;
		IMatch*					pMatch;
		LONG					lMatchCount;

		pRegExp->put_Pattern(BSTR(L"^{[0-9]?[0-9]}:{[0-9][0-9]}$"));
		pRegExp->put_Global(VARIANT_TRUE);		// 查找全部
		pRegExp->put_IgnoreCase(VARIANT_TRUE);	// 大小写敏感
		hr = pRegExp->Execute(BSTR(L"23：56"), (IDispatch**)&pMatchCollection);
		if(FAILED(hr))
			break;
		pMatchCollection->get_Count(&lMatchCount);
		for(LONG i = 0; i < lMatchCount; i++)
		{
			BSTR	bstrValue;
			LONG	lSize		= 0;
			hr = pMatchCollection->get_Item(i, (IDispatch **)&pMatch);
			if(FAILED(hr))
				continue;
			pMatch->get_Value(&bstrValue);
			pMatch->get_Length(&lSize);
			printf("%d: \"%S\"\n", i, bstrValue);
		}				
	} while (false);

	if(NULL != pRegExp)
		pRegExp->Release();
	CoUninitialize();
}
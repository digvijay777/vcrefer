// ErrTest2.cpp : CErrTest2 的实现

#include "stdafx.h"
#include "ErrTest2.h"
#include ".\errtest2.h"


// CErrTest2

STDMETHODIMP CErrTest2::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IErrTest2
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CErrTest2::ErrTest(void)
{
	// TODO: 在此添加实现代码
	Error("错误的操作");

	return E_FAIL;
}

// ErrTest2.cpp : CErrTest2 ��ʵ��

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
	// TODO: �ڴ����ʵ�ִ���
	Error("����Ĳ���");

	return E_FAIL;
}

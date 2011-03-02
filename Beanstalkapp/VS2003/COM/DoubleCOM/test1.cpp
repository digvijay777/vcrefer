// test1.cpp : Implementation of Ctest1

#include "stdafx.h"
#include "test1.h"
#include ".\test1.h"
#include <comdef.h>

// Ctest1



STDMETHODIMP Ctest1::GetStr1(BSTR* lpVal)
{
	*lpVal = _bstr_t("test 1").Detach();

	return S_OK;
}

STDMETHODIMP Ctest1::GetStr2(BSTR* lpVal)
{
	*lpVal = _bstr_t("test 2").Detach();

	return S_OK;
}

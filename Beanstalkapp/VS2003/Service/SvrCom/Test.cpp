// Test.cpp : CTest 的实现

#include "stdafx.h"
#include "Test.h"
#include ".\test.h"


// CTest


STDMETHODIMP CTest::GetMsg(BSTR* lpVal)
{
	// TODO: 在此添加实现代码
	*lpVal = SysAllocString(L"This service com test.");
	return S_OK;
}

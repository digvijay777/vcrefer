// ExeComTest.cpp : CExeComTest 的实现

#include "stdafx.h"
#include "ExeComTest.h"
#include ".\execomtest.h"


// CExeComTest


STDMETHODIMP CExeComTest::GetMsg(BSTR* lpVal)
{
	// TODO: 在此添加实现代码

	*lpVal = SysAllocString(L"This is a test.");
	return S_OK;
}

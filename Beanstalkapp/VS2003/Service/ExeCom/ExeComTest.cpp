// ExeComTest.cpp : CExeComTest ��ʵ��

#include "stdafx.h"
#include "ExeComTest.h"
#include ".\execomtest.h"


// CExeComTest


STDMETHODIMP CExeComTest::GetMsg(BSTR* lpVal)
{
	// TODO: �ڴ����ʵ�ִ���

	*lpVal = SysAllocString(L"This is a test.");
	return S_OK;
}

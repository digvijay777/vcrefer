// Test.cpp : CTest ��ʵ��

#include "stdafx.h"
#include "Test.h"
#include ".\test.h"


// CTest


STDMETHODIMP CTest::GetMsg(BSTR* lpVal)
{
	// TODO: �ڴ����ʵ�ִ���
	*lpVal = SysAllocString(L"This service com test.");
	return S_OK;
}

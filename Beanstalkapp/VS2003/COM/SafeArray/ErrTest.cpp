// ErrTest.cpp : CErrTest ��ʵ��

#include "stdafx.h"
#include "ErrTest.h"
#include ".\errtest.h"


// CErrTest


STDMETHODIMP CErrTest::ErrTest(void)
{
	// TODO: �ڴ����ʵ�ִ���
	Error("����Ĳ���");
	return E_FAIL;
}

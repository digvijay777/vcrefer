// ErrTest.cpp : CErrTest 的实现

#include "stdafx.h"
#include "ErrTest.h"
#include ".\errtest.h"


// CErrTest


STDMETHODIMP CErrTest::ErrTest(void)
{
	// TODO: 在此添加实现代码
	Error("错误的操作");
	return E_FAIL;
}

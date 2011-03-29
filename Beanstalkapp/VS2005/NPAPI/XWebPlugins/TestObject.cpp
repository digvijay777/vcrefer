#include "StdAfx.h"
#include ".\testobject.h"

VPlugObject* CTestObject::CreateObject()
{
	return new CTestObject();
}

CTestObject::CTestObject(void)
{
}

CTestObject::~CTestObject(void)
{
}

ULONG CTestObject::GetIDOfName(LPCWSTR name)
{
	if(wcscmp(L"test", name) == 0)
		return 1;
	return -1;
}

BOOL CTestObject::CallMethod(ULONG nMethodID, const VARIANT *args, UINT argCount, VARIANT *lpVal)
{
	if(-1 == nMethodID)
		return FALSE;

	if(1 == nMethodID)
	{
		MessageBoxW(NULL, args[0].bstrVal, L"TestObject", MB_OK);
		return TRUE;
	}

	return FALSE;
}

BOOL CTestObject::SetProperty(ULONG nPropertyID, const VARIANT var)
{
	return FALSE;
}

BOOL CTestObject::GetProperty(ULONG nPropertyID, VARIANT* lpVal)
{
	return FALSE;
}

void CTestObject::Release()
{
	delete this;
}

void CTestObject::SetWindow(HWND hParent, LPRECT lpRect)
{

}



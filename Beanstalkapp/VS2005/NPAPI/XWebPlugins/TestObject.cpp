#include "StdAfx.h"
#include ".\testobject.h"

VPlugObject* CTestObject::CreateObject()
{
	return new CTestObject();
}

CTestObject::CTestObject(void)
{
	m_pPlugDocument = NULL;
	m_hParentWnd = NULL;
	memset(m_szError, 0, sizeof(m_szError));
	// 初始化列表
	m_objItem.push_back(OBJECTITEM(ObjectItem_Method, L"test", &CTestObject::OnTest));
}

CTestObject::~CTestObject(void)
{
	for(int i = 0; i < (int)m_objItem.size(); i++)
	{
		if(ObjectItem_Property == m_objItem[i].type)
			VariantInit(&m_objItem[i].value.val);
	}
}

BOOL CTestObject::Error(LPCWSTR lpFmt, ...)
{
	va_list		vaArr;
	WCHAR		szError[1024]		= {0};

	va_start(vaArr, lpFmt);
	_vsnwprintf(szError, sizeof(szError)/sizeof(WCHAR), lpFmt, vaArr);
	va_end(vaArr);
	
	return FALSE;
}

LPCWSTR CTestObject::GetLastError()
{
	return m_szError;
}

BOOL CTestObject::SetDocument(VPlugDocument* pDocument)
{
	m_pPlugDocument = pDocument;
	return TRUE;
}

ULONG CTestObject::GetIDOfName(LPCWSTR name)
{
	for(int i = 0; i < (int)m_objItem.size(); i++)
	{
		if(wcscmp(m_objItem[i].name.c_str(), name) == 0)
			return i;
	}

	return -1;
}

BOOL CTestObject::CallMethod(ULONG nMethodID, const VARIANT *args, UINT argCount, VARIANT *lpVal)
{
	if(nMethodID >= (ULONG)m_objItem.size())
		return FALSE;

	if(ObjectItem_Method != m_objItem[nMethodID].type)
		return FALSE;

	if(NULL == m_objItem[nMethodID].value.call)
		return FALSE;

	return (this->*m_objItem[nMethodID].value.call)(args, argCount, lpVal);
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
	m_hParentWnd = hParent;
}

BOOL CTestObject::OnTest(const VARIANT *args, UINT argCount, VARIANT *lpVal)
{
	if(0 < argCount)
		MessageBoxW(m_hParentWnd, args[0].bstrVal, L"WebRequest", MB_OK|MB_ICONINFORMATION);
	return TRUE;
}


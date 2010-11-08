#include "stdafx.h"
#include <oleacc.h> 
#include "ExternIEOpt.h"

TCHAR							g_szBuffer[1024];
CComPtr<IHTMLDocument2>			g_spDoc2				= NULL;

//得到网页文档对象
IHTMLDocument2* ExtIE::GetDocInterface(HWND hWnd)
{
	HINSTANCE			hInst		= NULL;
	const UINT			nMsg		= ::RegisterWindowMessage(_T("WM_HTML_GETOBJECT"));
	LRESULT				lRes;
	HRESULT				hr;

	hInst = ::LoadLibrary(_T("oleacc.dll"));
	if(NULL != hInst)
	{
		if(::IsWindow(hWnd))
		{
			::SendMessageTimeout(hWnd, nMsg, 0, 0, SMTO_ABORTIFHUNG, 1000, (DWORD*)&lRes);
			LPFNOBJECTFROMLRESULT pfObjectFromLresult = \
				(LPFNOBJECTFROMLRESULT)::GetProcAddress(hInst, _T("ObjectFromLresult"));
			if(NULL != pfObjectFromLresult)
			{
				hr = pfObjectFromLresult(lRes, IID_IHTMLDocument2, 0, (void**)&g_spDoc2);
			}
		}
		::FreeLibrary(hInst);
	}
	else
	{
		AfxMessageBox(_T("请您安装Microsoft Active Accessibility"));
	}

	return g_spDoc2;
}

//得到密码文本
LPCTSTR ExtIE::GetPassWord(IHTMLDocument2* pDoc2, POINT pt)
{
	CComPtr<IHTMLElement>			spElement	= NULL;
	CComPtr<IHTMLInputElement>		spPasswd	= NULL;
	HRESULT							hr;
	CComBSTR						bsType		= NULL;
	CComBSTR						bsPwd		= NULL;;

	memset(g_szBuffer, 0, sizeof(TCHAR) * 1024);
	if(NULL == pDoc2)
	{
		_tcscpy(g_szBuffer, _T("不是浏览器窗口"));
		return g_szBuffer;
	}

	hr = pDoc2->elementFromPoint(pt.x, pt.y, &spElement);
	if(SUCCEEDED(hr))
	{
		hr = spElement->QueryInterface(&spPasswd);
		if(SUCCEEDED(hr))
		{
			spPasswd->get_type(&bsType);
			if(wcsicmp(bsType, L"password") == 0)
			{
				hr = spPasswd->get_value(&bsPwd);
				if(SUCCEEDED(hr) && bsPwd != NULL)
				{
#ifdef	_UNICODE
					wcscpy(g_szBuffer, bsPwd);
#else
					WideCharToMultiByte(CP_ACP, CP_ACP, bsPwd, wcslen(bsPwd), \
						g_szBuffer, 1024, NULL, NULL);
#endif
				}
				else
				{
					_tcscpy(g_szBuffer, _T("密码为空"));
				}
			}
			else
			{
				_tcscpy(g_szBuffer, _T("不是有密码框"));
			}
		}
		else
		{
			_tcscpy(g_szBuffer, _T("不是浏览器的输入框"));
		}
	}
	else
	{
		_tcscpy(g_szBuffer, _T("不可操作"));
	}

	return g_szBuffer;
}
//初始化
void ExtIE::CoInitialize()
{
	::CoInitialize(NULL);
}
//释放
void ExtIE::CoUninitialize()
{
	if(g_spDoc2 != NULL)
		g_spDoc2.Release();
	
	g_spDoc2 = NULL;
	::CoUninitialize();
}
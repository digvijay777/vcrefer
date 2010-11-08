#include "stdafx.h"
#include <oleacc.h> 
#include "ExternIEOpt.h"

TCHAR							g_szBuffer[1024];
CComPtr<IHTMLDocument2>			g_spDoc2				= NULL;

//�õ���ҳ�ĵ�����
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
		AfxMessageBox(_T("������װMicrosoft Active Accessibility"));
	}

	return g_spDoc2;
}

//�õ������ı�
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
		_tcscpy(g_szBuffer, _T("�������������"));
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
					_tcscpy(g_szBuffer, _T("����Ϊ��"));
				}
			}
			else
			{
				_tcscpy(g_szBuffer, _T("�����������"));
			}
		}
		else
		{
			_tcscpy(g_szBuffer, _T("����������������"));
		}
	}
	else
	{
		_tcscpy(g_szBuffer, _T("���ɲ���"));
	}

	return g_szBuffer;
}
//��ʼ��
void ExtIE::CoInitialize()
{
	::CoInitialize(NULL);
}
//�ͷ�
void ExtIE::CoUninitialize()
{
	if(g_spDoc2 != NULL)
		g_spDoc2.Release();
	
	g_spDoc2 = NULL;
	::CoUninitialize();
}
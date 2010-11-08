#pragma once
#include <Mshtml.h>

namespace ExtIE
{
	void					CoInitialize();
	void					CoUninitialize();
	IHTMLDocument2*			GetDocInterface(HWND hWnd);
	LPCTSTR					GetPassWord(IHTMLDocument2* pDoc2, POINT pt);
}
// WebForm.cpp : Implementation of CWebForm

#include "stdafx.h"
#include "DynamicWeb.h"
#include "WebForm.h"
#include ".\webform.h"

/////////////////////////////////////////////////////////////////////////////
// CWebForm

LRESULT CWebForm::OnClickedButton1(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// TODO : Add Code for control notification handler.
	CComPtr<IOleContainer> spIE;
	m_spClientSite->GetContainer(&spIE);
	CComQIPtr<IHTMLDocument2, &IID_IHTMLDocument2> spDoc(spIE);
	if(spDoc)
	{
		char	szColor[8] = {0};
		GetDlgItemText(IDC_BKCOLOR, szColor, 7);
		spDoc->put_bgColor(CComVariant(szColor));
	}
	return 0;
}

LRESULT CWebForm::OnClickedButton2(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	// TODO : Add Code for control notification handler.
	char szURL[128] = {0};
	CComPtr<IOleContainer>	spIE;
	m_spClientSite->GetContainer(&spIE);
	CComPtr<IHTMLWindow2>  spWnd;
	CComQIPtr<IHTMLDocument2, &IID_IHTMLDocument2> spDoc(spIE);
	if(spDoc)
	{
		spDoc->get_parentWindow(&spWnd);
		if(spWnd)
		{
			CComPtr<IHTMLWindow2> spNewWnd;
			GetDlgItemText(IDC_NEWURL, szURL, 127);
			spWnd->open(CComBSTR(szURL), CComBSTR("NewWindow"),
				CComBSTR("toolbar:no, status:no, menubar:no, \
				directories:no, width=350, height=200"),
				VARIANT_FALSE,  &spNewWnd);
		}
	}
	return 0;
}
LRESULT CWebForm::OnBnClickedButton3(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here

	CComPtr<IOleContainer> spIE;
	m_spClientSite->GetContainer(&spIE);
	CComPtr<IHTMLWindow2> spWnd;
	CComQIPtr<IHTMLDocument2, &IID_IHTMLDocument2> spDoc(spIE);
	if(spDoc)
	{
		spDoc->get_parentWindow(&spWnd);
		if(spWnd)
		{
			//Æô·¢ÓÚJavascript
			VARIANT					v;
			CComPtr<IHTMLWindow2>	spNewWnd;
			VariantInit(&v);
			v.vt = VT_BSTR;
			v.bstrVal = CComBSTR("");
			spWnd->put_opener(v);
			spWnd->open(CComBSTR(""), CComBSTR("_parent"), CComBSTR(""), VARIANT_FALSE, &spNewWnd);
			spWnd->close();
		}
	}
	return 0;
}

// WebInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WebInfo.h"
#include "WebInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWebInfoDlg dialog

BEGIN_DHTML_EVENT_MAP(CWebInfoDlg)
// 	DHTML_EVENT_CLASS(DISPID_HTMLELEMENTEVENTS_ONMOUSEOUT, _T("button"), OnButtonMouseLeave)
// 	DHTML_EVENT_CLASS(DISPID_HTMLELEMENTEVENTS_ONMOUSEOVER, _T("button"), OnButtonMouseHover)
// 	DHTML_EVENT_CLASS(DISPID_HTMLELEMENTEVENTS_ONMOUSEDOWN, _T("button"), OnButtonMouseDown)
// 	DHTML_EVENT_CLASS(DISPID_HTMLELEMENTEVENTS_ONMOUSEUP, _T("button"), OnButtonMouseUp)
// 	DHTML_EVENT_TAG(DISPID_HTMLELEMENTEVENTS_ONMOUSEDOWN, _T("body"), OnBodyLButtonDown)
// 	DHTML_EVENT_TAG(DISPID_HTMLELEMENTEVENTS_ONMOUSEUP, _T("body"), OnBodyLButtonUp)
// 	DHTML_EVENT_TAG(DISPID_HTMLELEMENTEVENTS_ONMOUSEMOVE, _T("body"), OnBodyLButtonMove)
// 	DHTML_EVENT_TAG(DISPID_HTMLELEMENTEVENTS_ONCONTEXTMENU, _T("body"), OnBodyContextMenu)
// 	DHTML_EVENT_TAG(DISPID_HTMLELEMENTEVENTS_ONMOUSEDOWN, _T("td"), OnBodyLButtonDown)
// 	DHTML_EVENT_TAG(DISPID_HTMLELEMENTEVENTS_ONMOUSEUP, _T("td"), OnBodyLButtonUp)
// 	DHTML_EVENT_TAG(DISPID_HTMLELEMENTEVENTS_ONMOUSEMOVE, _T("td"), OnBodyLButtonMove)
// 	DHTML_EVENT_TAG(DISPID_HTMLELEMENTEVENTS_ONCONTEXTMENU, _T("td"), OnBodyContextMenu)
// 	DHTML_EVENT_TAG(DISPID_HTMLELEMENTEVENTS_ONMOUSEDOWN, _T("div"), OnBodyLButtonDown)
// 	DHTML_EVENT_TAG(DISPID_HTMLELEMENTEVENTS_ONMOUSEUP, _T("div"), OnBodyLButtonUp)
// 	DHTML_EVENT_TAG(DISPID_HTMLELEMENTEVENTS_ONMOUSEMOVE, _T("div"), OnBodyLButtonMove)
// 	DHTML_EVENT_TAG(DISPID_HTMLELEMENTEVENTS_ONCONTEXTMENU, _T("div"), OnBodyContextMenu)
END_DHTML_EVENT_MAP()

BEGIN_EVENTSINK_MAP(CWebInfoDlg, CMyDHtmlDialog)
	ON_EVENT(CWebInfoDlg, AFX_IDC_BROWSER, 271 /*271 DISPID_NAVIGATEERROR */, OnNavigateError, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
END_EVENTSINK_MAP()
/*DISPID_NAVIGATEERROR*/
CWebInfoDlg::CWebInfoDlg(CWnd* pParent /*=NULL*/)
	: CMyDHtmlDialog(CWebInfoDlg::IDD, NULL/*CWebInfoDlg::IDH*/, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWebInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CMyDHtmlDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWebInfoDlg, CMyDHtmlDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CWebInfoDlg message handlers

BOOL CWebInfoDlg::OnInitDialog()
{
	CMyDHtmlDialog::OnInitDialog();


	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	Navigate(_T("f:\\MySVN\\GoogleCode\\Beanstalkapp\\VS2005\\MFCControl\\WebInfo\\") _T("WebInfo.htm"));
	//Navigate(_T("http://www.baidu13164654679879.com/test.jsp"));
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWebInfoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CMyDHtmlDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CWebInfoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CWebInfoDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(WM_LBUTTONUP == message || WM_RBUTTONUP == message)
	{
		if(::GetCapture() == GetSafeHwnd())
		{
			ReleaseCapture();
			if(NULL != m_spCapture)
			{
				LONG			nX = 0, nY = 0, nCX = 0, nCY = 0;
				CRect			rect(0, 0, 0, 0);

				m_spCapture->get_offsetLeft(&nX);
				m_spCapture->get_offsetTop(&nY);
				m_spCapture->get_offsetWidth(&nCX);
				m_spCapture->get_offsetHeight(&nCY);
				rect.left = nX;
				rect.top = nY;
				rect.right = rect.left + nCX;
				rect.bottom = rect.top + nCY;
				if(rect.PtInRect(CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))))
					OnButtonMouseUp(m_spCapture);
				else
					OnButtonMouseLeave(m_spCapture);
			}
			m_spCapture.Release();
			return 0;
		}
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

HRESULT CWebInfoDlg::OnBodyLButtonDown(IHTMLElement *pElement)
{
	// SendMessage(WM_SYSCOMMAND, SC_MOVE, 0);
	POINT		point		= {0};

	GetCursorPos(&point);
	SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
	return S_FALSE;
}

HRESULT CWebInfoDlg::OnBodyLButtonUp(IHTMLElement *pElement)
{
	// SendMessage(WM_SYSCOMMAND, SC_MOVE, 0);
	POINT		point		= {0};

	GetCursorPos(&point);
	SendMessage(WM_NCLBUTTONUP, HTCAPTION, MAKELPARAM(point.x, point.y));
	return S_FALSE;
}

HRESULT CWebInfoDlg::OnBodyLButtonMove(IHTMLElement *pElement)
{
	// SendMessage(WM_SYSCOMMAND, SC_MOVE, 0);
	POINT		point		= {0};

	GetCursorPos(&point);
	SendMessage(WM_NCMOUSEMOVE, HTCAPTION, MAKELPARAM(point.x, point.y));
	return S_FALSE;
}

HRESULT CWebInfoDlg::OnBodyContextMenu(IHTMLElement *pElement)
{
	return S_FALSE;
}

HRESULT CWebInfoDlg::OnButtonMouseLeave(IHTMLElement *pElement)
{
	CComPtr<IHTMLStyle>		spStyle;
	CComVariant				varPos;

	if(::GetCapture() == GetSafeHwnd())
		return S_FALSE;

	pElement->get_style(&spStyle);
	if(NULL == spStyle)
		return S_FALSE;
	// 改变背景索引
	varPos = 0L;
	spStyle->put_backgroundPositionX(varPos);

	return S_FALSE;
}

HRESULT CWebInfoDlg::OnButtonMouseHover(IHTMLElement *pElement)
{
	CComPtr<IHTMLStyle>		spStyle;
	LONG					nWidth		= 0;
	CComVariant				varPos;
	CComBSTR				bstrDis("disabled");
	CComVariant				varDis;

	if(::GetCapture() == GetSafeHwnd())
		return S_FALSE;

	pElement->getAttribute(bstrDis.m_str, 0, &varDis);
	if(VT_BOOL == varDis.vt && VARIANT_FALSE != varDis.boolVal)
		return S_FALSE;
	pElement->get_style(&spStyle);
	if(NULL == spStyle)
		return S_FALSE;
	// 获取大小
	pElement->get_offsetWidth(&nWidth);
	// 改变背景索引
	varPos = -nWidth;
	spStyle->put_backgroundPositionX(varPos);

	return S_FALSE;
}

HRESULT CWebInfoDlg::OnButtonMouseDown(IHTMLElement *pElement)
{
	CComBSTR				bstrDis("disabled");
	CComVariant				varDis;

	pElement->getAttribute(bstrDis.m_str, 0, &varDis);
	if(VT_BOOL == varDis.vt && VARIANT_FALSE != varDis.boolVal)
		return S_FALSE;

	m_spCapture.Release();
	m_spCapture = pElement;
	SetCapture();
	
	CComPtr<IHTMLStyle>		spStyle;
	LONG					nWidth		= 0;
	CComVariant				varPos;

	pElement->get_style(&spStyle);
	if(NULL == spStyle)
		return S_FALSE;
	// 获取大小
	pElement->get_offsetWidth(&nWidth);
	// 改变背景索引
	varPos = -nWidth * 2;
	spStyle->put_backgroundPositionX(varPos);
	return S_FALSE;
}

HRESULT CWebInfoDlg::OnButtonMouseUp(IHTMLElement *pElement)
{
	OnButtonMouseHover(pElement);

	pElement->click();
	return S_FALSE;
}

HRESULT CWebInfoDlg::OnNull(IHTMLElement *pElement)
{
#ifdef DEBUG
	CComBSTR			bstrTagName;
	CComBSTR			bstrClass;
	CComBSTR			bstrID;

	pElement->get_tagName(&bstrTagName);
	pElement->get_className(&bstrClass);
	pElement->get_id(&bstrClass);
	
	TRACE("[CWebInfoDlg::OnNull] TagName:%S, class: %S, id: %S"
		, bstrTagName.m_str, bstrClass.m_str, bstrID.m_str);
#endif
	return S_FALSE;
}

void CWebInfoDlg::OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl)
{
	CMyDHtmlDialog::OnDocumentComplete(pDisp, szUrl);

	// TODO: Add your specialized code here and/or call the base class
}

void CWebInfoDlg::OnNavigateComplete(LPDISPATCH pDisp, LPCTSTR szUrl)
{
	CMyDHtmlDialog::OnNavigateComplete(pDisp, szUrl);

	 CComQIPtr<ICustomDoc,   &IID_ICustomDoc>   spCustomDoc(m_spHtmlDoc);

}

void CWebInfoDlg::OnNavigateError(LPDISPATCH pDisp, VARIANT* pvURL, VARIANT* pvFrame, VARIANT* pvStatusCode, VARIANT_BOOL* pvbCancel)
{
	*pvbCancel = VARIANT_TRUE;
// 	Navigate(_T("http://www.baidu.com"));
	return;
}
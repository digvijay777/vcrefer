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
	DHTML_EVENT_TAG(DISPID_HTMLELEMENTEVENTS_ONMOUSEDOWN, _T("body"), OnBodyLButtonDown)
	DHTML_EVENT_TAG(DISPID_HTMLELEMENTEVENTS_ONMOUSEUP, _T("body"), OnBodyLButtonUp)
	DHTML_EVENT_TAG(DISPID_HTMLELEMENTEVENTS_ONMOUSEMOVE, _T("body"), OnBodyLButtonMove)
	DHTML_EVENT_TAG(DISPID_HTMLELEMENTEVENTS_ONCONTEXTMENU, _T("body"), OnBodyContextMenu)
	DHTML_EVENT_TAG(DISPID_HTMLELEMENTEVENTS_ONMOUSEDOWN, _T("td"), OnBodyLButtonDown)
	DHTML_EVENT_TAG(DISPID_HTMLELEMENTEVENTS_ONMOUSEUP, _T("td"), OnBodyLButtonUp)
	DHTML_EVENT_TAG(DISPID_HTMLELEMENTEVENTS_ONMOUSEMOVE, _T("td"), OnBodyLButtonMove)
	DHTML_EVENT_TAG(DISPID_HTMLELEMENTEVENTS_ONCONTEXTMENU, _T("td"), OnBodyContextMenu)
	DHTML_EVENT_TAG(DISPID_HTMLELEMENTEVENTS_ONMOUSEDOWN, _T("div"), OnBodyLButtonDown)
	DHTML_EVENT_TAG(DISPID_HTMLELEMENTEVENTS_ONMOUSEUP, _T("div"), OnBodyLButtonUp)
	DHTML_EVENT_TAG(DISPID_HTMLELEMENTEVENTS_ONMOUSEMOVE, _T("div"), OnBodyLButtonMove)
	DHTML_EVENT_TAG(DISPID_HTMLELEMENTEVENTS_ONCONTEXTMENU, _T("div"), OnBodyContextMenu)
END_DHTML_EVENT_MAP()



CWebInfoDlg::CWebInfoDlg(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CWebInfoDlg::IDD, NULL/*CWebInfoDlg::IDH*/, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWebInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWebInfoDlg, CDHtmlDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CWebInfoDlg message handlers

BOOL CWebInfoDlg::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	Navigate(_T("F:\\GoogleCode\\Beanstalkapp\\VS2005\\MFCControl\\WebInfo\\") _T("web\\WebInfo.html"));
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
		CDHtmlDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CWebInfoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
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

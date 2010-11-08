// TabCtrlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TabCtrlDlg.h"
#include ".\tabctrldlg.h"


// CTabCtrlDlg dialog

IMPLEMENT_DYNAMIC(CTabCtrlDlg, CDialog)
CTabCtrlDlg::CTabCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTabCtrlDlg::IDD, pParent)
{
}

CTabCtrlDlg::~CTabCtrlDlg()
{
}

void CTabCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_ctlTab);
}


BEGIN_MESSAGE_MAP(CTabCtrlDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CTabCtrlDlg message handlers

void CTabCtrlDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

BOOL CTabCtrlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_ctlTab.InsertItem(0, "系统诊断", 0);
	m_ctlTab.InsertItem(1, "病毒查杀", 1);
	m_ctlTab.InsertItem(2, "系统修复", 2);

	m_ctlTab.LoadImages(IDB_BUTTON, 2, IDB_TAB);
	m_ctlTab.SetTabVertical(TRUE);
	m_ctlTab.SetTextColor(RGB(0xff, 0xff, 0xff), RGB(0xff, 0xff, 0xff));
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

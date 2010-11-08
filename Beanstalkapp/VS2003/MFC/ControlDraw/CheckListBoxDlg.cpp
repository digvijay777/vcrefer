// CheckListBoxDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ControlDraw.h"
#include "CheckListBoxDlg.h"
#include ".\checklistboxdlg.h"


// CCheckListBoxDlg dialog

IMPLEMENT_DYNAMIC(CCheckListBoxDlg, CDialog)
CCheckListBoxDlg::CCheckListBoxDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCheckListBoxDlg::IDD, pParent)
{
}

CCheckListBoxDlg::~CCheckListBoxDlg()
{
}

void CCheckListBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ctlList);
}


BEGIN_MESSAGE_MAP(CCheckListBoxDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_LBN_SELCHANGE(IDC_LIST1, OnLbnSelchangeList1)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CCheckListBoxDlg message handlers

void CCheckListBoxDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

BOOL CCheckListBoxDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_ctlList.AddItem("test1", FALSE, 0);
	m_ctlList.AddItem("test2", FALSE, 1, "test");
	m_ctlList.AddItem("test3", FALSE, 0, "test");

	m_ctlList.SetImageList(IDB_CHECK, IDB_CHECKINGSTATE);
	m_ctlList.SetLinkMessage(WM_USER+1);
	m_ctlList.SetTextColor(RGB(0x7d, 0x7d, 0x7d));
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCheckListBoxDlg::OnLbnSelchangeList1()
{
	// TODO: Add your control notification handler code here
}

void CCheckListBoxDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

LRESULT CCheckListBoxDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if((WM_USER + 1) == message)
	{
		CString		str;

		str.Format("单了第几项%d", wParam);
		MessageBox(str);
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

// SysRepairDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ControlDraw.h"
#include "SysRepairDlg.h"
#include ".\sysrepairdlg.h"


// CSysRepairDlg dialog

IMPLEMENT_DYNAMIC(CSysRepairDlg, CDialog)
CSysRepairDlg::CSysRepairDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSysRepairDlg::IDD, pParent)
{
}

CSysRepairDlg::~CSysRepairDlg()
{
}

void CSysRepairDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ctlSafe);
}


BEGIN_MESSAGE_MAP(CSysRepairDlg, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, OnLbnSelchangeList1)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTNTEST, OnBnClickedBtntest)
END_MESSAGE_MAP()


// CSysRepairDlg message handlers

void CSysRepairDlg::OnLbnSelchangeList1()
{
	// TODO: Add your control notification handler code here
}

void CSysRepairDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	// OnOK();
	static	int		n  = 0;

	m_ctlSafe.AddSafeItem(FALSE, "safe1", "describ1", "π‹¿Ì÷––ƒ", n++);
	m_ctlSafe.AddSafeItem(TRUE, "safe2", "ªÍ«£√Œ›”≥Ø«ÿƒ∫≥˛ªÍ«£√Œ›”ªÍ«£√Œ›”ªÍ«£√Œ›”ªÍ«£√Œ›”≥Ø«ÿƒ∫≥˛", "ø™∆Ù", n++);
}

void CSysRepairDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CSysRepairDlg::OnBnClickedBtntest()
{
	m_ctlSafe.ClearSafeItem();
}

BOOL CSysRepairDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_ctlSafe.SetImageList(IDB_SAFESTATE, IDB_BUTTON);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

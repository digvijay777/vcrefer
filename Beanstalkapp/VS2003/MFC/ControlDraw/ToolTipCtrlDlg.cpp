// ToolTipCtrlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ControlDraw.h"
#include "ToolTipCtrlDlg.h"
#include ".\tooltipctrldlg.h"


// CToolTipCtrlDlg dialog

IMPLEMENT_DYNAMIC(CToolTipCtrlDlg, CDialog)
CToolTipCtrlDlg::CToolTipCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CToolTipCtrlDlg::IDD, pParent)
{
}

CToolTipCtrlDlg::~CToolTipCtrlDlg()
{
}

void CToolTipCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC1, m_ctlStatic);
}


BEGIN_MESSAGE_MAP(CToolTipCtrlDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CToolTipCtrlDlg message handlers

void CToolTipCtrlDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

BOOL CToolTipCtrlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_tooltip.Create(/*GetDlgItem(IDC_EDIT1)*/NULL);
// 	m_tooltip.Activate(TRUE);

	m_tooltip.AddTool(GetDlgItem(IDC_EDIT1), "这是提示测式");
// 	m_tooltip.AddTool(GetDlgItem(IDC_STATIC1), "当前分数 99 分");
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CToolTipCtrlDlg::PreTranslateMessage(MSG* pMsg)
{
	if(NULL != m_tooltip.GetSafeHwnd())
		m_tooltip.RelayEvent(pMsg);

	return CDialog::PreTranslateMessage(pMsg);
}

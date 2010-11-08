// CtrlListBoxDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ControlDraw.h"
#include "CtrlListBoxDlg.h"
#include ".\ctrllistboxdlg.h"


// CCtrlListBoxDlg dialog

IMPLEMENT_DYNAMIC(CCtrlListBoxDlg, CDialog)
CCtrlListBoxDlg::CCtrlListBoxDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCtrlListBoxDlg::IDD, pParent)
{
}

CCtrlListBoxDlg::~CCtrlListBoxDlg()
{
}

void CCtrlListBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(CCtrlListBoxDlg, CDialog)
END_MESSAGE_MAP()


// CCtrlListBoxDlg message handlers

BOOL CCtrlListBoxDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CSkyMonListBox::MONLISTDATA		item1		= {"title1", "detail1", "(on)", "(off)", "turnon", "turnoff", 0, 0};
	CSkyMonListBox::MONLISTDATA		item2		= {"title2", "detail2", "(on)", "(off)", "turnon", "turnoff", 1, 1};
	CSkyMonListBox::MONLISTDATA		item3		= {"title3", "detail3", "(on)", "(off)", "turnon", "turnoff", 2, 1};
	CSkyMonListBox::MONLISTDATA		item4		= {"title4", "detail4", "(on)", "(off)", "turnon", "turnoff", 3, 0};
	CSkyMonListBox::MONLISTDATA		item5		= {"title5", "detail5 fdsa asdfasd fdsaf dfsaf 123 355 654 698 fdasf fdaf fdsaf", "(on)", "(off)", "turnon", "turnoff", 4, 0};

	m_list.AddMonItem(&item1);
	m_list.AddMonItem(&item2);
	m_list.AddMonItem(&item3);
	m_list.AddMonItem(&item4);
	m_list.AddMonItem(&item5);

	m_list.SetBitmaps(IDB_LISTSTATEMAX, IDB_LISTSTATEMIN, IDB_LISTSELBK);
	m_list.SetCallBackMsg(UM_SELECTITEM);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CCtrlListBoxDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(UM_SELECTITEM == message)
	{
		CSkyMonListBox::MONLISTDATA		data;

		data.bRunning = !lParam;
		m_list.SetMonItem(wParam, &data, MD_RUNNING);
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

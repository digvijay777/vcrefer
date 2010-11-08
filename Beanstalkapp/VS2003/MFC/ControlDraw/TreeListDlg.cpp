// TreeListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ControlDraw.h"
#include "TreeListDlg.h"
#include ".\treelistdlg.h"


// CTreeListDlg dialog

IMPLEMENT_DYNAMIC(CTreeListDlg, CDialog)
CTreeListDlg::CTreeListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTreeListDlg::IDD, pParent)
{
}

CTreeListDlg::~CTreeListDlg()
{
}

void CTreeListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1/*IDC_TREE1*/, m_treelist);
}


BEGIN_MESSAGE_MAP(CTreeListDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
END_MESSAGE_MAP()


// CTreeListDlg message handlers

BOOL CTreeListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_treelist.InsertColumn(0, "��Ŀһ", 0, 100);
	m_treelist.InsertColumn(1, "��ϸ��Ϣ", 0, 200);

	HTREELIST		hTree, hTree1;

	m_treelist.InsertItem("����Ŀһ", 0);
	hTree = m_treelist.InsertItem("����Ŀ��", 0);
	m_treelist.SetItemText(hTree, 1, "��������ϸ˵���ĵ�");
	m_treelist.InsertItem("����Ŀһ", 1, hTree);
	m_treelist.InsertItem("����Ŀ��", 1, hTree);
	m_treelist.InsertItem("����Ŀ��", 1, hTree);
	m_treelist.InsertItem("����Ŀ��", 1, hTree);
	m_treelist.InsertItem("����Ŀ��", 0);
	m_treelist.InsertItem("����Ŀ��", 0);
	hTree = m_treelist.InsertItem("����Ŀ��", 0, 0);
	m_treelist.SetItemText(hTree, 1, "��������ϸ˵���ĵ�");
	m_treelist.InsertItem("����Ŀһ", 1, hTree);
	m_treelist.InsertItem("����Ŀ��", 1, hTree);
	m_treelist.InsertItem("����Ŀ��", 1, hTree);
	hTree = m_treelist.InsertItem("����Ŀ��", 0, 0);
	hTree = m_treelist.InsertItem("����Ŀ��", 0, 0);
	hTree = m_treelist.InsertItem("����Ŀ��", 0, 0);
	hTree = m_treelist.InsertItem("����Ŀ��", 0, 0);
	m_treelist.SetItemText(hTree, 1, "��������ϸ˵���ĵ�");
	m_treelist.InsertItem("����Ŀһ", 1, hTree);
	m_treelist.InsertItem("����Ŀ��", 1, hTree);
	m_treelist.InsertItem("����Ŀ��", 1, hTree);
	m_treelist.InsertItem("����Ŀ��", 1, hTree);
	m_treelist.InsertItem("����Ŀ��", 1, hTree);
	m_treelist.InsertItem("����Ŀ��", 1, hTree);
	hTree1 = m_treelist.InsertItem("����Ŀ��", 1, hTree);
	m_treelist.SetItemText(hTree1, 1, "�����и������ϸ˵��");
	m_treelist.InsertItem("�δ���Ŀһ", 2, hTree1);
	m_treelist.InsertItem("�δ���Ŀ��", 2, hTree1);
	m_treelist.InsertItem("�δ���Ŀ��", 2, hTree1);
	m_treelist.InsertItem("����Ŀ��", 1, hTree);
	m_treelist.InsertItem("����Ŀ��", 1, hTree);
	hTree = m_treelist.InsertItem("����Ŀʮ", 0, 0);

	CBitmap		bitmap;
	BITMAP		bmp;

	bitmap.LoadBitmap(IDB_FOLDERICON);
	bitmap.GetBitmap(&bmp);
	m_imglist.Create(bmp.bmHeight, bmp.bmHeight, ILC_COLOR24|ILC_MASK, 0, 0);
	m_imglist.Add(&bitmap, RGB(0xff, 0x0, 0xff));

	m_treelist.SetImageList(&m_imglist, LVSIL_SMALL);

	m_treelist.UpdateData();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTreeListDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CTreeListDlg::OnBnClickedButton4()
{
	m_treelist.CheckAll(TRUE);
}

void CTreeListDlg::OnBnClickedButton2()
{
	m_treelist.CheckAll(FALSE);
}

void CTreeListDlg::OnBnClickedButton3()
{
	m_treelist.SetCheck(3, TRUE);
}

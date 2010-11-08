// TreeTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ControlDraw.h"
#include "TreeTestDlg.h"
#include ".\treetestdlg.h"

#define TREE_ICON_COMPUTER			6
#define TREE_ICON_DISK				3
#define TREE_ICON_FOLDER			0
#define TREE_ICON_MYDOCUMENT		8
// CTreeTestDlg dialog

IMPLEMENT_DYNAMIC(CTreeTestDlg, CDialog)
CTreeTestDlg::CTreeTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTreeTestDlg::IDD, pParent)
{
	m_hTreeItem = NULL;
}

CTreeTestDlg::~CTreeTestDlg()
{
}

void CTreeTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_ctlTree);
	DDX_Control(pDX, IDC_TREE2, m_ctlFolder);
}


BEGIN_MESSAGE_MAP(CTreeTestDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnBnClickedButton6)
END_MESSAGE_MAP()


// CTreeTestDlg message handlers

BOOL CTreeTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_ctlTree.InitImageList(IDB_TREEICON, IDB_FOLDERICON);

	m_ctlTree.InsertItem("我的文档", TREE_ICON_MYDOCUMENT, TREE_ICON_MYDOCUMENT, NULL);

	HTREEITEM		hRoot	= m_ctlTree.InsertItem("我的电脑", TREE_ICON_COMPUTER, TREE_ICON_COMPUTER, NULL);
	HTREEITEM		hTreeC	= m_ctlTree.InsertItem("C:[系统]", TREE_ICON_DISK, TREE_ICON_DISK, hRoot);
	HTREEITEM		hTreeD	= m_ctlTree.InsertItem("D:[误乐]", TREE_ICON_DISK, TREE_ICON_DISK, hRoot);

	m_ctlTree.InsertItem("Windows", TREE_ICON_FOLDER, TREE_ICON_FOLDER, hTreeC);
	m_ctlTree.InsertItem("Programer files", TREE_ICON_FOLDER, TREE_ICON_FOLDER, hTreeC);
	m_ctlTree.InsertItem("Boot", TREE_ICON_FOLDER, TREE_ICON_FOLDER, hTreeC);
	m_hTreeItem = m_ctlTree.InsertItem("游戏", TREE_ICON_FOLDER, TREE_ICON_FOLDER, hTreeD);
	m_ctlTree.InsertItem("电影", TREE_ICON_FOLDER, TREE_ICON_FOLDER, hTreeD);

	m_ctlFolder.InitFolder(IDB_TREEICON, IDB_FOLDERICON);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTreeTestDlg::OnBnClickedButton1()
{
	m_ctlTree.InsertItem("测试一", TREE_ICON_FOLDER, TREE_ICON_FOLDER, m_hTreeItem);
	m_ctlTree.Invalidate();
}

void CTreeTestDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CTreeTestDlg::OnBnClickedButton3()
{
	m_ctlFolder.CheckMyDocument();
}

void CTreeTestDlg::OnBnClickedButton2()
{
	m_ctlFolder.CheckAll(TRUE);
}

void CTreeTestDlg::OnBnClickedButton4()
{
	m_ctlFolder.CheckMemory();
}

void CTreeTestDlg::OnBnClickedButton5()
{
	if(FALSE != m_ctlFolder.IsSelectMemory())
	{
		AfxMessageBox("选中内存");
	}
	else
	{
		AfxMessageBox("没有选中内存");
	}
}

void CTreeTestDlg::OnBnClickedButton6()
{
	std::vector<std::string>		vctFolder;
	CString							strFolder;

	m_ctlFolder.GetSelectFolder(vctFolder);
	for(int i = 0; i < vctFolder.size(); i++)
	{
		strFolder += vctFolder[i].c_str();
		strFolder += "; ";
	}
	
	AfxMessageBox(strFolder);
}

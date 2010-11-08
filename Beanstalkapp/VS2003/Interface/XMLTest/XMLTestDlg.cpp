// XMLTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "XMLTest.h"
#include "XMLTestDlg.h"
#include ".\xmltestdlg.h"

#import "msxml6.dll" named_guids raw_interfaces_only
#include <msxml2.h>

#pragma comment(lib, "comsuppw.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CXMLTestDlg 对话框



CXMLTestDlg::CXMLTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CXMLTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CXMLTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CXMLTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CXMLTestDlg 消息处理程序

BOOL CXMLTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将\“关于...\”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SetDlgItemText(IDC_EDIT1, _T("<root size='100px'>\r\n\t<test1>data1</test1>\r\n\t<test2 size='300px'>data2</test2>\
		\r\n\t<test3>\r\n\t\t<test31 size='233' heigth='100'></test31>\r\n\t</test3>\r\n</root>"));
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CXMLTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CXMLTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CXMLTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CXMLTestDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
	MSXML2::IXMLDOMDocumentPtr			pDoc				= NULL;
	MSXML2::IXMLDOMNodeListPtr			pNodeList			= NULL;
	MSXML2::IXMLDOMNodePtr				pNode				= NULL;
	MSXML2::IXMLDOMNodePtr				pRoot				= NULL;
	MSXML2::IXMLDOMNamedNodeMapPtr		pAttr				= NULL;
	CTreeCtrl							*tree				= NULL;
	TCHAR								xmlBuffer[1024]		= {0};
	HRESULT								hr					= E_FAIL;
	VARIANT_BOOL						isSuccessful		= VARIANT_FALSE;
	_bstr_t								bstrXML;
	BSTR								bstr;
	HTREEITEM							treeRoot			= NULL;
	LONG								length				= 0;

	::CoInitialize(NULL);
	GetDlgItemText(IDC_EDIT1, xmlBuffer, 1023);
	if(_tcslen(xmlBuffer) < 7)
	{
		AfxMessageBox(_T("错误的输入"));
		goto EndCom;
	}
	tree = (CTreeCtrl*)GetDlgItem(IDC_TREE1);
	if(NULL == tree)
	{
		AfxMessageBox(_T("得到树形句柄失败"));
		goto EndCom;
	}
	tree->DeleteAllItems();
	//////////////////////////////////////////////////////////////////////////
	hr = ::CoCreateInstance(MSXML2::CLSID_DOMDocument60, NULL, CLSCTX_INPROC_SERVER,
		MSXML2::IID_IXMLDOMDocument, (void**)&pDoc);
	if(FAILED(hr))
	{
		AfxMessageBox(_T("获取接口IXMLDOMDocument失败"));
		goto EndCom;
	}

	//////////////////////////////////////////////////////////////////////////
	bstrXML = xmlBuffer;
	pDoc->put_async(VARIANT_FALSE);
	pDoc->loadXML(bstrXML, &isSuccessful);
	//////////////////////////////////////////////////////////////////////////
	pDoc->get_firstChild(&pRoot);
	if(NULL == pRoot)
		goto EndOpt;
	ShowTree(tree, NULL, (LPVOID)pRoot);
//	treeRoot = tree->InsertItem(GetNodeAttribute((LPVOID)pRoot));
/*	pRoot->get_nodeName(&bstr);
	treeRoot = tree->InsertItem((LPCTSTR)(_bstr_t)bstr);
	
	pRoot->get_attributes(&pAttr);
	pAttr->get_length(&length);
	if(length > 0)
	{
		CString		strAttr;
		strAttr = bstr;
		strAttr += _T("[");
		for(int i = 0; pAttr->nextNode(&pNode), pNode; ++i)
		{
			BSTR baseName;
			pNode->get_baseName(&baseName);
			BSTR text;
			pNode->get_text(&text);
			pNode.Release();
			if(i > 0)
				strAttr += _T(";");
			strAttr += baseName;
			strAttr += _T(":");
			strAttr += text;
			::SysFreeString(baseName);
			::SysFreeString(text);
		}
		strAttr += _T("]");
		tree->SetItemText(treeRoot, strAttr);
	}*/
	//////////////////////////////////////////////////////////////////////////
//	pRoot->get_childNodes(&pNodeList);
//	pNodeList->get_length(&length);
//	if(length < 0)
//		goto EndOpt;

	//////////////////////////////////////////////////////////////////////////	
EndOpt:
	if(NULL != pDoc)
		pDoc.Release();
	if(NULL != pNodeList)
		pNodeList.Release();
	if(NULL != pNode)
		pNode.Release();
	if(NULL != pAttr)
		pAttr.Release();
	if(NULL != pRoot)
		pRoot.Release();
EndCom:
	::CoUninitialize();
}

CString CXMLTestDlg::GetNodeAttribute(LPVOID node)
{
	CString								strAttr;
	BSTR								bstr;
	MSXML2::IXMLDOMNodePtr				pRoot				= NULL;
	MSXML2::IXMLDOMNamedNodeMapPtr		pAttr				= NULL;
	MSXML2::IXMLDOMNodePtr				pNode				= NULL;
	LONG								length				= 0;

	pRoot = (MSXML2::IXMLDOMNode*)node;
	pRoot->get_nodeName(&bstr);
	strAttr = bstr;
	pRoot->get_attributes(&pAttr);
	if(NULL == pAttr)
		return strAttr;
	pAttr->get_length(&length);
	if(length > 0)
	{
		strAttr += _T("[");
		for(int i = 0; pAttr->nextNode(&pNode), pNode; ++i)
		{
			BSTR baseName;
			pNode->get_baseName(&baseName);
			BSTR text;
			pNode->get_text(&text);
			pNode.Release();
			if(i > 0)
				strAttr += _T(";");
			strAttr += baseName;
			strAttr += _T(":\"");
			strAttr += text;
			strAttr += _T("\"");
			::SysFreeString(baseName);
			::SysFreeString(text);
		}
		strAttr += _T("]");
	}
	pAttr.Release();
	return strAttr;
}

void CXMLTestDlg::ShowTree(CTreeCtrl* tree, HTREEITEM hTree, LPVOID node)
{
	BSTR								bstr;
	HTREEITEM							treeItem	= NULL;
	MSXML2::IXMLDOMNodePtr				pRoot		= NULL;
	MSXML2::IXMLDOMNodePtr				pNode		= NULL;
	MSXML2::IXMLDOMNodeListPtr			pList		= NULL;
	MSXML2::DOMNodeType					nodeType;
	LONG								length		= 0;
    
	pRoot = (MSXML2::IXMLDOMNode*)node;
	pRoot->get_nodeType(&nodeType);
	if(nodeType != MSXML2::NODE_ELEMENT)
	{
		CString			text;
		pRoot->get_text(&bstr);
		text.Format(_T("Data:%s"), (LPCSTR)(_bstr_t)bstr);
		tree->InsertItem(text, hTree);
		return;
	}

	pRoot->get_childNodes(&pList);
	if(NULL == pList)
	{
		return;
	}
	pList->get_length(&length);
	treeItem = tree->InsertItem(GetNodeAttribute((LPVOID)node), hTree);
	if(length > 0)
	{
		for(int i = 0; pList->nextNode(&pNode), pNode; i++)
		{
			ShowTree(tree, treeItem, pNode);
		}
	}
	pList.Release();
}

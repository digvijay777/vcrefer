// CommonTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CommonTest.h"
#include "CommonTestDlg.h"
#include ".\commontestdlg.h"

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


// CCommonTestDlg 对话框



CCommonTestDlg::CCommonTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCommonTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCommonTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCommonTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CCommonTestDlg 消息处理程序

BOOL CCommonTestDlg::OnInitDialog()
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
	
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CCommonTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCommonTestDlg::OnPaint() 
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
HCURSOR CCommonTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCommonTestDlg::OnBnClickedButton1()
{
	// COM ERROR TEST
	CoInitialize(NULL);

	CLSID			iid;
	HRESULT			hres;

// 	hres = CLSIDFromProgID(OLESTR("SafeArray.ErrTest2"), &iid);
	hres = CLSIDFromProgID(OLESTR("SafeArray.ErrTest"), &iid);
	
	if(FAILED(hres))
	{
		CoUninitialize();
		return;
	}

	LPUNKNOWN		pUkn	= NULL;
	hres = CoCreateInstance(iid, NULL, CLSCTX_INPROC_SERVER, IID_IUnknown, (void **)&pUkn);
	if(FAILED(hres))
	{
		CoUninitialize();
		return;
	}

	LPDISPATCH		pDisp	= NULL;
	hres = pUkn->QueryInterface(IID_IDispatch, (void **)&pDisp);
	pUkn->Release();
	if(FAILED(hres))
	{
		CoUninitialize();
		return;
	}

	DISPID		disp;
	BSTR		bstr = L"ErrTest";
	hres = pDisp->GetIDsOfNames(IID_NULL, &bstr, 1, LOCALE_USER_DEFAULT, &disp);
	if(FAILED(hres))
	{
		pDisp->Release();
		CoUninitialize();
		return;
	}
	
	DISPPARAMS		param = {0};
	VARIANT			var;
	hres = pDisp->Invoke(disp, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &param, &var, NULL, NULL);
	if(FAILED(hres))
	{
		IErrorInfo   *pinfo;
		hres = ::GetErrorInfo(0, &pinfo);
		if(SUCCEEDED(hres))
		{
			BSTR		bstr;
			hres = pinfo->GetDescription(&bstr);
			//pinfo->GetHelpContext()
			if(SUCCEEDED(hres))
				MessageBoxW(m_hWnd, bstr, L"Error", MB_OK | MB_ICONERROR);
			::SysFreeString(bstr);
		}
	}

	pDisp->Release();
	CoUninitialize();
}

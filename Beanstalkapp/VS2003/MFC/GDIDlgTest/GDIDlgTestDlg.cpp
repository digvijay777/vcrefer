// GDIDlgTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GDIDlgTest.h"
#include "GDIDlgTestDlg.h"
#include ".\gdidlgtestdlg.h"





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


// CGDIDlgTestDlg 对话框



CGDIDlgTestDlg::CGDIDlgTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGDIDlgTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGDIDlgTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGDIDlgTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_MOVE()
END_MESSAGE_MAP()


// CGDIDlgTestDlg 消息处理程序

BOOL CGDIDlgTestDlg::OnInitDialog()
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
	
// 	DWORD dwExStyle=GetWindowLong(m_hWnd,GWL_EXSTYLE);
// 	SetWindowLong(m_hWnd,GWL_EXSTYLE,dwExStyle^0x80000); 
// 
// 	HDC hdcTemp = GetDC()->m_hDC;
// 	m_hdcMemory=CreateCompatibleDC(hdcTemp);
// 	HBITMAP hBitMap=CreateCompatibleBitmap(hdcTemp,500,500);
// 	SelectObject(m_hdcMemory,hBitMap);
// 
// 	//使用GDI+载入PNG图片
// 	HDC hdcScreen=::GetDC (m_hWnd);
// 	RECT rct;
// 	GetWindowRect(&rct);
// 	POINT ptWinPos={rct.left,rct.top};
// 	Graphics graph(m_hdcMemory);   //GDI+中的类
// // 	Image image(L"bk.png",TRUE);   //GDI+中的类
// // 	graph.DrawImage(&image,0,0,267,154); //后面两个参数要设置成跟图片一样大小，否则会失真
// 	SolidBrush		brush(Color(100, 255, 0, 255));
// 
// 	graph.FillRectangle(&brush, 0, 0, 267, 154);
// 
// 	//窗口透明贴图：
// 		//使用UpdateLayerWindow进行窗口透明处理
// 	HMODULE hFuncInst=LoadLibrary("User32.DLL");
// 	typedef BOOL (WINAPI *MYFUNC)(HWND,HDC,POINT*,SIZE*,HDC,POINT*,COLORREF,BLENDFUNCTION*,DWORD);          
// 	MYFUNC UpdateLayeredWindow;
// 	UpdateLayeredWindow=(MYFUNC)GetProcAddress(hFuncInst,"UpdateLayeredWindow");
// 	SIZE sizeWindow={267,154};
// 	POINT ptSrc={0,0};
// 	UpdateLayeredWindow( m_hWnd,hdcScreen,&ptWinPos,&sizeWindow,m_hdcMemory,&ptSrc,0,&m_Blend,2); 
// 
// 	dwExStyle=GetWindowLong(m_hWnd,GWL_EXSTYLE);
// 	SetWindowLong(m_hWnd,GWL_EXSTYLE,dwExStyle|0x80000);
// 
// 	//设置透明色：
// 		//用SetLayeredWindowAttributes设置透明色为0，它比UpdateLayeredWindow的使用要简单些
// 		HMODULE hInst=LoadLibrary("User32.DLL");
// 	typedef BOOL (WINAPI *MYFUNC2)(HWND,COLORREF,BYTE,DWORD);
// 	MYFUNC2 SetLayeredWindowAttributes = NULL;
// 	SetLayeredWindowAttributes=(MYFUNC2)GetProcAddress(hInst, "SetLayeredWindowAttributes");
// 	SetLayeredWindowAttributes(this->GetSafeHwnd(),0xff00ff,0,1);
// 	FreeLibrary(hInst);
// 	CRect		rect;
// 
// 	GetWindowRect(&rect);
// 	ScreenToClient(&rect);
// 	GetDC()->FillSolidRect(rect, RGB(0xff, 0, 0));;
//	::SetLayeredWindowAttributes(this->GetSafeHwnd(), RGB(0xff, 0, 0), 0, LWA_COLORKEY);
// 	::SetLayeredWindowAttributes(this->GetSafeHwnd(), 0, 255, 2);
	 m_Blend.BlendOp=0; //theonlyBlendOpdefinedinWindows2000
	 m_Blend.BlendFlags=0; //nothingelseisspecial...
	 m_Blend.AlphaFormat=1; //...
	 m_Blend.SourceConstantAlpha=255;//AC_SRC_ALPHA

	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CGDIDlgTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGDIDlgTestDlg::OnPaint() 
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
		SolidBrush		brush(Color(100, 255, 0, 255));
		SolidBrush		brush2(Color(50, 255, 0, 255));
		CRect			rect;
		CDC				memDC;
		CBitmap			bit;

		GetWindowRect(&rect);
		memDC.CreateCompatibleDC(GetDC());
		bit.CreateCompatibleBitmap(GetDC(), rect.Width(), rect.Height());
		CBitmap*		pOld = memDC.SelectObject(&bit);
		//	ScreenToClient(&rect);


		//	pDC->FillSolidRect(&rect, RGB(255, 0, 255));
		// 	pDC->FillRect(&rect, NULL);

		Graphics		graphics(memDC.m_hDC);
		POINT			pt		= {rect.left, rect.top};
		SIZE			sz		= {rect.Width(), rect.Height()};
		POINT			pt2		= {0, 0};

		// 	graphics.FillRectangle(&brush2, rect.left, rect.bottom, rect.Width(), rect.Height());
		// 	graphics.Clear(Color(50, 255, 255, 255));
		// 	graphics.FillRectangle(&brush, rect.left, rect.top, rect.Width(), rect.Height());
		graphics.FillRectangle(&brush2, 0, 0, rect.Width() / 2, rect.Height());
		graphics.FillRectangle(&brush, rect.Width() / 2, 0, rect.Width()/2, rect.Height());
	
		SendMessage(WM_PRINTCLIENT, (WPARAM)memDC.m_hDC, PRF_CHILDREN | PRF_CLIENT);
		// CDialog::OnPaint();
		ModifyStyleEx(0, WS_EX_LAYERED);
		::UpdateLayeredWindow(m_hWnd, GetDC()->GetSafeHdc(), &pt, &sz, memDC.m_hDC, &pt2, 0, &m_Blend, 2);
		// ModifyStyleEx(WS_EX_LAYERED, 0);
		memDC.SelectObject(pOld);

	}	
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CGDIDlgTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CGDIDlgTestDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return 	CDialog::OnEraseBkgnd(pDC);	
//	CDialog::OnEraseBkgnd(pDC);

	return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}

void CGDIDlgTestDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	// OnOK();
}

void CGDIDlgTestDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);


// 	BLENDFUNCTION 		bb;
// 	UpdateLayeredWindow(NULL, NULL, NULL, NULL, NULL, RGB(0xff, 0, 0), NULL, ULW_COLORKEY);
// 	::SetLayeredWindowAttributes(this->GetSafeHwnd(), 0, 100, 2);
}

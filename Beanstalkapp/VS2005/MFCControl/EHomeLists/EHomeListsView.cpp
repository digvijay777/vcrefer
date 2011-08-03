// EHomeListsView.cpp : CEHomeListsView 类的实现
//

#include "stdafx.h"
#include "EHomeLists.h"

#include "EHomeListsDoc.h"
#include "EHomeListsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEHomeListsView

IMPLEMENT_DYNCREATE(CEHomeListsView, CFormView)

BEGIN_MESSAGE_MAP(CEHomeListsView, CFormView)
	ON_MESSAGE(UM_EHOMELISTCLICKITEM, OnUmEHomeListClickItem)
	ON_MESSAGE(UM_EHOMELISTCLICKBUTTON, OnUmEHomeListClickMuiltButton)
END_MESSAGE_MAP()

// CEHomeListsView 构造/析构

CEHomeListsView::CEHomeListsView()
	: CFormView(CEHomeListsView::IDD)
{
	// TODO: 在此处添加构造代码
	m_listsoft.SetItemHeight(50);
	m_listurl.SetItemHeight(35);
	m_listtime.SetItemHeight(35);
	m_listtype.SetItemHeight(45);
	m_listsoftuse.SetItemHeight(50);
}

CEHomeListsView::~CEHomeListsView()
{
}

void CEHomeListsView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listsoft);
	DDX_Control(pDX, IDC_LIST2, m_listurl);
	DDX_Control(pDX, IDC_LIST3, m_listtime);
	DDX_Control(pDX, IDC_LIST4, m_listtype);
	DDX_Control(pDX, IDC_LIST5, m_listsoftuse);
}

BOOL CEHomeListsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CFormView::PreCreateWindow(cs);
}

void CEHomeListsView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
	//////////////////////////////////////////////////////////////////////////
	int			nItem			= 0;
	CBitmap		bmp;
	CBitmap		bmp2;
	CBitmap		bmp3;
	CBitmap		bmp4;
	CBitmap		bmp5;
	CBitmap		bmp6;

	bmp.LoadBitmap(IDB_BITMAP1);
	bmp2.LoadBitmap(IDB_BITMAP2);
	bmp3.LoadBitmap(IDB_BITMAP3);
	bmp4.LoadBitmap(IDB_BITMAP4);
	bmp5.LoadBitmap(IDB_BITMAP5);
	bmp6.LoadBitmap(IDB_BITMAP6);
	m_imagelist.Create(32, 32, ILC_COLOR24|ILC_MASK, 0, 0);
	m_imagelist2.Create(80, 24, ILC_COLOR24|ILC_MASK, 0, 0);
	m_imagelist3.Create(16, 16, ILC_COLOR24|ILC_MASK, 0, 0);
	m_imagelist4.Create(32, 32, ILC_COLOR24|ILC_MASK, 0, 0);
	m_imagelistscore.Create(85, 17, ILC_COLOR24|ILC_MASK, 0, 0);
	m_imagemuiltbutton.Create(65, 23, ILC_COLOR24|ILC_MASK, 0, 0);
	m_imagelist.Add(&bmp, RGB(0xff, 0x00, 0xff));
	m_imagelist2.Add(&bmp2, RGB(0xff, 0x00, 0xff));
	m_imagelist3.Add(&bmp3, RGB(0xff, 0x00, 0xff));
	m_imagelist4.Add(&bmp4, RGB(0xff, 0x00, 0xff));
	m_imagelistscore.Add(&bmp5, RGB(0xff, 0x00, 0xff));
	m_imagemuiltbutton.Add(&bmp6, RGB(0xff, 0x00, 0xff));
	//////////////////////////////////////////////////////////////////////////
	// 软件列表
	m_listsoft.InsertColumn(0, _T("软件名称"), LVCFMT_LEFT, 280);
	m_listsoft.InsertColumn(1, _T("分类"), LVCFMT_CENTER, 50);
	m_listsoft.InsertColumn(2, _T("黑名单"), 0, 120);

	m_listsoft.SetColumnFormat(0, EHLF_Title);
	m_listsoft.SetImageList(&m_imagelist, LVSIL_SMALL);
	m_listsoft.SetSwitchImageList(2, m_imagelist2.GetSafeHandle());

	nItem = m_listsoft.InsertItem(0, _T("飞信2011Beta夏舞\r\n免费短信 随时发送，无缝沟通新体验"), 0);
	m_listsoft.SetItemText(nItem, 1, _T("聊天"));
	m_listsoft.SetItemText(nItem, 2, _T("0"));
	nItem = m_listsoft.InsertItem(1, _T("Windows Live Messenger2009\r\n全求使用最广的通讯软件，办公人士必备"), 1);
	m_listsoft.SetItemText(nItem, 1, _T("聊天"));
	m_listsoft.SetItemText(nItem, 2, _T("0"));
	nItem = m_listsoft.InsertItem(2, _T("迅雷7.2.0.3076正式版\r\n迅雷最新版本，采用全新界面，提供更顺畅的下载"), 2);
	m_listsoft.SetItemText(nItem, 1, _T("下载"));
	m_listsoft.SetItemText(nItem, 2, _T("1"));
	//////////////////////////////////////////////////////////////////////////
	// 网址列表

	m_listurl.InsertColumn(0, _T("网址"), LVCFMT_LEFT, 180);
	m_listurl.InsertColumn(1, _T("网站"), LVCFMT_CENTER, 65);
	m_listurl.InsertColumn(2, _T("类型"), LVCFMT_CENTER, 85);
	m_listurl.InsertColumn(3, _T("最后浏览时间"), LVCFMT_CENTER, 95);
	m_listurl.InsertColumn(4, _T(" "), LVCFMT_CENTER, 100);

	m_listurl.SetColumnFormat(0, EHLF_Link);
	m_listurl.SetImageList(&m_imagelist3, LVSIL_SMALL);
	m_listurl.SetSwitchImageList(4, m_imagelist2.GetSafeHandle());

	nItem = m_listurl.InsertItem(0, _T("static.24quan.com"), 0);
	m_listurl.SetItemText(nItem, 1, _T("24券"));
	m_listurl.SetItemText(nItem, 2, _T("新闻"));
	m_listurl.SetItemText(nItem, 3, _T("18：19：41"));
	m_listurl.SetItemText(nItem, 4, _T("0"));
	nItem = m_listurl.InsertItem(1, _T("cdn.gaopeng.com"), 1);
	m_listurl.SetItemText(nItem, 1, _T("高鹏"));
	m_listurl.SetItemText(nItem, 2, _T("新闻"));
	m_listurl.SetItemText(nItem, 3, _T("18：19：41"));
	m_listurl.SetItemText(nItem, 4, _T("0"));	
	nItem = m_listurl.InsertItem(2, _T("www.xmbangban.com"), 2);
	m_listurl.SetItemText(nItem, 1, _T("厦门帮帮"));
	m_listurl.SetItemText(nItem, 2, _T("新闻"));
	m_listurl.SetItemText(nItem, 3, _T("18：19：41"));
	m_listurl.SetItemText(nItem, 4, _T("1"));
	nItem = m_listurl.InsertItem(2, _T("www.xmbangban.com"), 2);
	m_listurl.SetItemText(nItem, 1, _T("厦门帮帮"));
	m_listurl.SetItemText(nItem, 2, _T("新闻"));
	m_listurl.SetItemText(nItem, 3, _T("18：19：41"));
	m_listurl.SetItemText(nItem, 4, _T("1"));
	nItem = m_listurl.InsertItem(2, _T("www.xmbangban.com"), 2);
	m_listurl.SetItemText(nItem, 1, _T("厦门帮帮"));
	m_listurl.SetItemText(nItem, 2, _T("新闻"));
	m_listurl.SetItemText(nItem, 3, _T("18：19：41"));
	m_listurl.SetItemText(nItem, 4, _T("1"));
	nItem = m_listurl.InsertItem(2, _T("www.xmbangban.com"), 2);
	m_listurl.SetItemText(nItem, 1, _T("厦门帮帮"));
	m_listurl.SetItemText(nItem, 2, _T("新闻"));
	m_listurl.SetItemText(nItem, 3, _T("18：19：41"));
	m_listurl.SetItemText(nItem, 4, _T("1"));
	nItem = m_listurl.InsertItem(2, _T("www.xmbangban.com"), 2);
	m_listurl.SetItemText(nItem, 1, _T("厦门帮帮"));
	m_listurl.SetItemText(nItem, 2, _T("新闻"));
	m_listurl.SetItemText(nItem, 3, _T("18：19：41"));
	m_listurl.SetItemText(nItem, 4, _T("1"));
	//////////////////////////////////////////////////////////////////////////
	// 时间列表
	m_listtime.InsertColumn(0, _T("序号"), LVCFMT_CENTER, 50);
	m_listtime.InsertColumn(1, _T("时间"), LVCFMT_CENTER, 160);
	m_listtime.InsertColumn(2, _T("超始时间"), LVCFMT_CENTER, 75);
	m_listtime.InsertColumn(3, _T("结束时间"), LVCFMT_CENTER, 75);
	m_listtime.InsertColumn(4, _T("状态"), LVCFMT_CENTER, 130);

	m_listtime.SetSwitchImageList(4, m_imagelist2.GetSafeHandle());

	nItem = m_listtime.InsertItem(0, _T("1"));
	m_listtime.SetItemText(nItem, 1, _T("星期二至星期五"));
	m_listtime.SetItemText(nItem, 2, _T("09:12"));
	m_listtime.SetItemText(nItem, 3, _T("12:09"));
	m_listtime.SetItemText(nItem, 4, _T("1"));
	nItem = m_listtime.InsertItem(0, _T("2"));
	m_listtime.SetItemText(nItem, 1, _T("星期二至星期五"));
	m_listtime.SetItemText(nItem, 2, _T("09:12"));
	m_listtime.SetItemText(nItem, 3, _T("12:09"));
	m_listtime.SetItemText(nItem, 4, _T("1"));
	nItem = m_listtime.InsertItem(0, _T("3"));
	m_listtime.SetItemText(nItem, 1, _T("星期六至星期一"));
	m_listtime.SetItemText(nItem, 2, _T("18:30"));
	m_listtime.SetItemText(nItem, 3, _T("19:00"));
	m_listtime.SetItemText(nItem, 4, _T("0"));
	//////////////////////////////////////////////////////////////////////////
	// 分类列表
	m_listtype.InsertColumn(0, _T(""), LVCFMT_CENTER, 80);
	m_listtype.InsertColumn(1, _T(""), LVCFMT_CENTER, 220);
	m_listtype.InsertColumn(2, _T(""), LVCFMT_CENTER, 160);
	m_listtype.InsertColumn(3, _T(""), LVCFMT_CENTER, 130);

	m_listtype.SetColumnFormat(2, EHLF_Progress);
	m_listtype.SetSwitchImageList(3, m_imagelist2.GetSafeHandle());
	m_listtype.ModifyStyle(0, LVS_NOCOLUMNHEADER);

	nItem = m_listtype.InsertItem(0, _T("其它"));
	m_listtype.SetItemText(nItem, 1, _T("分类介绍"));
	m_listtype.SetItemText(nItem, 2, _T("50"));
	m_listtype.SetItemText(nItem, 3, _T("0"));
	nItem = m_listtype.InsertItem(0, _T("新闻"));
	m_listtype.SetItemText(nItem, 1, _T("分类介绍"));
	m_listtype.SetItemText(nItem, 2, _T("40"));
	m_listtype.SetItemText(nItem, 3, _T("0"));
	nItem = m_listtype.InsertItem(0, _T("娱乐"));
	m_listtype.SetItemText(nItem, 1, _T("分类介绍"));
	m_listtype.SetItemText(nItem, 2, _T("30"));
	m_listtype.SetItemText(nItem, 3, _T("0"));
	nItem = m_listtype.InsertItem(0, _T("媒体"));
	m_listtype.SetItemText(nItem, 1, _T("分类介绍"));
	m_listtype.SetItemText(nItem, 2, _T("20"));
	m_listtype.SetItemText(nItem, 3, _T("0"));
	nItem = m_listtype.InsertItem(0, _T("学习"));
	m_listtype.SetItemText(nItem, 1, _T("分类介绍"));
	m_listtype.SetItemText(nItem, 2, _T("10"));
	m_listtype.SetItemText(nItem, 3, _T("0"));
	nItem = m_listtype.InsertItem(0, _T("体育"));
	m_listtype.SetItemText(nItem, 1, _T("分类介绍"));
	m_listtype.SetItemText(nItem, 2, _T("5"));
	m_listtype.SetItemText(nItem, 3, _T("0"));
	nItem = m_listtype.InsertItem(0, _T("其它"));
	m_listtype.SetItemText(nItem, 1, _T("分类介绍"));
	m_listtype.SetItemText(nItem, 2, _T("1"));
	m_listtype.SetItemText(nItem, 3, _T("0"));
	nItem = m_listtype.InsertItem(0, _T("IT"));
	m_listtype.SetItemText(nItem, 1, _T("分类介绍"));
	m_listtype.SetItemText(nItem, 2, _T("1"));
	m_listtype.SetItemText(nItem, 3, _T("0"));

	//////////////////////////////////////////////////////////////////////////
	// 软件使用列表
	m_listsoftuse.InsertColumn(0, _T("软件名称"), LVCFMT_CENTER, 360);
	m_listsoftuse.InsertColumn(1, _T("安装时间"), LVCFMT_CENTER, 90);
	m_listsoftuse.InsertColumn(2, _T("使用频率"), LVCFMT_CENTER, 90);
	m_listsoftuse.InsertColumn(3, _T("软件评分"), LVCFMT_CENTER, 110);
	m_listsoftuse.InsertColumn(4, _T("操作"), LVCFMT_CENTER, 150);

	m_listsoftuse.SetColumnFormat(0, EHLF_Title);
	m_listsoftuse.SetImageList(&m_imagelist4, LVSIL_SMALL);
	m_listsoftuse.SetIconImageList(3, m_imagelistscore.GetSafeHandle());
	m_listsoftuse.SetMuiltButtonImageList(4, m_imagemuiltbutton.GetSafeHandle());

	nItem = m_listsoftuse.InsertItem(0, _T("Axure RP Pro 6\r\n一款办公自动化帮助工具。AxureRP允许程序设计"), 0);
	m_listsoftuse.SetItemText(nItem, 1, _T("2011-07-05"));
	m_listsoftuse.SetItemText(nItem, 2, _T("今天"));
	m_listsoftuse.SetItemText(nItem, 3, _T("0"));
	m_listsoftuse.SetItemText(nItem, 4, _T("1"));
	nItem = m_listsoftuse.InsertItem(0, _T("360安全卫士\r\n功能最强、效果最好、最受用户欢迎的必备安全软件"), 1);
	m_listsoftuse.SetItemText(nItem, 1, _T("2011-07-05"));
	m_listsoftuse.SetItemText(nItem, 2, _T("今天"));
	m_listsoftuse.SetItemText(nItem, 3, _T("10"));
	m_listsoftuse.SetItemText(nItem, 4, _T("3"));
	nItem = m_listsoftuse.InsertItem(0, _T("Bonjour\r\n苹果公司在开发Mac OS X10.2版本之后引入的服务"), 2);
	m_listsoftuse.SetItemText(nItem, 1, _T("2011-07-05"));
	m_listsoftuse.SetItemText(nItem, 2, _T("今天"));
	m_listsoftuse.SetItemText(nItem, 3, _T("9"));
	m_listsoftuse.SetItemText(nItem, 4, _T("3"));
	nItem = m_listsoftuse.InsertItem(0, _T("Microsoft .NET Framework 4 Client Profile\r\nMicrosoft 公司开发的移动应用架构平台。使用该架构"), 3);
	m_listsoftuse.SetItemText(nItem, 1, _T("2011-07-05"));
	m_listsoftuse.SetItemText(nItem, 2, _T("今天"));
	m_listsoftuse.SetItemText(nItem, 3, _T("7"));
	m_listsoftuse.SetItemText(nItem, 4, _T("2"));
}


// CEHomeListsView 诊断

#ifdef _DEBUG
void CEHomeListsView::AssertValid() const
{
	CFormView::AssertValid();
}

void CEHomeListsView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CEHomeListsDoc* CEHomeListsView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEHomeListsDoc)));
	return (CEHomeListsDoc*)m_pDocument;
}
#endif //_DEBUG


// CEHomeListsView 消息处理程序
LRESULT CEHomeListsView::OnUmEHomeListClickItem(WPARAM wParam, LPARAM lParam)
{
	HWND		hWnd		= (HWND)wParam;
	int			nItem		= LOWORD(lParam);
	int			nSubItem	= HIWORD(lParam);

	if(hWnd == m_listsoft.GetSafeHwnd())
	{
		CString		str;

		str.Format(_T("你单击了\"软件列表\"的第%d项%d列."), nItem, nSubItem);
		MessageBox(str);
	}
	else if(hWnd == m_listurl.GetSafeHwnd())
	{
		CString		str;

		str.Format(_T("你单击了\"网址列表\"的第%d项%d列."), nItem, nSubItem);
		MessageBox(str);
	}
	else if(hWnd == m_listtime.GetSafeHwnd())
	{
		CString		str;

		str.Format(_T("你单击了\"时间列表\"的第%d项%d列."), nItem, nSubItem);
		MessageBox(str);
	}
	else if(hWnd == m_listtype.GetSafeHwnd())
	{
		CString		str;

		str.Format(_T("你单击了\"类别列表\"的第%d项%d列."), nItem, nSubItem);
		MessageBox(str);
	}
	return 0;
}
// CEHomeListsView 消息处理程序
LRESULT CEHomeListsView::OnUmEHomeListClickMuiltButton(WPARAM wParam, LPARAM lParam)
{
	HWND		hWnd		= (HWND)wParam;
	int			nItem		= LOWORD(lParam);
	int			nIndex		= HIWORD(lParam);

	if(hWnd == m_listsoftuse.GetSafeHwnd())
	{
		CString		str;

		str.Format(_T("你单击了\"软件使用列表\"的第%d项%d个按钮."), nItem, nIndex);
		MessageBox(str);
	}
	return 0;
}

// DetFileView.cpp : 实现文件
//

#include "stdafx.h"
#include "Detours.h"
#include "DetFileView.h"
#include ".\detfileview.h"


FUN_CREATEFILE	CDetFileView::RealCreateFile = (FUN_CREATEFILE)CreateFile;
CDetFileView*	CDetFileView::s_this = NULL;		
// CDetFileView

IMPLEMENT_DYNCREATE(CDetFileView, CListView)

CDetFileView::CDetFileView()
{
	m_bIsDetours = false;
	s_this = this;
}

CDetFileView::~CDetFileView()
{
}

BEGIN_MESSAGE_MAP(CDetFileView, CListView)
	ON_WM_CREATE()
	ON_MESSAGE(UM_VIEW_ADDITEM, OnAddItem)
	ON_COMMAND(IDM_ADDDETOURS, OnAdddetours)
	ON_UPDATE_COMMAND_UI(IDM_ADDDETOURS, OnUpdateAdddetours)
END_MESSAGE_MAP()


// CDetFileView 诊断

#ifdef _DEBUG
void CDetFileView::AssertValid() const
{
	CListView::AssertValid();
}

void CDetFileView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG


// CDetFileView 消息处理程序

int CDetFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CListCtrl&		list = GetListCtrl();
	// 列表视图
	DWORD	dwStyle = GetWindowLong(m_hWnd, GWL_STYLE);
	DWORD	dwExStyle = list.GetExtendedStyle();

	dwStyle &= ~LVS_TYPEMASK;
	dwStyle |= LVS_REPORT;
	dwStyle |= LVS_SINGLESEL;
	SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
	dwExStyle |= LVS_EX_FULLROWSELECT;
	dwExStyle |= LVS_EX_GRIDLINES;
	list.SetExtendedStyle(dwExStyle);

	//列表头
	list.InsertColumn(0, "时间", LVCFMT_CENTER, 100);
	list.InsertColumn(1, "操作", LVCFMT_CENTER, 170);

	// 测试数据
	return 0;
}

LRESULT CDetFileView::OnAddItem(WPARAM wParam, LPARAM lParam)
{
	CTime		time	= CTime::GetCurrentTime();
	CString		szTime	= time.Format("%H:%M:%S");
	CListCtrl&	list	= GetListCtrl();
	
	int n = list.InsertItem(list.GetItemCount(), szTime);
	list.SetItemText(n, 1, (LPTSTR)wParam);
	return 0;
}

int CALLBACK MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	// lParamSort contains a pointer to the list view control.
	// The lParam of an item is just its index.
	CListCtrl* pListCtrl = (CListCtrl*) lParamSort;
// 	CString    strItem1 = pListCtrl->GetItemText(lParam1, 0);
// 	CString    strItem2 = pListCtrl->GetItemText(lParam2, 0);

	return /*strcmp(strItem2, strItem1)*/ lParam1 < lParam2;
}

void CDetFileView::OnAdddetours()
{
	// TODO: 在此添加命令处理程序代码
	// 排序
	CListCtrl&	list = GetListCtrl();
// 	list.SortItems(MyCompareProc, (DWORD_PTR)&list);
// 	return;
	if(false == m_bIsDetours)
	{
		// 开始监控
		LONG	lCode		= 0;

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		lCode = DetourAttach(&(PVOID&)RealCreateFile, (void *)DetourCreateFile);
		DetourTransactionCommit();		
		if(NO_ERROR == lCode)
			m_bIsDetours = true;
		else
			m_bIsDetours = false;
	}
	else
	{	
		// 取消监控
		LONG lCode			= 0;
		
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		lCode = DetourDetach(&(PVOID&)RealCreateFile, (void *)DetourCreateFile);
		DetourTransactionCommit();		
		if(NO_ERROR == lCode)
			m_bIsDetours = false;
		else
			m_bIsDetours = true;
	}

	HANDLE hand = CreateFile("c:\\tt.txt", GENERIC_READ, FILE_SHARE_READ, NULL, 
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(INVALID_HANDLE_VALUE == hand)
	{
		TRACE("filed open file.");
	}
	else
	{
		CloseHandle(hand);
	}
}

void CDetFileView::OnUpdateAdddetours(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(m_bIsDetours);
}

// 监控函数
HANDLE CDetFileView::DetourCreateFile(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, 
	LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDispostion , DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	if(NULL != s_this)
		s_this->SendMessage(UM_VIEW_ADDITEM, (WPARAM)lpFileName, 0);
	HANDLE handle = RealCreateFile(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, 
		dwCreationDispostion , dwFlagsAndAttributes, hTemplateFile);
	return handle;
}
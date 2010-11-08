// DetFileView.cpp : ʵ���ļ�
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


// CDetFileView ���

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


// CDetFileView ��Ϣ�������

int CDetFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CListCtrl&		list = GetListCtrl();
	// �б���ͼ
	DWORD	dwStyle = GetWindowLong(m_hWnd, GWL_STYLE);
	DWORD	dwExStyle = list.GetExtendedStyle();

	dwStyle &= ~LVS_TYPEMASK;
	dwStyle |= LVS_REPORT;
	dwStyle |= LVS_SINGLESEL;
	SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
	dwExStyle |= LVS_EX_FULLROWSELECT;
	dwExStyle |= LVS_EX_GRIDLINES;
	list.SetExtendedStyle(dwExStyle);

	//�б�ͷ
	list.InsertColumn(0, "ʱ��", LVCFMT_CENTER, 100);
	list.InsertColumn(1, "����", LVCFMT_CENTER, 170);

	// ��������
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
	// TODO: �ڴ���������������
	// ����
	CListCtrl&	list = GetListCtrl();
// 	list.SortItems(MyCompareProc, (DWORD_PTR)&list);
// 	return;
	if(false == m_bIsDetours)
	{
		// ��ʼ���
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
		// ȡ�����
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
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetCheck(m_bIsDetours);
}

// ��غ���
HANDLE CDetFileView::DetourCreateFile(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, 
	LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDispostion , DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	if(NULL != s_this)
		s_this->SendMessage(UM_VIEW_ADDITEM, (WPARAM)lpFileName, 0);
	HANDLE handle = RealCreateFile(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, 
		dwCreationDispostion , dwFlagsAndAttributes, hTemplateFile);
	return handle;
}
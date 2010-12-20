// HistoryListView.cpp : implementation file
//

#include "stdafx.h"
#include "BatchEmail.h"
#include "TaskListView.h"
#include "Mymsgdef.h"
#include ".\tasklistview.h"
#include "../../../ExtClass/QuickDomXml.h"

// CHistoryListView

IMPLEMENT_DYNCREATE(CTaskListView, CListView)

CTaskListView::CTaskListView()
{
}

CTaskListView::~CTaskListView()
{
}

BEGIN_MESSAGE_MAP(CTaskListView, CListView)
	ON_MESSAGE(UM_TL_UPDATE, OnTlUpdate)
END_MESSAGE_MAP()


// CHistoryListView diagnostics

#ifdef _DEBUG
void CTaskListView::AssertValid() const
{
	CListView::AssertValid();
}

void CTaskListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG


LRESULT CTaskListView::OnTlUpdate(WPARAM wParam, LPARAM lParam)
{
	CListCtrl&		list		= GetListCtrl();

	list.SetRedraw(FALSE);
	list.DeleteAllItems();

	//////////////////////////////////////////////////////////////////////////
	// 操作XML
	WCHAR		szFile[MAX_PATH]		= {0};
	HANDLE		hFind					= NULL;
	WIN32_FIND_DATAW		fd			= {0};

	theApp.ReSetCurrentDir();
	hFind = FindFirstFileW(L"xml\\*.xml", &fd);
	if(NULL == hFind || INVALID_HANDLE_VALUE == hFind)
	{
		list.SetRedraw(TRUE);
		return FALSE;
	}
	CoInitialize(NULL);
	do
	{
		if(FILE_ATTRIBUTE_DIRECTORY & fd.dwFileAttributes)
			continue;

		wcscpy(szFile, L"xml\\");
		wcscat(szFile, fd.cFileName);
		try
		{
			MSXML2::IXMLDOMDocumentPtr		spDoc;
			int								nItem;

			spDoc.CreateInstance("Msxml2.DOMDocument");
			spDoc->put_async(VARIANT_FALSE);
			spDoc->load(_variant_t(szFile));
			
			nItem = list.InsertItem(list.GetItemCount(), GetDomXMLStringA(spDoc, "task/config/TaskName"));
			if(1 == GetDomXMLLong(spDoc, "task/complete/allcompleted"))
			{
				list.SetItemText(nItem, 1, "执行完成");
			}
			else if(strcmp("", GetDomXMLStringA(spDoc, "task/complete/firsttime")) == 0)
			{
				list.SetItemText(nItem, 1, "尚未执行");
			}
			else
			{
				list.SetItemText(nItem, 1, "尚未完成");
			}
			list.SetItemText(nItem, 2, GetDomXMLStringA(spDoc, "task/complete/firsttime"));
			list.SetItemText(nItem, 3, GetDomXMLStringA(spDoc, "task/complete/lasttime"));
		}
		catch(_com_error& e)
		{
			WCHAR			szLog[1024]		= {0};

			_snwprintf(szLog, sizeof(szLog)/sizeof(WCHAR)
				, L"Open file '%s' failed: %s"
				, szFile, (WCHAR*)e.Description());
			MessageBoxW(GetSafeHwnd(), szLog, L"Error", MB_OK|MB_ICONERROR);
		}
	}while(FindNextFileW(hFind, &fd));
	FindClose(hFind);
	//////////////////////////////////////////////////////////////////////////
	list.SetRedraw(TRUE);
	return 0;
}

void CTaskListView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	CListCtrl&		list		= GetListCtrl();

	list.SetExtendedStyle(LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	list.InsertColumn(0, "任务名称", LVCFMT_LEFT, 120);
	list.InsertColumn(1, "执行状态", LVCFMT_LEFT, 120);
	list.InsertColumn(2, "添加时间", LVCFMT_LEFT, 120);
	list.InsertColumn(3, "第一次执行时间", LVCFMT_LEFT, 120);
	list.InsertColumn(4, "最后一次执行时间", LVCFMT_LEFT, 120);

	PostMessage(UM_TL_UPDATE);
}

BOOL CTaskListView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= LVS_REPORT;

	return CListView::PreCreateWindow(cs);
}

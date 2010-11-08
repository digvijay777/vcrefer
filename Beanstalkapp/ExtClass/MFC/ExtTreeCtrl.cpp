// ExtTreeCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "ExtTreeCtrl.h"
#include ".\exttreectrl.h"


// CExtTreeCtrl

IMPLEMENT_DYNAMIC(CExtTreeCtrl, CTreeCtrl)
CExtTreeCtrl::CExtTreeCtrl()
{
}

CExtTreeCtrl::~CExtTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CExtTreeCtrl, CTreeCtrl)
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnTvnItemexpanding)
END_MESSAGE_MAP()



// CExtTreeCtrl 消息处理程序


void CExtTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	UINT uFlags = 0;
	HTREEITEM hItem = HitTest(point, &uFlags);

	if(uFlags & TVHT_ONITEMSTATEICON)
	{
		SelectItem(hItem);
	} 

	CTreeCtrl::OnLButtonDown(nFlags, point);
}


void CExtTreeCtrl::OnTvnItemexpanding(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

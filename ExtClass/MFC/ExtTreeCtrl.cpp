// ExtTreeCtrl.cpp : ʵ���ļ�
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



// CExtTreeCtrl ��Ϣ�������


void CExtTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}

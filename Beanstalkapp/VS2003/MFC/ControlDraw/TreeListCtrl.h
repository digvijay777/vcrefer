#pragma once
#include <vector>
#include <string>

typedef struct tagTreeListNode
{
	// 文本
#if defined(_UNICODE) || defined(UNICODE)
	std::vector<std::wstring>			vTexts;
#else
	std::vector<std::string>			vTexts;
#endif

	BOOL								bExpend;		// 展开标记
	int									nState;			// 状态索引
	int									nImage;			// 图标索引
	LPARAM								lParam;			// 附加数据

	tagTreeListNode*					parent;			// 父节点
	std::vector<tagTreeListNode*>		childs;			// 子节点集
}TREELISTNODE, *HTREELIST;

typedef std::vector<HTREELIST>::iterator	TreeListPtr;

class CTreeList
{
public:
	CTreeList();
	virtual ~CTreeList();

private:
	TREELISTNODE		m_root;

	TreeListPtr		GetItemPtr(HTREELIST hParent, HTREELIST hItem);
	void			DeleteChildItems(HTREELIST hParent);
	void			GetChildVisibleItems(std::vector<HTREELIST>& vItems, HTREELIST hParent);
	int				GetChildCount(HTREELIST hParent);
public:
	HTREELIST		InsertItem(LPCTSTR lpText, int nImage, HTREELIST hParnet = NULL, HTREELIST hAfter = NULL);
	void			SetItemText(HTREELIST hItem, int nSubItem, LPCTSTR lpText);
	void			SetItemData(HTREELIST hItem, DWORD_PTR pData);
	void			SetItemState(HTREELIST hItem, int nState);
	void			SetItemImage(HTREELIST hItem, int nImage);
	void			GetItemText(HTREELIST hItem, int nSubItem, LPTSTR lpText, int nMaxLen);
	DWORD_PTR		GetItemData(HTREELIST hItem);
	int				GetItemState(HTREELIST hItem);
	int				GetItemImage(HTREELIST hItem);
	void			DeleteItem(HTREELIST hItem);
	void			DeleteAllItems();
	void			GetVisibleItems(std::vector<HTREELIST>& vItems);
	void			Expand(HTREELIST hItem, UINT nCode);
	HTREELIST		GetParentItem(HTREELIST hItem);
	int				GetCount();
public:
	static BOOL			ItemHasChildren(HTREELIST hItem);
	static int			GetTreeDegree(HTREELIST hItem);
	static HTREELIST	GetTopItem(HTREELIST hItem);
};
// CTreeListCtrl

class CTreeListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CTreeListCtrl)

public:
	CTreeListCtrl();
	virtual ~CTreeListCtrl();

private:
	CTreeList*				m_pData;
	CTreeList*				m_pOldData;
	std::vector<HTREELIST>	m_list;
	BOOL					m_bDataChange;

protected:
	void			InitTreeListCtrl();

	virtual void	DrawColumn_1(LPNMLVCUSTOMDRAW pNMCD);
	virtual int		DrawItemButton(CDC* pDC, RECT rect, BOOL bExpand);
	virtual int		DrawItemCheckBox(CDC* pDC, RECT rect, int nImg);
	virtual int		DrawItemIcon(CDC* pDC, RECT rect, int nImg);
	virtual void	GetItemCtrlRect(int nItem, LPRECT lpCheckBox, LPRECT lpButton);
public:
	HTREELIST		InsertItem(LPCTSTR lpText, int nImage, HTREELIST hParent = NULL, HTREELIST hAfter = NULL);
	BOOL			SetItemText(HTREELIST hItem, int nSubItem, LPCTSTR lpszText);
	BOOL			SelectData(CTreeList* pData);
	void			CheckAll(BOOL bCheck = TRUE);
	BOOL			GetCheck(HTREELIST hItem);
	BOOL			GetCheck(int nItem);
    void			SetCheck(HTREELIST hItem, BOOL bCheck = TRUE);
	void			SetCheck(int nItem, BOOL bCheck = TRUE);


	void			UpdateData(BOOL bForce = FALSE);
protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
public:
	afx_msg void OnLvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};



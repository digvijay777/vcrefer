/*
 * CMiniTabCtrl 控件
 *
 * 使用范围:TAB页比较少，且一次添加后不再更改的简单情况
 */
#pragma once
#include <vector>


class CMiniTabCtrl : public CWnd
{
	DECLARE_DYNAMIC(CMiniTabCtrl)

public:
	CMiniTabCtrl();
	virtual ~CMiniTabCtrl();
	
protected:
	DECLARE_MESSAGE_MAP()
private:
	CBrush		m_brushItemNormal;
	CBrush		m_brushItemHover;
	CBitmap		m_bmpItemHover;
	CBitmap		m_bmpItemNormal;
	CBitmap		m_bmpBk;
	CDC			m_dcMem;
	SIZE		m_sizeItem;
	COLORREF	m_color;

	std::vector<TCITEM>		m_vecTcitem;	// 不能超过10个
	int						m_nCurSel;
public:
	BOOL	InitTabCtrl(SIZE *siItem, UINT nIDNorBmp = -1, UINT nIDHovBmp = -1, UINT nIDBkBmp = -1);
	LONG	InsertItem(int nItem, LPCTSTR lpszItem, int nImage = -1);
	BOOL	SetItem(int nItem, TCITEM* pTabCtrlItem);		// 只适用mask=TCIF_PARAM的情况
	BOOL	GetItem(int nItem, TCITEM* pTabCtrlItem) const; // 目的是为以后兼容
	int		SetCurSel(int nIndex);
	int		GetCurSel() const;
	int		GetItemCount( ) const;
	void	SetTextColor(COLORREF& color);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};



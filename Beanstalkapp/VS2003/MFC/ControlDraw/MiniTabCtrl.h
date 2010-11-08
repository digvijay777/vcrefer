/*
 * CMiniTabCtrl �ؼ�
 *
 * ʹ�÷�Χ:TABҳ�Ƚ��٣���һ����Ӻ��ٸ��ĵļ����
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

	std::vector<TCITEM>		m_vecTcitem;	// ���ܳ���10��
	int						m_nCurSel;
public:
	BOOL	InitTabCtrl(SIZE *siItem, UINT nIDNorBmp = -1, UINT nIDHovBmp = -1, UINT nIDBkBmp = -1);
	LONG	InsertItem(int nItem, LPCTSTR lpszItem, int nImage = -1);
	BOOL	SetItem(int nItem, TCITEM* pTabCtrlItem);		// ֻ����mask=TCIF_PARAM�����
	BOOL	GetItem(int nItem, TCITEM* pTabCtrlItem) const; // Ŀ����Ϊ�Ժ����
	int		SetCurSel(int nIndex);
	int		GetCurSel() const;
	int		GetItemCount( ) const;
	void	SetTextColor(COLORREF& color);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};



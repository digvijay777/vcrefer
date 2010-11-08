#pragma once
#include <vector>
// CSkyCheckListBox

class CSkyCheckListBox : public CListBox
{
	DECLARE_DYNAMIC(CSkyCheckListBox)

public:
	CSkyCheckListBox();
	virtual ~CSkyCheckListBox();

protected:
	DECLARE_MESSAGE_MAP()

	typedef struct tagItemData{
		CString			strText;
		BOOL			bChecked;
	}ITEMDATA;

	std::vector<ITEMDATA>		m_vctItem;
	int							m_nItemHeight;
	CImageList					m_imgState;
	COLORREF					m_colText;
protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	void			ModifyListStyle();
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	void			DrawItem_ShowList(LPDRAWITEMSTRUCT pItem);
public:
	void			AddItem(LPCSTR lpText);
	void			ClearItem();
	void			SetImageList(UINT nIDState = -1);
	void			CheckAll(BOOL bChecked = TRUE);
	BOOL			GetItemChecked(int nIndex);
	void			SetTextColor(COLORREF colText = RGB(0x0, 0x0, 0x0));
private:
	CRect		CalCheckBoxRect(CRect& itRect);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};



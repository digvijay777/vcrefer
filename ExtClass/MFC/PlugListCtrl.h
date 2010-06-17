#pragma once
#include <vector>

// CPlugListCtrl

class CPlugListCtrl : protected CListBox
{
	DECLARE_DYNAMIC(CPlugListCtrl)

public:
	CPlugListCtrl();
	virtual ~CPlugListCtrl();

protected:
	DECLARE_MESSAGE_MAP()

	typedef struct tagPlugData{
		CString			strTitle;
		CString			strPath;
		CString			strType;
		CString			strCompany;
		BOOL			bChecked;
	}PLUGDATA;

	typedef struct tagPromptData{
		CString			strText;
		CString			strText2;
		int				nImage;
		COLORREF		nColor;
	}PROMPTDATA;
	
	std::vector<PLUGDATA>		m_vctPlug;
	PROMPTDATA					m_pdPrompt;
	BOOL						m_bShowPlug;
	int							m_nItemHeight;
	CImageList					m_imgState;
	CImageList					m_imgPrompt;
protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	
	void			ModifyListStyle();
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	void			DrawItem_ShowPlugList(LPDRAWITEMSTRUCT pItem);
	void			DrawItem_ShowPrompt(LPDRAWITEMSTRUCT pItem);
public:
	void			AddPlug(LPCSTR lpName, LPCSTR lpPath, LPCSTR lpType, LPCSTR lpCompany);
	void			AddPrompt(LPCSTR lpText, LPCSTR lpText2 = NULL, int nImg = 0, COLORREF col = RGB(0x0, 0x0, 0xff));
	void			ClearPlug();
	void			SetImageList(UINT nIDState = -1, UINT nIDPrompt = -1);
	void			CheckAll(BOOL bChecked = TRUE);
	BOOL			GetPlugChecked(int nIndex);
private:
	CRect		CalCheckBoxRect(CRect& itRect);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};



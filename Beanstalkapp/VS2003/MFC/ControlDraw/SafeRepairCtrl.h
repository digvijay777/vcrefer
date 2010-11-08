#pragma once
#include <vector>

// CSafeRepairCtrl
#define UM_SAFEREPAIR_CLICKED			WM_USER+1024

class CSafeRepairCtrl : public CListBox
{
	DECLARE_DYNAMIC(CSafeRepairCtrl)

public:
	CSafeRepairCtrl();
	virtual ~CSafeRepairCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	void		AddSafeItem(BOOL bSafe, LPCTSTR lpTitle, LPCTSTR lpDescrib, LPCTSTR lpBtnText, int nBtnID);
	void		ClearSafeItem();
	void		SetImageList(UINT nSafeImg = -1, UINT nBtn = -1);
private:
	typedef struct tagSafeData{
		BOOL		bSafe;
		CString		strTitle;
		CString		strDescrib;
		CString		strBtnText;
		int			nBtnID;
		int			nBtnState;
	}SAFEDATA;

	typedef struct tagMouseState{
		int			nIndex;
		int			nOldIndex;
		BOOL		bDown;
	}MOUSESTATE;

	std::vector<SAFEDATA>		m_vctSafeItem;
	int							m_nItemHeight;
	CImageList					m_imgSafeState;
	CImageList					m_imgBtn;
	CSize						m_sizeBtn;
	CSize						m_sizeState;
	MOUSESTATE					m_msMouse;

	virtual void PreSubclassWindow();
private:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void		DrawItem_ShowSafeItem(LPDRAWITEMSTRUCT pItem);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	CRect		CalButtonRect(CRect rect);
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};



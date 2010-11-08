#pragma once
#include <vector>

// CSkyScoreList

class CSkyScoreListEx : public CListBox
{
	DECLARE_DYNAMIC(CSkyScoreListEx)

public:
	CSkyScoreListEx();
	virtual ~CSkyScoreListEx();

private:
	typedef struct tagScoreData
	{
		CString			strText;
		BOOL			bGroup;
		int				nState;
		CString			strLink;
		CRect			rtLink;
	}SCOREDATA;

	std::vector<SCOREDATA>		m_vctData;
	CFont						m_fontBlod;
	COLORREF					m_colItemText;
	COLORREF					m_colGroup;
	CImageList					m_imgList;
	CImageList					m_imgCheckingState;
	IMAGEINFO					m_infoList;
	IMAGEINFO					m_infoCheckingState;
	int							m_nCheckingState;
	UINT						m_uLinkMsg;
	HWND						m_hwndLinkMsg;
	HCURSOR						m_handCursor;
protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();

public:
	int	AddItem(LPCSTR lpText, BOOL Group = FALSE, int nState = 0, LPCSTR lpLink = NULL);
	void SetItemState(int nIndex, int nState);
	void SetItemText(int nIndex, LPCSTR lpText, LPCSTR lpLink = NULL);
	void SetTextColor(COLORREF colText = RGB(0x0, 0x0, 0x0), COLORREF colGroup = RGB(0x0, 0x0, 0x0));
	void SetLinkMessage(UINT uLinkMsg, HWND hWnd = NULL);
	void SetImageList(UINT uImg, UINT nCheckingState = -1);
	void ClearItem();
	BOOL PtInLink(CPoint pt);
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	void		DrawItem_Group(CDC *pDC, LPSTR lpText, CRect rect);
	void		DrawItem_Item(CDC* pDC, LPSTR lpText, CRect rect, int nState, LPSTR lpLink = NULL, CRect* pRtLink = NULL);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};



#pragma once
#include <vector>

// CSkyScoreList

class CSkyScoreList : public CListBox
{
	DECLARE_DYNAMIC(CSkyScoreList)

public:
	CSkyScoreList();
	virtual ~CSkyScoreList();

private:
	typedef struct tagScoreData
	{
		CString			strText;
		BOOL			bGroup;
		int				nState;
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
protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();

public:
	int	AddItem(LPCSTR lpText, BOOL Group = FALSE, int nState = 0);
	void SetItemState(int nIndex, int nState);
	void SetItemText(int nIndex, LPCSTR lpText);
	void SetTextColor(COLORREF colText = RGB(0x0, 0x0, 0x0), COLORREF colGroup = RGB(0x0, 0x0, 0x0));
	void SetImageList(UINT uImg, UINT nCheckingState = -1);
	void ClearItem();
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	void		DrawItem_Group(CDC *pDC, LPSTR lpText, CRect rect);
	void		DrawItem_Item(CDC* pDC, LPSTR lpText, CRect rect, int nState);
	afx_msg void OnTimer(UINT nIDEvent);
};



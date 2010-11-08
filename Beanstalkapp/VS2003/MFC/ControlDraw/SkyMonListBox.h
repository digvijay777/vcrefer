#pragma once
#include <vector>

// CCtrlListBox
#define MD_TITLE		0x1
#define MD_DETAIL		0x2
#define MD_ON			0x4
#define MD_OFF			0x8
#define MD_TURNON		0x10
#define MD_TURNOFF		0x20
#define MD_IMAGE		0x40
#define MD_RUNNING		0x80
#define MD_ALL			(~0)

class CSkyMonListBox : public CListBox
{
	DECLARE_DYNAMIC(CSkyMonListBox)

public:
	CSkyMonListBox();
	virtual ~CSkyMonListBox();

public:
	typedef struct tagMonListData
	{
		CString		strTitle;		// 标题
		CString		strDetail;		// 细节
		CString		strOn;			// (已关闭)
		CString		strOff;			// (已开启)
		CString		strBtnTurnOn;	// 开启
		CString		strBtnTurnOff;	// 关闭
		int			nImage;			// 图标索引
		BOOL		bRunning;		// 是否开启
	}MONLISTDATA, *PMONLISTDATA;

private:
	CImageList		m_imgMinState;
	CImageList		m_imgMaxState;
	CImageList		m_imgSelBk;
	int				m_nCurSel;
	HCURSOR			m_hHand;
	UINT			m_uMsgClick;

	std::vector<MONLISTDATA>		m_vctData;
	std::vector<CRect>				m_vctRect;

	int		GetMonItemHeight(int nItem, BOOL bSelect);
	int		GetMouseOnLink(CPoint& point);
public:
	int		AddMonItem(PMONLISTDATA pData, int nMask = MD_ALL);
	void	DeleteAllItem();
	BOOL	SetMonItem(int nItem, PMONLISTDATA pData, int nMask);
	BOOL	SetBitmaps(CBitmap* pMaxState, CBitmap* pMinState, CBitmap* pSelBk);
	BOOL	SetBitmaps(UINT uMaxState, UINT uMinState, UINT uSelBk);
	int		HitTest(CPoint point);
	int		SetCallBackMsg(UINT uMsg);
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual int		DrawItem_Select(CDC* pDC, CRect& rect, MONLISTDATA& monData, CRect* pRetBtn = NULL);
	virtual int		DrawItem_Normal(CDC* pDC, CRect& rect, MONLISTDATA& monData, CRect* pRetBtn = NULL);

	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
protected:
	virtual void PreSubclassWindow();
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};



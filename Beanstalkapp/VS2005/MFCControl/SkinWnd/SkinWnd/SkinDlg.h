#pragma once
#include <vector>

class CSkinNcControl;
// CSkinDlg dialog
class CSkinDlg : public CDialog
{
	DECLARE_DYNAMIC(CSkinDlg)

public:
	CSkinDlg(UINT nIDTemplate, CWnd* pParent = NULL);   // standard constructor
	virtual ~CSkinDlg();

	DECLARE_MESSAGE_MAP()

public:
	CBitmap							m_bmpBk;
	CBitmap							m_snapshotBmpbk;
	std::vector<CSkinNcControl*>	m_SkinControl;
	BOOL							m_bTrackMouseEvent;
	COLORREF						m_colorTitle;
	BOOL							m_bNcCapture;
	CRect							m_wndrect;
	int								m_nCaptionHeight;
	int								m_nFrameWidth;

public:
	virtual void	Draw_TitleBar(CDC* pDC, CRect& rect);
	virtual void	Draw_ControlBox(CDC* pDC, CRect& rect);

public:
	afx_msg void OnNcPaint();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnNcMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg void OnNcRButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnNcRButtonDown(UINT nHitTest, CPoint point);
	afx_msg LRESULT On0x313(WPARAM wParam, LPARAM lParam);
	
	virtual BOOL OnInitDialog();

public:
	void		ScreenToWindow(CPoint* pPoint);
	BOOL		LoadImages(INT nIDBk);
	BOOL		LoadBitmaps(HBITMAP hBk);
	BOOL		DoCalcWindowRect();
	BOOL		DoCreateSnapshotBmpbk();
	BOOL		ContorlButton(INT nID, HBITMAP hBitmap, int nRight, int nTop, BOOL bAdd);
	BOOL		AddContorlButton(INT nID, INT nBtnBmpID, int nRight, int nTop);
	BOOL		DelContorlButton(int nID);
	void		SetTextColor(COLORREF col);
	void		SetNonClient(int nCaptionHeight = -1, int nFrameWidth = -1);
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

// 位图操作类
class CBitmapDC : public CDC
{
public:
	CBitmapDC(HBITMAP hBitmap);
	~CBitmapDC();

private:
	HBITMAP			m_hBmp;
	int				m_nSaveID;
public:
	BOOL	CreateFromBitmap(HBITMAP hBitmap);
};

// 控制按钮操作类
class CSkinNcControl
{
public:
	CSkinNcControl();
	~CSkinNcControl();

public:
	DWORD				m_ID;
	HWND				m_hWnd;
	CPoint				m_relPos;
	CSize				m_relSize;
	CRect				m_rawRect;
	LPARAM				m_lpData;
	BOOL				m_bMouseOver;
	BOOL				m_bCapture;
	BOOL				m_bDisabled;

public:
	BOOL		Create(HWND hWnd, DWORD nID, int rl, int rt, int nWidth, int nHeight);
	BOOL		CalcSize(CRect rtParent);
	int			GetWidth();
	int			GetHeight();		

public:
	virtual BOOL		OnLButtonDown(CPoint point);
	virtual BOOL		OnLButtonUp(CPoint point);
	virtual BOOL		OnMouseMove(CPoint point);
public:
	virtual void		Draw(CDC* pDC);
};

// 按钮类
class CSkinNcButton : public CSkinNcControl
{
public:
	CSkinNcButton();
	~CSkinNcButton();

public:
	CImageList			m_imagelist;

public:
	virtual void		Draw(CDC* pDC);

public:
	BOOL		SetBitmap(HBITMAP hBitmap);
};
#pragma once
#include <gdiplus.h>

// CPlayGifStatic

class CPlayGifStatic : public CStatic
{
	DECLARE_DYNAMIC(CPlayGifStatic)

public:
	CPlayGifStatic();
	virtual ~CPlayGifStatic();

protected:
	DECLARE_MESSAGE_MAP()

public:
	BOOL						Load(LPCTSTR lpStr);
	BOOL						Play();
	BOOL						Stop();

protected:
	BOOL						Load(Gdiplus::Image* pImage);
	BOOL						Load(HINSTANCE hInst, LPCSTR lpName, LPCSTR lpType);

private:
	Gdiplus::Image*				m_pImage;
	GUID*						m_pDimensionIDs;
	UINT						m_FrameCount;
	Gdiplus::PropertyItem*		m_pItem;
	int							m_iCurrentFrame;

	void						Clear();
public:
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};



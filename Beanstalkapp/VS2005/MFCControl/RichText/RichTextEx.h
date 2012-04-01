#pragma once


// CRichTextEx

class CRichTextEx : public CRichEditCtrl
{
	DECLARE_DYNAMIC(CRichTextEx)

public:
	CRichTextEx();
	virtual ~CRichTextEx();

protected:
	DECLARE_MESSAGE_MAP()

public:
	static BOOL		s_init;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};



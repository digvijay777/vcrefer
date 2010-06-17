#pragma once


// CGdipDailog dialog

class CGdipDialog : public CDialog
{
	DECLARE_DYNAMIC(CGdipDialog)

public:
	CGdipDialog(UINT nIDTemplate, CWnd* pParent = NULL);   // standard constructor
	virtual ~CGdipDialog();

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
private:
	BOOL			m_bBkgndChanged;
	CBrush			m_brushBkgnd;
public:

	DECLARE_MESSAGE_MAP()
public:
	BOOL	CreateBkgndBrush(CDC* pDC, CBrush* pBrush);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

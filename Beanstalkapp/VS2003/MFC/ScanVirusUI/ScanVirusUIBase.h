#pragma once


// CScanVirusUIBase dialog

class CScanVirusUIBase : public CDialog
{
	DECLARE_DYNAMIC(CScanVirusUIBase)

protected:
	CScanVirusUIBase(UINT nIDTemplate, CWnd* pParent = NULL);   // standard constructor
	virtual ~CScanVirusUIBase();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
protected:
	UINT		m_nIDTitleBarBkimg;		// ±ÍÃ‚¿∏±≥æ∞
private:
	bool DrawTitleBar();
protected:
	virtual void InitImageID() = 0;
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

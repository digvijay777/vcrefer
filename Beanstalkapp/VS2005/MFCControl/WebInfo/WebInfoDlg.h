// WebInfoDlg.h : header file
//

#pragma once


// CWebInfoDlg dialog
class CWebInfoDlg : public CDHtmlDialog
{
// Construction
public:
	CWebInfoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_WEBINFO_DIALOG, IDH = IDR_HTML_WEBINFO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	HRESULT OnBodyLButtonDown(IHTMLElement *pElement);
	HRESULT OnBodyLButtonUp(IHTMLElement *pElement);
	HRESULT OnBodyLButtonMove(IHTMLElement *pElement);
	HRESULT OnBodyContextMenu(IHTMLElement *pElement);

	HRESULT OnButtonMouseLeave(IHTMLElement *pElement);
	HRESULT OnButtonMouseHover(IHTMLElement *pElement);
	HRESULT OnButtonMouseDown(IHTMLElement *pElement);
	HRESULT OnButtonMouseUp(IHTMLElement *pElement);
	HRESULT OnNull(IHTMLElement *pElement);

	BOOL CanAccessExternal(){ return TRUE; }; //允许访问外部对象
	BOOL IsExternalDispatchSafe(){ return true; }//安全访问
// Implementation
protected:
	HICON							m_hIcon;
	CComPtr<IHTMLElement>			m_spCapture;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
};

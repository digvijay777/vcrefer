// DHTMLTestDlg.h : 头文件
//

#pragma once


// CDHTMLTestDlg 对话框
class CDHTMLTestDlg : public CDHtmlDialog
{
// 构造
public:
	CDHTMLTestDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DHTMLTEST_DIALOG, IDH = IDR_HTML_DHTMLTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
	DECLARE_DISPATCH_MAP() 

public:
	void MyNavigate(LPCTSTR lpURL);
	virtual void OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl);
};

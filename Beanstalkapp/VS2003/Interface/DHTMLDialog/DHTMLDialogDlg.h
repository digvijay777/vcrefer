// DHTMLDialogDlg.h : 头文件
//

#pragma once


// CDHTMLDialogDlg 对话框
class CDHTMLDialogDlg : public CDHtmlDialog
{
// 构造
public:
	CDHTMLDialogDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DHTMLDIALOG_DIALOG, IDH = IDR_HTML_DHTMLDIALOG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);
	HRESULT OnButtonHideImg(IHTMLElement *pElement);
	HRESULT OnButtonSelect(IHTMLElement *pElement);
	HRESULT OnButtonCheck(IHTMLElement *pElement);
	HRESULT OnButtonAddSelect(IHTMLElement *pElement);
	HRESULT OnButtonDeleteSelect(IHTMLElement *pElement);
	HRESULT OnButtonClickImg(IHTMLElement *pElement);
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
public:
//	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

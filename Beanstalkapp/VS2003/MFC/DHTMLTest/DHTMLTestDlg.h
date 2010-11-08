// DHTMLTestDlg.h : ͷ�ļ�
//

#pragma once


// CDHTMLTestDlg �Ի���
class CDHTMLTestDlg : public CDHtmlDialog
{
// ����
public:
	CDHTMLTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DHTMLTEST_DIALOG, IDH = IDR_HTML_DHTMLTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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

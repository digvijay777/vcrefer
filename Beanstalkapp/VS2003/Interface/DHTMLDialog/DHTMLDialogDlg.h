// DHTMLDialogDlg.h : ͷ�ļ�
//

#pragma once


// CDHTMLDialogDlg �Ի���
class CDHTMLDialogDlg : public CDHtmlDialog
{
// ����
public:
	CDHTMLDialogDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DHTMLDIALOG_DIALOG, IDH = IDR_HTML_DHTMLDIALOG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);
	HRESULT OnButtonHideImg(IHTMLElement *pElement);
	HRESULT OnButtonSelect(IHTMLElement *pElement);
	HRESULT OnButtonCheck(IHTMLElement *pElement);
	HRESULT OnButtonAddSelect(IHTMLElement *pElement);
	HRESULT OnButtonDeleteSelect(IHTMLElement *pElement);
	HRESULT OnButtonClickImg(IHTMLElement *pElement);
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
public:
//	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

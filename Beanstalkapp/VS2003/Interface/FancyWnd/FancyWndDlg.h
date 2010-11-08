// FancyWndDlg.h : ͷ�ļ�
//

#pragma once
#include "TFancyWnd.h"

typedef TFancyWnd<CDHtmlDialog>	CMyDialog;
// CFancyWndDlg �Ի���
class CFancyWndDlg : public CMyDialog
{
// ����
public:
	CFancyWndDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_FANCYWND_DIALOG, IDH = IDR_HTML_FANCYWND_DIALOG };

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
};

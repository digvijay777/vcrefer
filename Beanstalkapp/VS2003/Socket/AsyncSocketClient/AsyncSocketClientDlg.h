// AsyncSocketClientDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CAsyncSocketClientDlg �Ի���
class CAsyncSocketClientDlg : public CDialog
{
// ����
public:
	CAsyncSocketClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_ASYNCSOCKETCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CListBox m_msg;
	afx_msg void OnBnClickedBtConnect();
	afx_msg void OnBnClickedBtClose();
	afx_msg void OnBnClickedBtSend();
	afx_msg void OnClose();
};

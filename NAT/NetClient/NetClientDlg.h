// NetClientDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CNetClientDlg �Ի���
class CNetClientDlg : public CDialog
{
// ����
public:
	CNetClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_NETCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	CString			m_strINI;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();

	SOCKET		m_sockChart;

	void		ListenSocket(SOCKET sock);
	void		EnableChartWin(BOOL bConnect = FALSE);
public:
	static DWORD CALLBACK	ListenSocketThread(LPVOID lpParam);
	static DWORD CALLBACK	RecvFromSocketThread(LPVOID lpParam);
	CListBox m_list;
	afx_msg void OnBnClickedBtConnect();
	afx_msg void OnBnClickedBtSend();
	afx_msg void OnBnClickedBtSendnat();
};

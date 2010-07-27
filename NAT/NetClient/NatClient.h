#pragma once
#include "afxwin.h"


// CNatClient dialog

class CNatClient : public CDialog
{
	DECLARE_DYNAMIC(CNatClient)

public:
	CNatClient(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNatClient();

// Dialog Data
	enum { IDD = IDD_NATCLIENT };

	CString			m_strINI;
	SOCKET			m_sock;
	SOCKET			m_svrSock;
	SOCKET			m_sockChart;
	int				m_nPortChart;
	HANDLE			m_hEvent;
	CString			m_strHoleIP;
	BOOL			m_bHole;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CListBox m_list;
	CEdit m_chart;
	CEdit m_send;
	CListBox m_usr;

	void		EnableChart(BOOL bChar = FALSE);
	void		EnableUser(BOOL bChar = FALSE);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedBtHole();

	void		StartListen();
	void		StartHole(int nPort);
	void		ConnectHole(int nPort);
	void		Chart();

	static CNatClient*		s_pThis;
	static DWORD CALLBACK		OnStartListen(LPVOID lpParam);
	static DWORD CALLBACK		OnStartHole(LPVOID lpParam);
	static DWORD CALLBACK		OnConnectHole(LPVOID lpParam);
	static DWORD CALLBACK		OnChart(LPVOID lpParam);
	afx_msg void OnBnClickedBtSend();
};

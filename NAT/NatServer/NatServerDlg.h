// NatServerDlg.h : ͷ�ļ�
//

#pragma once
#include <vector>
#include "afxwin.h"

// CNatServerDlg �Ի���
class CNatServerDlg : public CDialog
{
// ����
public:
	CNatServerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_NATSERVER_DIALOG };

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
	afx_msg void OnBnClickedOk();

	void			OnListen();
	void			OnAccept(SOCKET sock);

	std::vector<SOCKET>			m_vctClient;
	static CNatServerDlg*		m_pThis;
	static DWORD CALLBACK		OnThreadListen(LPVOID lpParam);
	static DWORD CALLBACK		OnThreadAccept(LPVOID lpParam);
	CListBox m_list;
};

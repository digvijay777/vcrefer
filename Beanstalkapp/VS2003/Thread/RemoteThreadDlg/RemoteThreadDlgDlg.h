// RemoteThreadDlgDlg.h : ͷ�ļ�
//

#pragma once


// CRemoteThreadDlgDlg �Ի���
class CRemoteThreadDlgDlg : public CDialog
{
// ����
public:
	CRemoteThreadDlgDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_REMOTETHREADDLG_DIALOG };

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
	afx_msg void OnBnClickedBtRemotepthread();
	static void WINAPI ThreadRemoteFn(LPVOID lpParameter);
	static void WINAPI ThreadRemoteFn_End();
};

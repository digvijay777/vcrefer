// ThreadTestDlg.h : ͷ�ļ�
//

#pragma once


// CThreadTestDlg �Ի���
class CThreadTestDlg : public CDialog
{
// ����
public:
	CThreadTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_THREADTEST_DIALOG };

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
private:
	CWinThread*			m_pThread;
public:
	afx_msg void OnBnClickedBtCreatethread();
	afx_msg void OnBnClickedBtSendthreadmessage();
	afx_msg void OnBnClickedStEndthread();
};

// SendMailDlg.h : ͷ�ļ�
//

#pragma once


// CSendMailDlg �Ի���
class CSendMailDlg : public CDialog
{
// ����
public:
	CSendMailDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SENDMAIL_DIALOG };

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
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();

public:
	static DWORD WINAPI SendMailThread(LPVOID lpParamter);
protected:
	virtual void OnCancel();
public:
	afx_msg void OnBnClickedButton3();
};

// ProcessAsUserDlg.h : ͷ�ļ�
//

#pragma once


// CProcessAsUserDlg �Ի���
class CProcessAsUserDlg : public CDialog
{
// ����
public:
	CProcessAsUserDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PROCESSASUSER_DIALOG };

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
	HANDLE OpenSystemProcess() ;
public:
	afx_msg void OnBnClickedButton1();
	BOOL	EnableProcPrivilege(DWORD PID);
	BOOL	EnablePrivilege(TCHAR * name, BOOL fEnable = TRUE, HANDLE hToken = NULL);
};

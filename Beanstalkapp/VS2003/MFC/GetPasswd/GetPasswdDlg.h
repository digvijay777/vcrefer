// GetPasswdDlg.h : ͷ�ļ�
//

#pragma once
#include "../../../extclass/mfc/SelectWndBtn.h"


// CGetPasswdDlg �Ի���
class CGetPasswdDlg : public CDialog
{
// ����
public:
	CGetPasswdDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GETPASSWD_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	
	BOOL GetPasswordWndTitle(HWND hWnd, CString& strTitle);
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg LRESULT OnSelectedWnd(WPARAM wParam, LPARAM lParam);
	CSelectWndBtn m_selectwnd;
	afx_msg BOOL OnQueryOpen();
};

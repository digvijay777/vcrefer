// CreateWindowDlg.h : ͷ�ļ�
//

#pragma once


// CCreateWindowDlg �Ի���
class CCreateWindowDlg : public CDialog
{
// ����
public:
	CCreateWindowDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CREATEWINDOW_DIALOG };

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
	afx_msg void OnBnClickedButton2();
public:
	static LRESULT CALLBACK ServerWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	afx_msg void OnBnClickedButton3();
};

// MultiDesktopDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CMultiDesktopDlg �Ի���
class CMultiDesktopDlg : public CDialog
{
// ����
public:
	CMultiDesktopDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MULTIDESKTOP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	void RefurbishDesktop();
	static BOOL CALLBACK EnumDesktopProc(LPTSTR lpszDesktop, LPARAM lParam);
	static BOOL CALLBACK EnumWindowStationProc(LPTSTR lpszWindowStation, LPARAM lParam);
	static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);

public:
	afx_msg void OnBnClickedOk();
	CListBox m_list;
	afx_msg void OnBnClickedBtNewdesktop();
	afx_msg void OnBnClickedBtSwitchdesktop();
	afx_msg void OnBnClickedButton1();
};

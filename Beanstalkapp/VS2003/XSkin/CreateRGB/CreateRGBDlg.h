// CreateRGBDlg.h : ͷ�ļ�
//

#pragma once


// CCreateRGBDlg �Ի���
class CCreateRGBDlg : public CDialog
{
// ����
public:
	CCreateRGBDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CREATERGB_DIALOG };

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
	int m_nModule;
	CString m_path;
	afx_msg void OnBnClickedBtScan();
	afx_msg void OnBnClickedOk();
	int m_nRows;
	BOOL m_nAllScan;
	BOOL m_bEnter;
	int m_nShowModule;
};

// CreateChildProcessDlg.h : ͷ�ļ�
//

#pragma once


// CCreateChildProcessDlg �Ի���
class CCreateChildProcessDlg : public CDialog
{
// ����
public:
	CCreateChildProcessDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CREATECHILDPROCESS_DIALOG };

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
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtShowprocesslist();
	afx_msg void OnBnClickedBtSelect();
	afx_msg void OnBnClickedButton3();
};

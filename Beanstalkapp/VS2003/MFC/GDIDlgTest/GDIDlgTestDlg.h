// GDIDlgTestDlg.h : ͷ�ļ�
//

#pragma once


// CGDIDlgTestDlg �Ի���
class CGDIDlgTestDlg : public CDialog
{
// ����
public:
	CGDIDlgTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GDIDLGTEST_DIALOG };

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
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedOk();

public:
	afx_msg void OnMove(int x, int y);
	HDC m_hdcMemory;
	BLENDFUNCTION m_Blend;
};

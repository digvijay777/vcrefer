// DeskAssistDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"


// CDeskAssistDlg �Ի���
class CDeskAssistDlg : public CDialog
{
// ����
public:
	CDeskAssistDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DESKASSIST_DIALOG };

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
public:
	afx_msg void OnBnClickedOk();
	CTabCtrl				m_ctlTab;
	afx_msg void OnBnClickedButton1();
};

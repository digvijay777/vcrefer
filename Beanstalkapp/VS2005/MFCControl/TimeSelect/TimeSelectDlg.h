// TimeSelectDlg.h : ͷ�ļ�
//

#pragma once
#include "TimeSelectCtrl.h"
#include "afxwin.h"

// CTimeSelectDlg �Ի���
class CTimeSelectDlg : public CDialog
{
// ����
public:
	CTimeSelectDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TIMESELECT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	CTimeSelectCtrl		m_timesel;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	CStatic m_ctlRichTest;
};

// SkinWndDlg.h : ͷ�ļ�
//

#pragma once
#include "SkinWnd/SkinDlg.h"

// CSkinWndDlg �Ի���
class CSkinWndDlg : public CSkinDlg
{
// ����
public:
	CSkinWndDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SKINWND_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON			m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};

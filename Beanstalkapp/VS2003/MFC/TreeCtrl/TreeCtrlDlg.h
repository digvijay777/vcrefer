// TreeCtrlDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"

#include "../../../ExtClass/mfc/ExtTreeCtrl.h"
// CTreeCtrlDlg �Ի���
class CTreeCtrlDlg : public CDialog
{
// ����
public:
	CTreeCtrlDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TREECTRL_DIALOG };

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
	CExtTreeCtrl m_tree;
	afx_msg void OnTvnItemexpandingTree1(NMHDR *pNMHDR, LRESULT *pResult);
};

// IconListDlg.h : ͷ�ļ�
//

#pragma once
#include "IconList/IconListCtrl.h"
#include "afxwin.h"

// CIconListDlg �Ի���
class CIconListDlg : public CDialog
{
// ����
public:
	CIconListDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_ICONLIST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON			m_hIcon;
	CIconListCtrl	m_ilc;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnUmNotify(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	CButton m_ck;
public:
	afx_msg void OnBnClickedCheck1();
};

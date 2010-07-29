// UpnpMangerDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "upnp.h"
#include "upnp2.h"

// CUpnpMangerDlg �Ի���
class CUpnpMangerDlg : public CDialog
{
// ����
public:
	CUpnpMangerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_UPNPMANGER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	VOID	EnableOptButton(BOOL bEnabled = TRUE);
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

	MyUPnP			m_upnp;
	CListCtrl		m_list;
	CEdit			m_ip;
	afx_msg void OnBnClickedBtRefurish();
	afx_msg void OnBnClickedBtAdd();
	afx_msg void OnBnClickedBtDelete();
};

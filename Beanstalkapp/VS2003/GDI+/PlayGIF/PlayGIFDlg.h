// PlayGIFDlg.h : ͷ�ļ�
//

#pragma once
#include "playgifstatic.h"


// CPlayGIFDlg �Ի���
class CPlayGIFDlg : public CDialog
{
// ����
public:
	CPlayGIFDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PLAYGIF_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	int		m_nBk;
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CPlayGifStatic m_gif;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
};

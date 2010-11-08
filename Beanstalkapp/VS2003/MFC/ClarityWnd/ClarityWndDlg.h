// ClarityWndDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "../../../ExtClass/mfc/SelectWndBtn.h"
#include "afxcmn.h"

// CClarityWndDlg �Ի���
class CClarityWndDlg : public CDialog
{
// ����
public:
	CClarityWndDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CLARITYWND_DIALOG };

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
	afx_msg LRESULT OnEndSelectWnd(WPARAM wParam, LPARAM lParam);

	CSelectWndBtn m_btnSertch;
	CSliderCtrl m_Alpha;
	HWND		m_hAlphaWnd;
	afx_msg void OnNMThemeChangedSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};

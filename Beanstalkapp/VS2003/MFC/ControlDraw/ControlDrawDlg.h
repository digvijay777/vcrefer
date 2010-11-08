// ControlDrawDlg.h : ͷ�ļ�
//

#pragma once
#include "minitabctrl.h"
#include "transparentbtn.h"
#include "afxwin.h"
#include "../../../ExtClass/mfc/RadioButton.h"
#include "../../../ExtClass/mfc/pluglistctrl.h"


// CControlDrawDlg �Ի���
class CControlDrawDlg : public CDialog
{
// ����
public:
	CControlDrawDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CONTROLDRAW_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	CBitmap		m_bmpBk;
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	CMiniTabCtrl m_tab;
	afx_msg void OnBnClickedOk();
	afx_msg void OnTabCtrlSelectChange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedButton1();
	CTransparentBtn m_tranbtn;
// 	CButton			m_groupbox;

	CRadioButton			m_radio1;
	afx_msg void OnBnClickedRadio1();
	CPlugListCtrl m_ctlPlugList;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
};

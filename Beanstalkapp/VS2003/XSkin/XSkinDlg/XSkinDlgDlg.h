// XSkinDlgDlg.h : ͷ�ļ�
//

#pragma once
// #include "../../../ExtClass/XSkin/XSkinDialog.h"
#include "../../../ExtClass/XSkin/XSkinWnd.h"

// CXSkinDlgDlg �Ի���
class CXSkinDlgDlg : public CDialog, protected CXSkinWnd
{
// ����
public:
	CXSkinDlgDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_XSKINDLIALOG };

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
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};

// TransWndDlg.h : ͷ�ļ�
//

#pragma once
#include "ShowDlg.h"

// CTransWndDlg �Ի���
class CTransWndDlg : public CDialog
{
// ����
public:
	CTransWndDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TRANSWND_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


private:
	Gdiplus::Image*		m_pImage;

public:
	static BOOL		UpdateLayeredImageWindow(Gdiplus::Image* pImage, CDC* pDC, CWnd* pWnd);
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg LRESULT OnNcHitTest(CPoint point);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

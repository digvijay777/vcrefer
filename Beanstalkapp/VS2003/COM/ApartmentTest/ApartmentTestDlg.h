// ApartmentTestDlg.h : ͷ�ļ�
//

#pragma once


// CApartmentTestDlg �Ի���
class CApartmentTestDlg : public CDialog
{
// ����
public:
	CApartmentTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_APARTMENTTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	static DWORD WINAPI STAThreadProc(LPVOID lpParameter);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};

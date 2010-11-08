// XMLTestDlg.h : ͷ�ļ�
//

#pragma once


// CXMLTestDlg �Ի���
class CXMLTestDlg : public CDialog
{
// ����
public:
	CXMLTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_XMLTEST_DIALOG };

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
	CString GetNodeAttribute(LPVOID node);
	void ShowTree(CTreeCtrl* tree, HTREEITEM hTree, LPVOID node);
};

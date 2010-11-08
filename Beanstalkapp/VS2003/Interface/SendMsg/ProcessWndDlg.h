#pragma once
#include "afxcmn.h"

#include <map>

// CProcessWndDlg dialog

class CProcessWndDlg : public CDialog
{
	DECLARE_DYNAMIC(CProcessWndDlg)

public:
	CProcessWndDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProcessWndDlg();

// Dialog Data
	enum { IDD = IDD_PROCESSWND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	void		InitializeTree();
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtRefrush();
	CTreeCtrl m_tree;
	virtual BOOL OnInitDialog();
	static BOOL CALLBACK EnumThreadWndProc(HWND hwnd, LPARAM lParam);
	static CProcessWndDlg*		s_pThis;
	afx_msg void OnBnClickedBtSelectwnd();
public:
	HWND		m_hSelWnd;
	CImageList	m_imgList;
	std::map<DWORD, HTREEITEM>		m_Process;
};

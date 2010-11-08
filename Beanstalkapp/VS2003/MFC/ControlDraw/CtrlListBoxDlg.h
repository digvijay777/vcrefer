#pragma once
#include "SkyMonListBox.h"

#define UM_SELECTITEM		WM_USER+5
// CCtrlListBoxDlg dialog

class CCtrlListBoxDlg : public CDialog
{
	DECLARE_DYNAMIC(CCtrlListBoxDlg)

public:
	CCtrlListBoxDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCtrlListBoxDlg();

// Dialog Data
	enum { IDD = IDD_CTRLLISTBOXDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CSkyMonListBox m_list;
	virtual BOOL OnInitDialog();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

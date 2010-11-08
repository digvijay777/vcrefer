#pragma once


// CTabToolBar

class CTabToolBar : public CDialogBar
{
	DECLARE_DYNAMIC(CTabToolBar)

public:
	CTabToolBar();
	virtual ~CTabToolBar();

protected:
	DECLARE_MESSAGE_MAP()
	enum{IDC_TABCTRL = 1};
private:
	CTabCtrl		m_wndTabCtrl;
public:
	BOOL		InsertItem(int nIndex, LPSTR lpTitle, CCreateContext* pContext);
	BOOL		InsertItem(int nIndex, LPSTR lpTitle, CDocument* pDoc, CRuntimeClass* pView);
	BOOL		InsertItem(int nIndex, LPSTR lpTitle, CView* pView);
	BOOL		DeleteItem(int nIndex);
	int			SetCurSel(int nItem);
	CTabCtrl&	GetTabCtrl();
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTabCtrlSelectChange(NMHDR *pNMHDR, LRESULT *pResult);
};



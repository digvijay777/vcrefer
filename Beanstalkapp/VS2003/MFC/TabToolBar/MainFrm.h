// MainFrm.h : CMainFrame ��Ľӿ�
//


#pragma once
#include "../../../common/mfc/TabToolBar.h"

class CMainFrame : public CFrameWnd
{
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CToolBar	m_wndSearchBar;
	CButton		m_btSearch;
	CImageList	m_iToolBar;
	CImageList	m_iToolBarG;
public:
	CTabToolBar	m_wndTabCtrl;
	CComboBox	m_cbSearch;
	CEdit		m_etSearch;
// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnUpdateSeparator(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	
public:
	afx_msg void OnSearch();
	afx_msg void OnUpdateSearch(CCmdUI *pCmdUI);
};



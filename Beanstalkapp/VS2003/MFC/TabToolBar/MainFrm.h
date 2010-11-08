// MainFrm.h : CMainFrame 类的接口
//


#pragma once
#include "../../../common/mfc/TabToolBar.h"

class CMainFrame : public CFrameWnd
{
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 属性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
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
// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnUpdateSeparator(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	
public:
	afx_msg void OnSearch();
	afx_msg void OnUpdateSearch(CCmdUI *pCmdUI);
};



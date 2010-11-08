// MainFrm.h : CMainFrame 类的接口
//


#pragma once


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
public:
	CTabToolBar m_wndTabBar;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT		OnUpdateTable(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	int	FindViewIntable(CRuntimeClass* pViewClass);
public:
	afx_msg void OnDetFile();
	afx_msg void OnUpdateDetFile(CCmdUI *pCmdUI);
	afx_msg void OnDetReg();
	afx_msg void OnUpdateDetReg(CCmdUI *pCmdUI);
};



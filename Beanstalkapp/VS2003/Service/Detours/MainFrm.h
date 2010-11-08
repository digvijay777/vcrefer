// MainFrm.h : CMainFrame ��Ľӿ�
//


#pragma once


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
public:
	CTabToolBar m_wndTabBar;

// ���ɵ���Ϣӳ�亯��
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



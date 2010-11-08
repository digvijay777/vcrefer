#pragma once


// CDetFileView ��ͼ
typedef HANDLE  (WINAPI *FUN_CREATEFILE)(LPCTSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD , DWORD, HANDLE);

class CDetFileView : public CListView
{
	DECLARE_DYNCREATE(CDetFileView)
protected:
	CDetFileView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CDetFileView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
private:
	bool		m_bIsDetours;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnAddItem(WPARAM wParam, LPARAM lParam);
	afx_msg void OnAdddetours();
	afx_msg void OnUpdateAdddetours(CCmdUI *pCmdUI);

public:
	static HANDLE WINAPI DetourCreateFile(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, 
		DWORD dwCreationDispostion , DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
	static FUN_CREATEFILE RealCreateFile;
	static CDetFileView*	s_this;
};



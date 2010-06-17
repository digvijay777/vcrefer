#pragma once
#include <vector>

class CXCaptionCtrl
{
public:
	CXCaptionCtrl();
	~CXCaptionCtrl();
protected:
	UINT		m_uID;			// 控件ID
	CString		m_strText;		// 控件文本
	CRect		m_rect;
	CWnd*		m_pMainWnd;		// 主窗体句柄

public:
	BOOL CreateCtrl(CString& text, CRect& rect, UINT ID);

public:
	virtual void OnDraw(CDC *pDC);

public:
	static std::vector<CXCaptionCtrl*>		s_vctXCaptionCtrl;
	static WNDPROC							s_OldWndProc;
	static LRESULT CALLBACK XCaptionCtrlCallWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
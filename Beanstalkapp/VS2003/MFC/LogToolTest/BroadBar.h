#pragma once


// CBaseBar

class CBroadBar : public CControlBar
{
	DECLARE_DYNAMIC(CBroadBar)

public:
	CBroadBar();
	virtual ~CBroadBar();
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
};



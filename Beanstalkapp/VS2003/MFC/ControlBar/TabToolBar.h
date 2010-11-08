#pragma once


// CTabToolBar

class CTabToolBar : public CControlBar
{
	DECLARE_DYNAMIC(CTabToolBar)

public:
	CTabToolBar();
	virtual ~CTabToolBar();
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

protected:
	DECLARE_MESSAGE_MAP()
};



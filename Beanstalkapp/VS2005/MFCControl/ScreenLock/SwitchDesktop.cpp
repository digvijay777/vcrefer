#include "StdAfx.h"
#include "SwitchDesktop.h"


CSwitchDesktop::CSwitchDesktop(LPCTSTR lpDesktop)
{
	m_dwHotKey = 0;
	m_Timer = NULL;
	SwitchTo(lpDesktop);
}

CSwitchDesktop::~CSwitchDesktop(void)
{
	SwitchToDefault();
}

BOOL CSwitchDesktop::SwitchTo(LPCTSTR lpDesktop)
{
	HDESK		hDesk		= NULL;
	BOOL		bRet		= FALSE;
	HDESK		hTdDesk		= GetThreadDesktop(GetCurrentThreadId());

	// 打开并创建桌面
	hDesk = ::OpenDesktop(lpDesktop, 0, FALSE, MAXIMUM_ALLOWED);
	if(NULL == hDesk)
	{
		hDesk = ::CreateDesktop(lpDesktop, NULL, NULL
			, 0
			, MAXIMUM_ALLOWED
			, NULL);
	}
	if(NULL == hDesk)
		return FALSE;
	// 设置线程
	if(::SetThreadDesktop(hDesk))
	{
		if(NULL != m_hCreateDesk && hDesk != m_hCreateDesk)
			CloseDesktop(m_hCreateDesk);
		// 交换桌面
		if(::SwitchDesktop(hDesk))
		{
			if(_tcsicmp(lpDesktop, _T("Default")) == 0)
				m_hCreateDesk = NULL;
			else
				m_hCreateDesk = hDesk;
			m_Timer = SetTimer(NULL, 1, 100, CSwitchDesktop::TimerProc);
			return TRUE;
		}
		::SetThreadDesktop(hTdDesk);
	}
	TRACE("");
	::CloseDesktop(hDesk);
	return FALSE;
}

// 定时器回调函数
VOID CALLBACK CSwitchDesktop::TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	EnumWindows(CSwitchDesktop::EnumWindowsProc, (LPARAM)GetCurrentProcessId());
}

// 枚举窗体
BOOL  CALLBACK CSwitchDesktop::EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	DWORD			dwProcID		= 0;
	DWORD			dwPreProcID		= 0;
	HWND			hParent			= NULL;
	HWND			hTop			= NULL;

	GetWindowThreadProcessId(hwnd, &dwProcID);
	if(dwProcID == (DWORD)lParam)
		return TRUE;
	// 不是当前线程的窗体就结束
	PostMessage(hwnd, WM_CLOSE, 0, 0);

	return TRUE;
}

BOOL CSwitchDesktop::SwitchToDefault()
{
	KillTimer(NULL, m_Timer);
	return SwitchTo(_T("Default"));
}

BOOL CSwitchDesktop::IsSwitched()
{
	return NULL != m_hCreateDesk;
}
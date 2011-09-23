#include "StdAfx.h"
#include "SwitchDesktop.h"


CSwitchDesktop::CSwitchDesktop(LPCTSTR lpDesktop)
{
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
	hDesk = ::OpenDesktop(lpDesktop, DF_ALLOWOTHERACCOUNTHOOK, FALSE, MAXIMUM_ALLOWED);
	if(NULL == hDesk)
	{
		hDesk = ::CreateDesktop(lpDesktop, NULL, NULL
			, DF_ALLOWOTHERACCOUNTHOOK
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
			return TRUE;
		}
		::SetThreadDesktop(hTdDesk);
	}
	TRACE("");
	::CloseDesktop(hDesk);
	return FALSE;
}

BOOL CSwitchDesktop::SwitchToDefault()
{
	return SwitchTo(_T("Default"));
}

BOOL CSwitchDesktop::IsSwitched()
{
	return NULL != m_hCreateDesk;
}
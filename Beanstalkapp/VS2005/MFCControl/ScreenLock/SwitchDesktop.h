#pragma once

class CSwitchDesktop
{
public:
	CSwitchDesktop(LPCTSTR lpDesktop);
	virtual ~CSwitchDesktop(void);

private:
	HDESK		m_hCreateDesk;
	DWORD		m_dwHotKey;
	UINT_PTR	m_Timer;

protected:
	BOOL		SwitchTo(LPCTSTR lpDesktop);
	BOOL		SwitchToDefault();

	static VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
	static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
public:
	BOOL		IsSwitched();
};

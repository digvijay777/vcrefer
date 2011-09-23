#pragma once

class CSwitchDesktop
{
public:
	CSwitchDesktop(LPCTSTR lpDesktop);
	virtual ~CSwitchDesktop(void);

private:
	HDESK		m_hCreateDesk;

protected:
	BOOL		SwitchTo(LPCTSTR lpDesktop);
	BOOL		SwitchToDefault();

public:
	BOOL		IsSwitched();
};

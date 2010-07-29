#pragma once

class CUpnp
{
public:
	CUpnp(void);
	virtual ~CUpnp(void);

private:
	CString			m_strRouter;
public:
	BOOL			Search(LPCSTR lpIPAndPort);

	CString			GetRouterIP();
};

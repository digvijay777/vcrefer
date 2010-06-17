#pragma once
#include <string>
#include <vector>

class CNetAdapterMgr
{
public:
	CNetAdapterMgr();
	~CNetAdapterMgr();

	typedef struct tagNetAdapter
	{
		std::string			strName;
		std::_Bvector		strDescription;
		std::string			strClass;

	}NETADAPTER, *PNETADAPTER;

private:
	std::wstring				m_wstrAdapterGUID;
	std::vector<NETADAPTER>		m_vctNetAdapter;
public:
	int		GetAdapterCount();
	BOOL	EnumNetAdapter();
	BOOL	GetAdapterInfo(PNETADAPTER pAdapter, LPCWSTR lpPDOName);
};
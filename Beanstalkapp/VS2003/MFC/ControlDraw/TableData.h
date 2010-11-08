#pragma once
#include <list>
#include <vector>
#include <string>

using namespace std;

class CTableData
{
public:
	CTableData();
	virtual ~CTableData();
private:
	typedef struct tagTableData
	{
		vector<string>				vRow;
		vector< vector<string> >	child;
	}TABLEDATA;

	vector<TABLEDATA>		m_vctData;
	int						m_nColumnCount;
public:
	void		SetColumnCount(int nColumn = 0);
	int			InsertItem(int nItem, LPCSTR lpText);
	int			InsertChildItem(int nItem, int nChildItem, LPCSTR lpText);
	BOOL		SetItem(int nItem, int nSubItem, LPCSTR lpText);
	BOOL		SetChildItem(int nItem, int nChildItem, int nChildSubItem, LPCSTR lpText);
	string		GetItem(int nItem, int nSubItem);
	string		GetChildItem(int nItem, int nChildItem, int nChildSubItem);
	void		DeleteItem(int nItem);
	void		DeleteAllItem();
};

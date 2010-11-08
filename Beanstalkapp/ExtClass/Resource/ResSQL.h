/*
 *	从资源中加载SQL语句，注意，SQL语句以GO(必须为大写)为分隔条件
 */
#pragma once

class CResSQL
{
public:
	CResSQL(HINSTANCE hModule, UINT nIDSQL, LPCTSTR lpType = _T("SQL"));
	virtual ~CResSQL();

private:
	
};
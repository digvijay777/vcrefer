/*
 *	����Դ�м���SQL��䣬ע�⣬SQL�����GO(����Ϊ��д)Ϊ�ָ�����
 */
#pragma once

class CResSQL
{
public:
	CResSQL(HINSTANCE hModule, UINT nIDSQL, LPCTSTR lpType = _T("SQL"));
	virtual ~CResSQL();

private:
	
};
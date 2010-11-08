#include "stdafx.h"
#include "ResSQL.h"
#include <assert.h>

CResSQL::CResSQL(HINSTANCE hModule, UINT nIDSQL, LPCTSTR lpType /* = _T */)
{
	HRSRC	hSrc	= NULL;
	
	hSrc = FindResource(hModule, MAKEINTRESOURCE(nIDSQL), lpType);
	if(NULL == hSrc)
	{
		assert(false);
		return;
	}
}

CResSQL::~CResSQL()
{

}


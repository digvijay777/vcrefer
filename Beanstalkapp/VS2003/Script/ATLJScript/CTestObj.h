#pragma once

#include <afxwin.h>

class CTestObj : public IDispatch
{
public:
	CTestObj()
	{

	}
	~CTestObj()
	{

	}

	DECLARE_DISPATCH_MAP()
};

BEGIN_DISPATCH_MAP(CTestObj, CCmdTarget)

END_DISPATCH_MAP()
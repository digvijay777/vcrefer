// StatusHSplitter.cpp : implementation file
//

#include "stdafx.h"
#include "StatusHSplitter.h"
#include ".\statushsplitter.h"


// CStatusHSplitter

IMPLEMENT_DYNAMIC(CStatusHSplitter, CWnd)
CStatusHSplitter::CStatusHSplitter()
{
	m_nStatusHeight = 50;
}

CStatusHSplitter::~CStatusHSplitter()
{
}


BEGIN_MESSAGE_MAP(CStatusHSplitter, CWnd)
END_MESSAGE_MAP()



// CStatusHSplitter message handlers


BOOL CStatusHSplitter::Create(CWnd* pParent, int nRows, ...)
{
	BOOL		bRes		= FALSE;

	

	return FALSE;
}

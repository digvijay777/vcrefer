// STAtest.cpp : Implementation of CSTAtest

#include "stdafx.h"
#include "STAtest.h"
#include ".\statest.h"


// CSTAtest
long CSTAtest::s_Count = 0;

STDMETHODIMP CSTAtest::GetID(LONG* pVal)
{
	// TODO: Add your implementation code here
	*pVal = m_Count++;
	return S_OK;
}

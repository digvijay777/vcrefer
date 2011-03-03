#pragma once

#ifndef _WINDOWS_
#include <Windows.h>	
#endif

//////////////////////////////////////////////////////////////////////////
// ���ֽ��ַ�������
template <unsigned int TCodePage = CP_ACP, unsigned int TIncrement = 1024>
class TMultiString
{
public:
	TMultiString()
	{
		m_pMulti = NULL;
		m_pWide = NULL;
		m_nSize = 0;
	}

	~TMultiString()
	{
		if(NULL != m_pMulti)
			free(m_pMulti);
		if(NULL != m_pWide)
			free(m_pWide);
	}
private:
	//������洢����
	unsigned __int64	m_nSize;
	char*				m_pMulti;
	wchar_t*			m_pWide;

public:
	// ���صȺ�
	TMultiString& operator= (const char* pStr)
	{

	}
};
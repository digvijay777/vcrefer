#pragma once

#ifndef ATLTRACE
#ifdef TRACE
#define ATLTRACE		TRACE
#else
#define ATLTRACE
#endif
#endif
#ifndef ATLASSERT
#ifdef ASSERT
#define ATLASSERT		ASSERT
#else
#define ATLASSERT
#endif
#endif

//////////////////////////////////////////////////////////////////////////
template <int TBuffSize = 2048>
class TSQLString
{
public:
	TSQLString()
	{
		memset(m_szSQL, 0, sizeof(m_szSQL));
	}
private:
	wchar_t		m_szSQL[TBuffSize];

public:
	void format(const wchar_t* lpFmt, ...)
	{
		va_list		vaArr;

		memset(m_szSQL, 0, sizeof(m_szSQL));
		va_start(vaArr, lpFmt);
		vformat(m_szSQL, TBuffSize, lpFmt, vaArr);
		va_end(vaArr);
	}
	void append(LPCWSTR lpFmt, ...)
	{
		va_list		vaArr;
		wchar_t*	pStart		= m_szSQL;

		va_start(vaArr, lpFmt);
		while(*pStart) pStart++;
		vformat(pStart, TBuffSize - (int)(pStart - m_szSQL), lpFmt, vaArr);
		va_end(vaArr);
	}

	void vformat(wchar_t* pBuff, int nSize, const wchar_t* lpFmt, va_list vaArr)
	{
		for(int iBuf = 0; iBuf < nSize && *lpFmt; lpFmt++)
		{
			if('%' == *lpFmt)
			{
				lpFmt++;
				if('s' == *lpFmt)
				{
					// 替换文本
					wchar_t* pData		= va_arg(vaArr, wchar_t*);

					for(; iBuf < nSize && *pData; pData++)
					{
						pBuff[iBuf++] = *pData;
						if(L'\'' == *pData && iBuf < nSize)
							pBuff[iBuf++] = L'\''; // 关键的操作 SQL语句要替换单引号
					}
				}
				else if('S' == *lpFmt)
				{
					// 替换多字节文本
					wchar_t			szData[TBuffSize/2]		= {0};
					char*			pIn						= va_arg(vaArr, char*);
					wchar_t*		pData					= szData;

					if(0 != MultiByteToWideChar(CP_ACP, 0, pIn, (int)strlen(pIn), szData
						, (int)sizeof(szData) / (int)sizeof(wchar_t)))
					{
						for(; iBuf < nSize && *pData; pData++)
						{
							pBuff[iBuf++] = *pData;
							if(L'\'' == *pData && iBuf < nSize)
								pBuff[iBuf++] = L'\''; // 关键的操作 SQL语句要替换单引号
						}
					}
					else
					{
						// 转换失败
						ATLASSERT(FALSE);
						for(; iBuf < nSize && *pIn; pIn++)
						{
							pBuff[iBuf++] = (wchar_t)*pIn;
							if('\'' == *pIn && iBuf < nSize)
								pBuff[iBuf++] = L'\'';
						}
					}
				}
				else if('d' == *lpFmt)
				{
					// 替换数字
					long			nData		= va_arg(vaArr, long);
					wchar_t			szData[32]	= {0};
					wchar_t*		pData		= szData;

					_ltow(nData, szData, 10);
					while(iBuf < nSize && *pData)
						pBuff[iBuf++] = *pData++;
				}
				else
				{
					ATLASSERT(FALSE);
					pBuff[iBuf++] = '%';
					pBuff[iBuf++] = *lpFmt;
				}

				continue;
			}
			pBuff[iBuf++] = *lpFmt;
		}
	}

	int length()
	{
		return (int)wcslen(m_szSQL);
	}
	inline operator BSTR ()
	{
		return (BSTR)m_szSQL;
	}
};

template <int TSize = 256>
class TFieldString
{
public:
	TFieldString()
	{
		memset(m_szField, 0, sizeof(m_szField));
	}
	TFieldString(LPCSTR lpStr)
	{
		memset(m_szField, 0, sizeof(m_szField));
		*this = lpStr;
	}
	TFieldString(LPCWSTR lpStr)
	{
		memset(m_szField, 0, sizeof(m_szField));
		*this = lpStr;
	}
protected:
	wchar_t			m_szField[TSize];

public:
	TFieldString& operator =(LPCSTR lpStr)
	{
		m_szField[0] = 0;
		MultiByteToWideChar(CP_UTF8, 0, lpStr, (int)strlen(lpStr), m_szField, TSize);
		return *this;
	}
	TFieldString& operator =(LPCWSTR lpStr)
	{
		wcsncpy(m_szField, lpStr, sizeof(m_szField)/sizeof(wchar_t));
		return *this;
	}

	TFieldString& operator =(LONG nl)
	{
		m_szField[0] = 0;
		_ltow(nl, m_szField, 10);
		return *this;
	}

	operator LPCWSTR ()
	{
		return m_szField;
	}
};

template <class T>
class TDateString
{
public:
	TDateString()
	{
		memset(m_szDate, 0, sizeof(m_szDate));
		m_time = NULL;
	}
	TDateString(LPCSTR lpStr)
	{
		memset(m_szDate, 0, sizeof(m_szDate));
		m_time = NULL;
		*this = lpStr;
	}
	TDateString(LPCWSTR lpStr)
	{
		memset(m_szDate, 0, sizeof(m_szDate));
		m_time = NULL;
		*this = lpStr;
	}
protected:
	wchar_t			m_szDate[32];
	__time64_t		m_time;

public:
	TDateString& operator =(LPCSTR lpStr)
	{
		int		nY		= 0;
		int		nM		= 0;
		int		nD		= 0;
		char	c1, c2;
		
		sscanf(lpStr, "%d%c%d%c%d", &nY, &c1, &nM, &c2, &nD);
		m_time = FormatTime(nY, nM, nD);
		memset(m_szDate, 0, sizeof(m_szDate));
		return *this;
	}
	__time64_t FormatTime(int nYear, int nMonth, int nDay)
	{
		tm		time		= {0};
		
		// year
		if(nYear <= 50)
			time.tm_year = 2000 + nYear;
		else if(nYear < 100)
			time.tm_year = 1900 + nYear;
		else if(nYear > 1900)
			time.tm_year = nYear;
		else
			return NULL;
		// month
		if(nMonth > 0 && nMonth < 13)
			time.tm_mon = nMonth -1;
		else 
			return NULL;
		// day
		if(nDay > 0 && nDay < 32)
			time.tm_mday = nDay;
		else
			return NULL;
		
		return _mktime64(&time);
	}

	TDateString& operator =(LPCWSTR lpStr)
	{
		int		nY		= 0;
		int		nM		= 0;
		int		nD		= 0;
		char	nS1,nS2;

		wscanf(lpStr, "%d%c%d%c%d", &nY, &nS1, &nM, &nS2, &nD);
		m_time = FormatTime(nY, nM, nD);
		memset(m_szDate, 0, sizeof(m_szDate));
		return *this;
	}

	TDateString& operator + (LONG nDay)
	{
		__time64_t		td		= 0 + 60* (0 + 60* (0 + __int64(24) * nDay));

		if(NULL != m_time)
			m_time += td;
		return *this;
	}

	operator LPCWSTR ()
	{
		if(0 != m_szDate[0])
			return m_szDate;

		if(NULL == m_time)
			return m_szDate;

		tm*		pt		= _gmtime64(&m_time);

		_snwprintf(m_szDate, sizeof(m_szDate)/sizeof(wchar_t), L"%04d-%02d-%02d", pt->tm_year, pt->tm_mon, pt->tm_mday);
		return m_szDate;
	}
};
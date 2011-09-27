#pragma once

template <int TBuffSize = 2048>
class TSQLString
{
public:
	TSQLString()
	{
		clear();
	}
private:
	char		m_szSQL[TBuffSize];
	int			m_nPos;

protected:
	/*
	 *	添加一个字符串
	 */
	bool		push_string(const char* pstr, bool bConvert = false, int nsize = -1)
	{
		int			i		= 0;

		if(TBuffSize < 1 || m_nPos >= (TBuffSize - 1))
			return false;
		if(-1 == nsize)
			nsize = strlen(pstr);
		for(; pstr[i] && i < nsize && (m_nPos < (TBuffSize - 1)); i++)
		{
			m_szSQL[m_nPos++] = pstr[i];
			if(false != bConvert && '\'' == pstr[i])
				m_szSQL[m_nPos++] = '\'';
		}
		
		return NULL == pstr[i] || i == nsize;
	}
	/*
	 *	添加一个字符
	 */
	bool		push_char(char pch, bool bConvert = false)
	{
		char		szBuf[2]		= {0};

		szBuf[0] = pch;
		return push_string(szBuf, bConvert);
	}
	/*
	 *	清空
	 */
	void		clear()
	{
		m_nPos = 0;
		memset(m_szSQL, 0, sizeof(m_szSQL));
	}

public:
	/*
	 *	解析字符串
	 */
	void		format(const char* lpFmt, ...)
	{
		va_list		vaArr;

		clear();

		va_start(vaArr, lpFmt);
		vformat(lpFmt, vaArr);
		va_end(vaArr);
	}

	void		append(const char* lpFmt, ...)
	{
		va_list		vaArr;

		va_start(vaArr, lpFmt);
		vformat(lpFmt, vaArr);
		va_end(vaArr);
	}

	void		vformat(const char* lpFmt, va_list vaArr)
	{
		for(int iBuf = 0; *lpFmt; lpFmt++)
		{
			if('%' == *lpFmt)
			{
				lpFmt++;
				if('s' == *lpFmt)
				{
					// 替换文本
					char* pData		= va_arg(vaArr, char*);

					if(NULL == pData)
						continue;
					
					push_string(pData, true);
				}
				else if('d' == *lpFmt)
				{
					// 替换数字
					int				nData		= va_arg(vaArr, int);
					char			szData[32]	= {0};

					itoa(nData, szData, 10);
					push_string(szData);
				}
				else if('0' <= *lpFmt && *lpFmt <= '9')
				{
					char			szCount[32]		= {0};
					const char*		p				= lpFmt;
					int				i				= 0;

					for(; i < sizeof(szCount); i++)
					{
						if('0' > p[i] || p[i] > '9')
							break;
						szCount[i] = p[i];
					}
					if('s' == p[i])
					{
						char* pData		= va_arg(vaArr, char*);

						lpFmt += i;
						if(NULL == pData)
							continue;
						push_string(pData, true, atoi(szCount));						
					}
					else
					{
						push_string(lpFmt - 1, false, 2);
					}					
				}
				else
				{
					push_string(lpFmt - 1, false, 2);
				}

				continue;
			}
			// 默认添加
			if(false == push_string(lpFmt, false, 1))
				break;
		}
	}

	int length()
	{
		return m_nPos;
	}

	operator const char* ()
	{
		return m_szSQL;
	}
};
#pragma once
#include <assert.h>

#define SIZEOFARRAY(x)		( (int)(sizeof(x) / sizeof(x[0])) )

namespace xtd
{
	//////////////////////////////////////////////////////////////////////////
	// �ַ����滻

	/*
	 *	�滻�����ַ���
	 *  
	 *  pStr: Դ�ַ����� Ҳ��Ŀ���ַ���
	 *  chOld: Ҫ�滻���ַ�
	 *  chNew: �滻����ַ���
	 */
	template <class TChar>
	TChar* StrReplace(TChar* pStr, TChar chOld, TChar chNew)
	{
		TChar*	p	= pStr;

		for(; *p; p++)
		{
			if(chOld == *p)
				*p = chNew;
		}
		return pStr;
	}

	/*
	 *	�ַ����滻
	 *  
	 *  pStr: Ҫ�滻��ԭ����Ŀ�ش�
	 *	nSize: pStr�ɷ��ʵĴ�С
	 *  pOld: Ҫ�滻�����ַ���
	 *  pNew: ������ַ���
	 */
	template <typename TChar, int TSize>
	TChar* StrReplaceGreaterT(TChar* pStr, int nLen, const TChar* pOld, const TChar* pNew)
	{
		TChar		szTemp[TSize]		= {0};
		TChar*		pTemp				= szTemp;
		TChar*		pSrcStr				= pStr;
		TChar*		pDesStr				= pStr;

		// ��Ҫʱ�������ڴ�
		if(nLen >= TSize)
			pTemp = new TChar[nLen];

		if(NULL == pTemp)
		{
			assert(NULL != pTemp);
			return pStr;
		}

		for(int i = 0; i < nLen && *pSrcStr; pSrcStr++)
		{
			if(*pOld == *pSrcStr)
			{
				// �ַ����Ƚ�
				TChar*		pBody		= pSrcStr + 1;
				bool		bRep		= true;

				for(int l = 1; pOld[l]; l++)
				{
					if(pOld[l] != *pBody++)
					{
						bRep = false;
						break;
					}
				}

				if(bRep)
				{
					// �滻����
					for(int j = 0; pNew[j] && i < nLen; j++)
						pTemp[i++] = pNew[j];
					continue;
				}
			}
			// û���滻
			pTemp[i++] = *pSrcStr;
		}

		// ���ƴ�
		TChar*		pSrc	= pTemp;
		do 
		{
			*pDesStr++ = *pSrc;
		} while (*pSrc++);

		// �ͷ��ڴ�
		if(nLen >= TSize)
			delete [] pTemp;		
		return pStr;
	}
	/*
	 *	�Ƚ��������ĳ���
	 *  ����ֵ: -1 pFont�ĳ���С��pBack���� 0��� 1����
	 */
	template <typename TChar>
	int StrCmpLen(const TChar* pFront, const TChar* pBack)
	{
		for(int i = 0; pFront[i] || pBack[i]; i++)
		{
			if(0 == pFront[i])
				return -1;
			else if(0 == pBack[i])
				return 1;
		}

		return 0;
	}

	/*
	*	�ַ����滻���ֵ
	*  
	*  pStr: Ҫ�滻��ԭ����Ŀ�ش�
	*  nSize: pStr�ɷ��ʵĴ�С
	*  pOld: Ҫ�滻�����ַ���
	*  pNew: ������ַ���
	*/
	template <typename TChar>
	TChar* StrReplaceLess(TChar* pStr, const TChar* pOld, const TChar* pNew)
	{
		TChar*	pSearch		= pStr;
		TChar*	pDes		= NULL;
		
		// ��ʼ�滻
		for(; *pSearch; pSearch++)
		{
			if(*pOld == *pSearch)
			{
				TChar	*pBody		= pSearch + 1;
				bool	bRep		= true;
				int		l;

				for(l = 1; pOld[l]; l++)
				{
					if(pOld[l] != *pBody++)
					{
						bRep = false;
						break;
					}
				}
				// �滻����
				if(bRep)
				{
					const TChar*	pRep	= pNew;

					if(NULL == pDes)
						pDes = pSearch;
					while(*pRep)
						*pDes++ = *pRep++;
				}
			}
			// �´��ƶ�ָ��
			if(NULL != pDes)
				pDes++;
		}

		if(NULL != pDes)
			*pDes = 0;

		return pStr;
	}

	template <typename TChar>
	TChar* StrReplaceEx(TChar* pStr, int nLen, const TChar* pOld, const TChar* pNew)
	{
		if(StrCmpLen(pOld, pNew) < 0)
			return StrReplaceGreaterT<TChar, 1024>(pStr, nLen, pOld, pNew);

		return StrReplaceLess(pStr, pOld, pNew);
	}

	/*
	 *	�ַ����滻����
	 */
	template <typename TChar>
	TChar* StrReplaceCopy(TChar* pDes, int nLen, const TChar* pSrc, const TChar* pOld, const TChar* pNew)
	{
		const TChar*	pStr		= pSrc;
		int				i			= 0;

		nLen--;	// ���һ���ַ�Ϊ_T('\0')
		for(i = 0; (i < nLen) && *pStr; pStr++)
		{
			if(*pStr == *pOld)
			{
				const TChar*	pBody	= pStr + 1;
				bool			bRep	= true;

				for(int l = 1; pOld[l]; l++)
				{
					if(pOld[l] != *pBody++)
					{
						bRep = false;
						break;
					}
				}

				// �滻����
				if(bRep)
				{
					const TChar*	pRep	= pNew;

					while(*pRep && (i < nLen))
						pDes[i++] = *pRep++;

					continue;
				}
			}

			pDes[i++] = *pStr;
		}

		pDes[i] = 0;
		return pDes;
	}
	//////////////////////////////////////////////////////////////////////////
	// �ַ�����ת��
	template <int InCodePage, int OutCodePage, int TSize>
	char* ConvertCharacterT(const char* pInStr, int nInLen, char* pOutStr, int nOutLen)
	{
		int			nwLen				= 0;
		wchar_t		szTemp[TSize]		= {0};
		wchar_t*	pTemp				= szTemp;

		nwLen = ::MultiByteToWideChar(InCodePage, NULL, pInStr, nInLen, NULL, 0);
		
		if(nwLen >= TSize)
			pTemp = new wchar_t[nwLen];
		// ����ҳת��ΪUNICODE
		::MultiByteToWideChar(InCodePage, NULL, pInStr, nInLen, pTemp, nwLen);
		// UNICODEת��ΪĿ�����ҳ
		::WideCharToMultiByte(OutCodePage, NULL, pTemp, nwLen, pOutStr, nOutLen, NULL, NULL);

		if(nwLen >= TSize)
			delete [] pTemp;

		return pOutStr;
	}
	inline wchar_t* Utf8ToUnicode(const char* pUtf8, wchar_t* pDes, int nLen)
	{
		::MultiByteToWideChar(CP_UTF8, NULL, pUtf8, (int)strlen(pUtf8), pDes, nLen);
		return pDes;
	}
	inline char* Utf8ToAnsi(const char* pUtf8, char* pAnsi, int nLen)
	{
		return ConvertCharacterT<CP_UTF8, CP_ACP, 1024>(pUtf8, (int)strlen(pUtf8), pAnsi, nLen);
	}
	inline char* UnicodeToUtf8(const wchar_t* pUnicode, char* pDes, int nLen)
	{
		::WideCharToMultiByte(CP_UTF8, NULL, pUnicode, (int)wcslen(pUnicode), pDes, nLen, NULL, NULL);
		return pDes;
	}
	inline char* AnsiToUtf8(const char* pAnsi, char* pUtf8, int nLen)
	{
		return ConvertCharacterT<CP_ACP, CP_UTF8, 1024>(pAnsi, (int)strlen(pAnsi), pUtf8, nLen);
	}
}
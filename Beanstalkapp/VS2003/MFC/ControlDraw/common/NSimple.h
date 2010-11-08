#pragma once

#ifndef _WIN32_WINNT
#define _WIN32_WINNT	0x0501
#endif

#ifndef WINVER
#define WINVER			0x0501
#endif

#ifndef _WIN32_IE
#define	_WIN32_IE		0x0500
#endif


#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <tchar.h>
#include <assert.h>
#include <CommCtrl.h>
#include <ComDef.h>
#include <atlstr.h>
#include <ShellAPI.h>
#include <vector>
#include <string>
#include <list>
#include <map>
#include <aclapi.h>


#if defined(UNICODE) || defined(_UNICODE)
#define std_string	std::wstring
#else
#define std_string	std::string
#endif


class ByteMem;

#define GetDllFun(_DLL, _FUN)	GetProcAddress(::LoadLibraryA(_DLL), _FUN)

#ifdef BREAKIF
#undef BREAKIF
#endif

#ifdef _DEBUG
#define BREAKIF(Condition)	if(Condition)	__asm int 3;
#define DBG_CODE(X)			X;
#else
#define BREAKIF(Condition)
#define DBG_CODE(X)			;
#endif

#ifndef ANY_SIZE
	#define ANY_SIZE 1
#endif

#define		MAXPATH		(MAX_PATH*2)

#define RECT_W(RCT)		(RCT.right - RCT.left)
#define RECT_H(RCT)		(RCT.bottom - RCT.top)
#define RECT_PW(PRCT)		(PRCT->right - PRCT->left)
#define RECT_PH(PRCT)		(PRCT->bottom - PRCT->top)


#define PTR_VAL_TYPE(_PTR, _TP)		(*(_TP *)(_PTR))

#define PV_DWORD(_PTR)				PTR_VAL_TYPE(_PTR, DWORD)
#define PV_INT(_PTR)				PTR_VAL_TYPE(_PTR, int)
#define PV_BYTE(_PTR)				PTR_VAL_TYPE(_PTR, BYTE)


#define SET_DWORD(_POINT, _VAL)		(*(DWORD *)(_POINT) = _VAL)
#define SET_WORD(_POINT, _VAL)		(*(WORD *)(_POINT) = _VAL)
#define SET_BYTE(_POINT, _VAL)		(*(BYTE *)(_POINT) = _VAL)


#ifdef _DEBUG
#define 	DbgMemset(_Ptr, _Val, _Size)	memset(_Ptr, _Val, _Size)
#else
#define 	DbgMemset(_Ptr, _Val, _Size)
#endif


class CMiniMem
{
public:
	CMiniMem()
	{

	}
	~CMiniMem()
	{
		FreeAll();
	}

	//	int		Arry2D[x][y];	->	Alloc2dArry(nCount1, y*sizeof(int))
	VOID **	Alloc2dArry(size_t nCount1, size_t nCount2, BOOL bZero = TRUE)
	{
		size_t		nTotalSize;
		BYTE *		pAlloc;
		BYTE *		pRealData;
		VOID **		ppRetVal;
		int			i;

		nTotalSize = nCount1*nCount2 + nCount1*sizeof(VOID *);
		pAlloc = (BYTE *)Alloc(nTotalSize, bZero);

		pRealData = pAlloc + nCount1*sizeof(VOID *);

		ppRetVal = (VOID **)pAlloc;
		for(i=0; i<(int)nCount1; i++)
		{
			ppRetVal[i] = pRealData + nCount2*i;
		}

		return ppRetVal;
	}

	VOID *	Alloc(size_t nSize, BOOL bZero = TRUE)
	{
		VOID *		pMem;

		pMem = malloc(nSize);
		if(bZero)
			memset(pMem, 0, nSize);

		m_MemList.push_back(pMem);

		return pMem;
	}

	BOOL	Free(VOID *pBuffer)
	{
		std::vector<VOID *>::iterator	Iter;

		for(Iter = m_MemList.begin();
			Iter != m_MemList.end();
			Iter ++)
		{
			if(*Iter == pBuffer)
			{
				free(pBuffer);
				m_MemList.erase(Iter);
				return TRUE;
			}
		}
		assert(0);
		return FALSE;
	}

	BOOL	FreeAll()
	{
		std::vector<VOID *>::iterator	Iter;

		for(Iter = m_MemList.begin();
			Iter != m_MemList.end();
			Iter ++)
		{
			free(*Iter);
		}
		m_MemList.clear();

		return TRUE;
	}

	std::vector<VOID *>	m_MemList;
};



class CAutoHandle
{
public:
	CAutoHandle()
	{
		m_hHandle = NULL;
	}
	~CAutoHandle()
	{
		Close();
	}

	VOID	Close()
	{
		if(m_hHandle != NULL && m_hHandle != INVALID_HANDLE_VALUE)
			CloseHandle(m_hHandle);
		m_hHandle = NULL;
	}


	CAutoHandle& operator = (HANDLE hHandle)
	{
		Close();
		m_hHandle = hHandle;
		return *this;
	}

	HANDLE * operator&()
	{
		return &m_hHandle;
	}

	HANDLE * GetAddr()
	{
		return &m_hHandle;
	}

	operator HANDLE() const
	{
		return m_hHandle;
	}

	BOOL	IsVaildHandle()
	{
		if(m_hHandle == NULL ||
			m_hHandle == INVALID_HANDLE_VALUE)
			return FALSE;

		return TRUE;
	}

	HANDLE		m_hHandle;
};


class CAutoMem
{
public:
	CAutoMem()
	{
		m_pData = NULL;
		m_nSize = 0;
	}

	~CAutoMem()
	{
		Clear();
	}

	CAutoMem& operator=(const CAutoMem &Right)
	{
		Alloc(Right.m_nSize, Right.m_pData);
		return *this;
	}

	CAutoMem& operator=(VOID *pBuffer)
	{
		m_pData = pBuffer;
		m_nSize = -1;
		return *this;
	}

	BOOL	Clear()
	{
		if(m_pData && m_nSize)
		{
			free(m_pData);
			m_pData = NULL;
		}
		m_nSize = 0;
		return TRUE;
	}

	BOOL	Alloc(int nSize, VOID *pBuffer = NULL)
	{
		Clear();

		m_nSize = nSize;
		m_pData = malloc(m_nSize);
		if(pBuffer)
			memcpy(m_pData, pBuffer, nSize);

		return TRUE;
	}

	BOOL	GetSize()
	{
		return m_nSize;
	}


	BOOL	SetPointer(VOID *pBuffer, int nSize = 0)
	{
		Clear();
		m_nSize = nSize;
		m_pData = pBuffer;
		return TRUE;
	}

	VOID * &	GetPtr()
	{
		return m_pData;
	}

	operator VOID *() const
	{
		return m_pData;
	}


	VOID *	m_pData;
	int		m_nSize;
};


namespace NData
{
	typedef enum _DATA_PTR_TPYE
	{
		DataPtrType_First  = 0,

		DataPtrType_BYTE		= 0,
		DataPtrType_CHAR		= 1,
		DataPtrType_CHAR_Ary	= 2,	//2维数组
		DataPtrType_CHAR_NoCase	= 3,	//不区分大小写
		DataPtrType_WORD		= 4,
		DataPtrType_WCHAR		= 5,
		DataPtrType_short		= 6,
		DataPtrType_DWORD		= 7,
		DataPtrType_int			= 8,
		DataPtrType_long		= 9,
		DataPtrType_BOOL		= 10,
		DataPtrType_UINT		= 11,
		DataPtrType_float		= 12,
		DataPtrType_double		= 13,
		DataPtrType_VOID		= 14,
		DataPtrType_P_CHAR		= 15, //CHAR *[]
		DataPtrType_P_WCHAR		= 16, //WCHAR *[]


		DataPtrType_Last			= 14
	}DATA_PTR_TPYE;


	typedef enum _DATA_TPYE
	{
		DataType_Ptr_Base			= 100,

		DataType_None			= 0,
		DataType_BYTE			= 1,
		DataType_CHAR			= 2,
		DataType_WORD			= 4,
		DataType_WCHAR			= 5,
		DataType_short			= 6,
		DataType_DWORD			= 7,
		DataType_int			= 8,
		DataType_long			= 9,
		DataType_BOOL			= 10,
		DataType_UINT			= 11,
		DataType_float			= 12,
		DataType_double			= 13,
		DataType_VOID			= 14,
		DataType_int64			= 15,
		DataType_CHAR_NoCase	= 20,	//不区分大小写

		DataType_BYTE_Ptr			= DataType_Ptr_Base + DataType_BYTE,
		DataType_CHAR_Ptr			= DataType_Ptr_Base + DataType_CHAR,
		DataType_CHAR_NoCase_Ptr	= DataType_Ptr_Base + DataType_CHAR_NoCase,
		DataType_WORD_Ptr			= DataType_Ptr_Base + DataType_WORD,
		DataType_WCHAR_Ptr			= DataType_Ptr_Base + DataType_WCHAR,
		DataType_short_Ptr			= DataType_Ptr_Base + DataType_short,
		DataType_DWORD_Ptr			= DataType_Ptr_Base + DataType_DWORD,
		DataType_int_Ptr			= DataType_Ptr_Base + DataType_int,
		DataType_long_Ptr			= DataType_Ptr_Base + DataType_long,
		DataType_BOOL_Ptr			= DataType_Ptr_Base + DataType_BOOL,
		DataType_UINT_Ptr			= DataType_Ptr_Base + DataType_UINT,
		DataType_float_Ptr			= DataType_Ptr_Base + DataType_float,
		DataType_double_Ptr			= DataType_Ptr_Base + DataType_double,
		DataType_VOID_Ptr			= DataType_Ptr_Base + DataType_VOID,

		DataType_PSTR			= 15, //CHAR *
		DataType_PSTR_NoCase	= 16, //CHAR *
		DataType_PWSTR			= 17, //WCHAR *
		DataType_PWSTR_NoCase	= 18, //WCHAR *

	}DATA_TPYE;

	DWORD	GetBigDataMax(VOID *ptr, DWORD dwCount, DWORD *pMaxIndex, DWORD dwMaxCount, DATA_PTR_TPYE type);
	DWORD	Sort(VOID * ptr, DWORD pOrder[], DWORD n, DATA_PTR_TPYE type);
	DWORD	qSort(VOID * ptr, DWORD pOrder[], DWORD dwDataCount, DATA_PTR_TPYE type);
	double	Lagrange(int n, double *px, double *py, double x);


	BOOL	IsHexChar(int ch);
	BYTE	Hex2Byte(CHAR c1, CHAR c2);
	DWORD	DataToHex(VOID * pMem, DWORD dwMemSize, _bstr_t *pbStrHex, int nOneLineChars = 128);
	DWORD	DataToHex(VOID * pMem, DWORD dwMemSize, CHAR *pHexStr, DWORD dwHexSize, int nOneLineChars = 128);
	DWORD	HexToData(CHAR * pHexString, VOID * pMem, DWORD dwMemSize);
	ULONGLONG	HexToInt64(CHAR * pHexString);

	DWORD	SimpleCrypt(VOID *pMem, DWORD dwSize, int nKey = 26);
	VOID *	Align(VOID *p, int nSize = 4);
	DWORD	Align(DWORD dwVal, DWORD dwSize = 4);
	ULONGLONG	Align(ULONGLONG dwVal, ULONGLONG uuAglinSize);

	VOID *	MemFindMem(VOID * pMem, UINT uMemSize, VOID * pFind, UINT uFindSize);
	VOID *	MemFindStr(VOID * pMem, UINT uMemSize, CHAR * pString);
	VOID *	MemFindStrNoCase(CHAR *pString, VOID *pMem, UINT uMemSize = -1);

	DWORD	HashData(VOID *pData, DWORD dwSize = 0);

	//	nMin <= Ret < nMax
	int		RandomInt(int nMin, int nMax);

	//	10987654 32109876 54321098 76543210
	//	从指定Bit位置获取指定数目2进制
	DWORD	GetBit(DWORD dwValue, int nBitPos, int nBitLen);

	DWORD	NextKey(DWORD dwKey);


	template<class TYPE>
		BOOL	DataToText(TYPE *pData, int nCount, std::string &sText, CHAR *pFormat = "%d, ", int	nOneLineCount = 16)
	{
		CHAR		szOneText[4096];
		int			i;
		int			nLineCount;

		sText = "";

		nLineCount = 0;
		for(i=0; i<nCount; i++)
		{
			sprintf(szOneText, pFormat, pData[i]);
			nLineCount ++;

			sText += szOneText;
			if(nLineCount >= nOneLineCount)
			{
				nLineCount = 0;
				sText += "\r\n";
			}
		}

		return TRUE;
	}


	template<class TYPE>
		BOOL	DbgArry100(TYPE *pPoint)
	{
		TYPE (&Temp) [100] = (TYPE (&) [100])(*pPoint);
		return TRUE;
	}

	template<class TYPE>
		BOOL	DbgArry10K(TYPE *pPoint)
	{
		TYPE (&Temp) [10*1024] = (TYPE (&) [10*1024])(*pPoint);
		return TRUE;
	}


	template<class TYPE>
		TYPE	PtrIncrease(TYPE pPoint, size_t nSize)
	{
		pPoint = TYPE((BYTE *)pPoint + nSize);
		return pPoint;
	}

	template<class TYPE>
		int	__cdecl Compare(TYPE &Item1, TYPE &Item2)
	{
		if(Item1 < Item2)
			return -1;
		if(Item1 > Item2)
			return 1;

		return 0;
	};

	template<class TYPE>
	int	__cdecl TQSortCompare(const void *pItem1, const void *pItem2)
	{
		TYPE *		p1 = (TYPE *)pItem1;
		TYPE *		p2 = (TYPE *)pItem2;

		if(*p1 < *p2)
			return -1;
		if(*p2 < *p1)
			return 1;

		return 0;
	};

	template<class TYPE>
	VOID		QSort(TYPE *pItems, size_t nCount)
	{
		qsort(pItems, nCount, sizeof(TYPE), TQSortCompare<TYPE>);
		return;
	};

	template<class TYPE>
		TYPE *	BSearch(TYPE *pItems, size_t nCount, TYPE &Item)
	{
		VOID *		pFind;
		pFind = ::bsearch(&Item, pItems, nCount, sizeof(TYPE), TQSortCompare<TYPE>);
		return (TYPE *)pFind;
	};

	template<class TYPE>
		TYPE *	TMalloc(size_t nCount = 1)
	{
		TYPE *		pItem;
		int			nSize;

		nSize = (int)(sizeof(TYPE) * nCount);
		pItem = (TYPE *)::malloc(nSize);
		memset(pItem, 0, nSize);

		return pItem;
	};

	template<class TYPE>
		VOID *	ConvertPtr(TYPE Value)
	{
		union
		{
			TYPE	Value;
			VOID *	pValue;
		}ConvertPtrUnion;

		ConvertPtrUnion.Value = Value;
		return ConvertPtrUnion.pValue;
	};

}



namespace	NMsg
{
	int		MsgBox(CHAR *pText, CHAR *pTitle = NULL, HWND hWnd = NULL, UINT uType = -1);

	BOOL	SetMsgParams(HWND hMsgWnd = (HWND)-1, CHAR *pMsgCpt = NULL, UINT uType = -1);
	int		Msg(CONST CHAR * lpszFormat, ...);
	int		Msgs(CONST CHAR * pMessage);
	int		MsgErr(int nError = 0);
	DWORD	MsgAry(void * ptr, DWORD n, NData::DATA_PTR_TPYE type, DWORD size = 0);

//	0xFFFFFFFF Standard beep using the computer speaker 
//	MB_ICONASTERISK SystemAsterisk 
//	MB_ICONEXCLAMATION SystemExclamation 
//	MB_ICONHAND SystemHand 
//	MB_ICONQUESTION SystemQuestion 
//	MB_OK SystemDefault 
	int		Msgb(int nType = MB_OK);
	int		MsgBeep(int nType = MB_OK);

	static	HWND			m_hMsgWnd = NULL;
	static	UINT			m_uType = MB_OK | MB_ICONSTOP | MB_TASKMODAL;
	static	CHAR			m_szMsgCpt[1024] = "Msg";
}


namespace	NReg
{
	#define REG_SUBITEM			99

	BOOL	GetReg(HKEY hKey, CHAR *pSubKey, CHAR *pValueName, BYTE *pData, DWORD cbData);
	BOOL	GetReg(HKEY hKey, CHAR *pSubKey, CHAR *pValueName, CHAR *pBuffer, int nBufSize = 1024);
	BOOL	GetReg(HKEY hKey, CHAR *pSubKey, CHAR *pValueName, DWORD *pdwValue);

	BOOL	SHGetReg(HKEY hKey, CHAR *pSubKey, CHAR *pValueName, BYTE *pData, DWORD cbData);
	BOOL	SHGetReg(HKEY hKey, CHAR *pSubKey, CHAR *pValueName, CHAR *pData, int nBufSize);
	BOOL	SHGetReg(HKEY hKey, CHAR *pSubKey, CHAR *pValueName, DWORD *pdwValue);

	BOOL	SetReg(HKEY hKey, CHAR *pSubKey, CHAR *pValueName, CHAR *pData, BOOL bFlush = FALSE);
	BOOL	SHSetReg(HKEY hKey, CHAR *pSubKey, CHAR *pValueName, CHAR *pData);
	BOOL	SHSetReg(HKEY hKey, CHAR *pSubKey, CHAR *pValueName, DWORD dwValue);
	BOOL	SHSetRegExsz(HKEY hKey, CHAR *pSubKey, CHAR *pValueName, CHAR *pData);

	BOOL	SetRegMultisz(HKEY hKey, CHAR *pSubKey, CHAR *pValueName, CHAR *pData, BOOL bFlush = FALSE);
	BOOL	SetReg(HKEY hKey, CHAR *pSubKey, CHAR *pValueName, DWORD dwValue, BOOL bFlush = FALSE);


	BOOL	DelReg(HKEY hKey, CHAR *pSubKey, CHAR *pValueName);
	BOOL	DelRegTree(HKEY hKey, CHAR *pSubKey);
	BOOL	CopyRegTree(HKEY hDestKey, CHAR *pDestSubKey, HKEY hSrcKey, CHAR * pSrcSubKey);

	CHAR *	GetRootKeyStr(HKEY hKey, CHAR *pBuf);
	CHAR *	GetRootKeyLongStr(HKEY hKey, CHAR *pBuf);

	BOOL	ParseRootKey(CHAR *pRootKey, HKEY *pKey);
	BOOL	ParseKrnRegPath(CHAR *pFullPath, HKEY *pRootKey, CHAR *pSubKey);
	BOOL	ParseRegPath(CHAR *pFullPath, HKEY *pRootKey, CHAR *pSubKey);
	BOOL	ParseRegPath(CHAR *pFullPath, HKEY *pRootKey, CHAR *pSubKeyName, CHAR *pValueName);
	//	转换注册表路径到字符串
	BOOL	ConvertRegPath(CHAR *pFullPath, HKEY RootKey, CHAR *pSubKey, CHAR *pValueName = NULL, BOOL bRootLong = FALSE);

	typedef BOOL (WINAPI *REGISTER_ENUM_PROC)(VOID *pParam, HKEY hRootKey, CHAR *pSubKey, CHAR *pValueName, DWORD dwType, BYTE *pValueData, DWORD cbData);

	BOOL	RegisterEnum(REGISTER_ENUM_PROC pEnumProc, VOID *pParam, HKEY hRootKey, CHAR *pSubKey, BOOL bEnumSub = TRUE);

	BOOL	DbgSaveLastMsg(CHAR *pMessage);

	BOOL	GetMUICache(std::map<std::string, std::string> &mAppCache);
	BOOL	GetUninstallList(std::map<std::string, std::string> &mAppUninstall);

	BOOL	OpenAndSelectRegItem(HKEY hKey, CHAR *pSubKey, CHAR *pValueName = NULL);
	
}



namespace NLog
{
	enum TIMED_LOG_TYPE
	{
		LT_TYPE_DAY = 0,
		LT_TYPE_TIME = 1,
		LT_TYPE_TIME_mS = 2
	};


	int		Logs(CHAR *pString, CHAR *pLogFile = NULL);
	BOOL	SetLogFile(CHAR *pNewLogFile, BOOL bClear = FALSE);

	int		LogString(CONST CHAR * lpszFormat, ...);
	int		LogPrintf(CONST CHAR * lpszFormat, ...);
	
	int		LogFile(CHAR *pLogFile, CONST CHAR * lpszFormat, ...);
	int		FolderFileLog(CHAR *pLogFolder, CHAR *pLogFile, CONST CHAR * lpszFormat, ...);

	int		SetTimedLogType(TIMED_LOG_TYPE nType = LT_TYPE_TIME);
	int		TimedLog(CONST CHAR * lpszFormat, ...);
	int		TimedLogFile(CHAR *pLogFile, CONST CHAR * lpszFormat, ...);




	int		LogMem(VOID *pMem, DWORD dwSize, BOOL bUnicode = FALSE);


	int		DbgLog(CONST CHAR * lpszFormat, ...);
	int		DbgLogMem(VOID *pMem, DWORD dwSize, BOOL bUnicode = FALSE);
	int		DbgLogFile(CHAR *pLogFile, CONST CHAR * lpszFormat, ...);

	int		DbgOutput(CONST CHAR * lpszFormat, ...);

	DWORD	LogTimeStart();
	int		LogTime(CONST CHAR * lpszFormat, ...);

	static TIMED_LOG_TYPE	s_TimeLogType = LT_TYPE_TIME;
	static DWORD		s_dwTimer = 0;
	static CHAR			s_ThisLogFile[MAX_PATH] = "C:\\Debug\\NLog.log";
	static CHAR			s_ThisDbgLog[MAX_PATH] = "C:\\Debug\\DbgLog.log";
}



namespace NStr
{
	BOOL	IsNumber(CHAR c);
	BOOL	IsAlph(CHAR c);
	BOOL	IsAlphUpr(CHAR c);
	BOOL	IsAlphLwr(CHAR c);
	BOOL	IsNumberAlph(CHAR c);
	BOOL	IsOperatChar(CHAR c);
	CHAR	CharUpr(CHAR c);


	//插入
	CHAR *	StrInsert(CHAR *str, int p, CHAR *InsertStr);
	//删除
	CHAR *	StrDel(CHAR *str, int p, int n);
	//替换

	int		StrReplace(CHAR *pString, CHAR cOld, CHAR cNew);
	int		StrReplace(CHAR *str, CHAR *scr_str, CHAR *dest_str);
	//忽略大小写替换
	int		StrIReplace(CHAR *str, CHAR *scr_str, CHAR *dest_str);

	// Check Length nDestSize:字节数   返回字符数
	int		StrCopy(CHAR *pDes, CONST CHAR *pSor, int nDestSize = -1);
	int		WcsCopy(WCHAR *pDes, CONST WCHAR *pSor, int nDestSize = -1);

	BOOL	CharCmp(CHAR a, CHAR b, BOOL bNoCase = TRUE);

	//Append '\0' 长度nCount 不包括'\0'
	CHAR	* StrnCopy(CHAR *pDes, CONST CHAR *pSor, size_t nCount);	
	//	头部比较
	int		StrHeadCmp(CHAR *pLongStr, CHAR *pStr, BOOL bNoCase = TRUE);
	int		StrHeadCmp(WCHAR *pLongStr, WCHAR *pStr, BOOL bNoCase = TRUE);
	//	尾部比较
	int		StrTailCmp(CHAR *pLongStr, CHAR *pStr, BOOL bNoCase = TRUE);

	//	替换BYTE
	DWORD	MemReplaceBYTE(VOID * pMem, DWORD dwMemSize, BYTE oldval, BYTE newval);
	//	替换WORD
	DWORD	MemReplaceWORD(void * Mem, DWORD dwMemSize, WORD oldval, WORD newval);	

	//	替换不可显示字符
	BOOL	MemToStr(VOID * pMem, int nSize, _bstr_t * pbString, BOOL bUniCode = FALSE);
	BOOL	MemToStr(VOID * pMem, int nSize, BOOL bUniCode, VOID *pBuffer);

	//	
	BOOL	MemToString(VOID *pMem, int nSize, std::string &sText, BOOL bUniCode = FALSE);

	//	这函数比较慢
	BOOL	MemToHex(VOID * pMem, int nSize, _bstr_t * pbString, DWORD dwAddress, BOOL bUniCode = FALSE);

	//	需要 free
	CHAR *	MemToHexView(VOID * pMem, int nSize, DWORD dwAddress = 0, BOOL bUniCode = FALSE);

	//	需要 free
	CHAR *	WINAPIV MallocByString(CHAR *pString);
	VOID	WINAPIV FreeString(CHAR *pString);



	//带?的查找
	CHAR	*StrFormatStr(CHAR *pString, CHAR *pFormat);

	//	通配比较 * ?
	BOOL	WildcardCompare(CHAR *pString, CHAR *pWildcard, BOOL bNoCase = FALSE);
	CHAR *	WildcardFind(CHAR *pString, CHAR *pWildcard, BOOL bNoCase = FALSE);

	int		StrTrimRight(CHAR *pString, CHAR *pTrim);
	int		StrTrimRightRN(CHAR *pString);
	//	最大1024
	int		StrTrimTwoSide(CHAR *pString, CHAR *pTrim = " ");
	CHAR *	StrChr(CHAR *pString, CHAR *pFind);
	CHAR *	StrStr(CHAR *pString, CHAR *pFind, BOOL bNoCase = TRUE);
	int		StrCompare(CHAR *pString1, CHAR *pString2, BOOL bNoCase = TRUE);

	//	获取一行, 返回0: 没有数据	1:有数据	2:最后一行数据
	int		GetLine(CHAR ** ppLastPos, CHAR * pBuffer, CHAR * pOneLine = NULL, int nLineMaxSize = 1024);
	//CHAR  Arry[ArryCount][ArrySize] bFormat:转义
	DWORD	StrtoFormatArry(CHAR *str, CHAR * Arry, DWORD ArryCount, DWORD ArrySize = 1024, BOOL bFormat = FALSE);	
	//CHAR  szItems[nItemCount][nItemSize]
	int		SplitString(CHAR *pString, CHAR * pItems, int nItemCount, int nItemSize, CHAR *pSplitChars = NULL);
	int		SplitString(CHAR *pString, std::vector<std::string> &strList, CHAR *pSplitChars = NULL);
	//	长字符串获取一段, pSplit为分割字符集, 当pBuffer=NULL时初始化
	CHAR *	GetOnePart(CHAR ** ppLastPos, CHAR *pString, CHAR *pBuffer = NULL, int nBufSize = 1024, CHAR *pSplit = ";&");

	//	复制到剪贴板
	BOOL	CopyToClip(CHAR *pString, DWORD dwLen = 0);
	BOOL	CopyToClip(WCHAR *pString, DWORD dwLen = 0);

	BOOL	GetClipString(std::string &sText);

	//	"\n"或"\r"  换成 "\r\n"
	BOOL	FormatMulitiLine(CHAR *pString, CHAR *pNewString, int nBufSize, BOOL bDelEmpty = TRUE);



	//	null-terminated strings, terminated by two null characters
	BOOL	MultiString2MultiLine(CHAR *pSor, CHAR *pDest);
	//	CHAR  szItems[nItemCount][nItemSize]
	int		MultiString2Arrys(CHAR *pString, CHAR *pItems, int nItemCount, int nItemSize);
	//	两个 '\0' 结束
	int		MultiString2Arrys(CHAR *pString, std::vector<std::string> &strList);

	//	CHAR *pSplit = "\r\n"
	int		Vector2MultiText(std::vector<std::string> &vStrList, std::string &sMultiText, CHAR *pSplit = "\r\n");


	//	nDestLen: 字节数, 不是个数
	int		TChar2Char(TCHAR *pSor, CHAR *pDest, int nDestLen = -1);
	int		TChar2WChar(TCHAR *pSor, WCHAR *pDest, int nDestLen = -1);
	int		Char2TChar(CHAR *pSor, TCHAR *pDest, int nDestLen = -1);
	int		Char2WChar(CHAR *pSor, WCHAR *pDest, int nDestLen = -1);
	int		Char2WChar(CHAR *pSor, std::wstring &wString);

	int		WChar2TChar(WCHAR *pSor, TCHAR *pDest, int nDestLen = -1);
	int		WChar2Char(WCHAR *pSor, CHAR *pDest, int nDestLen = -1);
	int		WChar2Char(WCHAR *pSor, std::string &sText);
	int		WChar2Char(std::wstring &wString, std::string &sText);

	VOID	String2Time(CHAR *pString, SYSTEMTIME *pTime);
	CHAR *	Time2String(CHAR *pString, size_t nSize = 128, BOOL bMs = FALSE, SYSTEMTIME *pTime = NULL);
	CHAR *	Day2String(CHAR *pString, size_t nSize = 128, SYSTEMTIME *pTime = NULL);


	BOOL	GetFormatedTime(CHAR *pString, FILETIME &ft, BOOL bShowMS = FALSE);
	BOOL	GetFormatedTime(CHAR *pString, SYSTEMTIME &st, BOOL bShowMS = FALSE);

	CHAR *	GetDateTime(CHAR *pString, SYSTEMTIME *pTime = NULL, BOOL bMs = FALSE);
	CHAR *	GetDateTime(CHAR *pString, FILETIME &ft, BOOL bMs = FALSE);
	CHAR *	GetDateTime(CHAR *pString, time_t t);

	WCHAR *	GetDateTime(WCHAR *pString, SYSTEMTIME *pTime = NULL, BOOL bMs = FALSE);
	WCHAR *	GetDateTime(WCHAR *pString, FILETIME &ft, BOOL bMs = FALSE);
	WCHAR *	GetDateTime(WCHAR *pString, time_t t);

	CHAR *	GetDate(CHAR *pString, SYSTEMTIME *pTime = NULL);
	CHAR *	GetTime(CHAR *pString, BOOL bMs = FALSE, SYSTEMTIME *pTime = NULL);
	CHAR *	GetTime(CHAR *pString, __time64_t tm64, BOOL bLocalConvrt = FALSE);
	CHAR *	GetTime(CHAR *pString, FILETIME &ft, BOOL bMs = FALSE);


	CHAR *	TickCount2Second(DWORD dwTime, CHAR *pBuffer = NULL);
	DWORD	Str2Dowrd(CHAR *pString);

	BOOL	Big5ToGbk(CHAR *pBig, CHAR *pGbk = NULL, int nBufSize = -1);
	BOOL	CodePageConvert(UINT uInCodePage, UINT uOutCodePage, CHAR *pIn, CHAR *pOut, int nOutSize = -1);


	//	CHAR *pSeed = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	BOOL	CreateRandomString(CHAR *pBuffer, int nSize, CHAR *pSeed = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	int		ConvertValBySeedString(CHAR c, CHAR *pSeed = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	int		ConvertValBySeedString(CHAR *pBuffer, DWORD &dwValue, CHAR *pSeed = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	int		ConvertValBySeedString(CHAR *pBuffer, ULONGLONG &uuValue, CHAR *pSeed = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");

	


	typedef BOOL (WINAPI *LOOP_FIND_PROC)(CHAR *pFind, VOID *pParam);
 
	//	LOOP_FIND_PROC 返回 TRUE 继续, 返回FALSE停止查找
	DWORD	LoopFindNoCaseString(LOOP_FIND_PROC LoopProc, CHAR *pBuffer, CHAR *pFind, VOID *pParam);

	//	pUprStr 需要查找的字符传, 需要转换大写
	CHAR *	StrFindNoCase(CHAR *pString, CHAR *pUprStr);

	//	C语言形式的字符串转换, 如果pBuffer 非NULL则需要 free
	CHAR *	CodeString2String(CHAR *pCodeString, CHAR *pBuffer = NULL, int *pnCount = NULL);

	//	转换成C语言形式的字符串, 如果pBuffer 非NULL则需要 free
	CHAR *	String2CodeString(CHAR *pString, CHAR *pBuffer = NULL);

	//	0x35, 0x45 ...
	CHAR *	Mem2CodeArry(VOID *pMem, DWORD dwMemSize, std::string &sText, int nOneLineCount = 16);

	//	IDA字符串转换成C语言形式的字符串
	BOOL	IdaString2CodeString(CHAR *pIdaString, std::string &sText);

	BOOL	HtmString2String(CHAR *pHtmStr, std::string &sText);


	BOOL	IdaString2Mem(CHAR *pIdaString, std::vector<ByteMem> &MemList);
	BOOL	IdaString2Mem(CHAR *pIdaString, ByteMem &Mem);

	//	比如	{a}{s}{sdsd+sg}{dfgsdfg} 分割成多个
	int		GetGroupText(CHAR *pMutiKey, std::vector<std::string> &sTextList, CHAR cHead = '{', CHAR cTail = '}');


//	加千位计数符号 ','
	CHAR *	Value2StringThud(int nValue, CHAR *pBuffer = NULL);
	WCHAR *	Value2StringThud(int nValue, WCHAR *pBuffer = NULL);

	BOOL	CheckStrBufferOver(CHAR *pBuffer, int nBufSize);
}


namespace NSys
{
	enum	SYS_LANGUAGE
	{
		Lang_Error = 0,
		Lang_English = 1,
		Lang_Chinese = 2,
		Lang_UnKnow = 3
	};

	BOOL	LoopSleep(DWORD dwSleepSecond = -1);
	BOOL	EnablePrivilege(TCHAR * name, BOOL fEnable = TRUE, HANDLE hToken = NULL);
	BOOL	EnableProcPrivilege(DWORD PID = 0);

	//	Global\\...
	BOOL	AppMultiRunCheck(CHAR * pMutexName);
	BOOL	SetWorkDir(CHAR *pProDir = NULL);
	BOOL	GetProcWorkDir(CHAR *pWorkDir);
	VOID	IdleSleep(DWORD dwTime, HWND hWnd = NULL);
	VOID	DoMessageLoop(HWND hWnd);


	FARPROC	GetDllProcAdders(CHAR *pDllName, CHAR *pProcName);

	BOOL	CreateAndStartService(CHAR *pFilePath, CHAR *pServiceName, 
		DWORD dwStartType = SERVICE_AUTO_START, LPCSTR lpLoadOrderGroup = NULL, BOOL bStart = TRUE);

	BOOL	CreateService(CHAR *pDrvPath, CHAR *pServiceName, 
		DWORD dwServiceType = SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS , 
		DWORD dwStartType = SERVICE_AUTO_START, 
		CHAR *pDisplayName = NULL);

	BOOL	GetServiceStatus(CHAR *pServiceName, DWORD *pdwStatus);
	BOOL	CheckServerExist(CHAR *pServiceName);
	BOOL	StartUpService(CHAR *pServiceName, DWORD dwWait = 10000);
	BOOL	PauseService(CHAR *pServiceName, BOOL bResume = FALSE, DWORD dwWait = 10000);
	BOOL	StopService(CHAR *pServiceName, DWORD dwWait = 10000);
	BOOL	DeleteService(CHAR *pServiceName, DWORD dwWait = 10000);
	BOOL	ChangeServerDisplayName(CHAR *pServiceName, CHAR *pServiceDisplayName);
	BOOL	ChangeServerDescrip(CHAR *pServiceName, CHAR *pServiceDescrip);
	BOOL	ChangeServerImgPath(CHAR *pServiceName, CHAR *pExePath);
	BOOL	ChangeServerDriverImgPath(CHAR *pServiceName, CHAR *pDriverPath);
	BOOL	SetServerDependOnService(CHAR *pServiceName, CHAR *pDependSvr);
	BOOL	SetServerDependOnGroup(CHAR *pServiceName, CHAR *pDependGrp);

	DWORD	GetServcieStartType(CHAR *pSvrName);
	BOOL	SetServcieStartType(CHAR *pSvrName, DWORD dwStartType);
	DWORD	GetServcieType(CHAR *pSvrName);
	BOOL	SetServcieType(CHAR *pSvrName, DWORD dwServiceType);
	BOOL	GetServiceDescrip(CHAR *pSvrName, CHAR *pServiceDescrip);

	BOOL	KillProc(DWORD dwProcId = 0, DWORD dwExitCode = ERROR_PROCESS_ABORTED);
	BOOL	KillProc(CHAR *pProcName, DWORD dwExitCode = ERROR_PROCESS_ABORTED);
	BOOL	GetProcIdFromName(CHAR *pProcName, DWORD *pdwProcId);
	BOOL	GetProcIdFromName(CHAR *pProcName, std::vector<DWORD> &PidList);
	BOOL	GetProcNameFromId(DWORD dwProcId, CHAR *pProcName);
	DWORD	GetProcModAddr(CHAR *pProcName, CHAR *pModName);
	DWORD	GetProcModAddr(DWORD dwPid, CHAR *pModName);
	BOOL	GetProcIdFromNameNt4(CHAR *pProcName, DWORD *pdwProcId);

	int		GetExeMultiProcess(CHAR *pProcName);

	BOOL	Execute(CHAR *pExeFile, CHAR *pCmdLine, BOOL bWait = FALSE, BOOL bHide = FALSE, DWORD *pExitCode = NULL);
	BOOL	ExecuteBk(CHAR *pCmdLine, DWORD *pExitCode, DWORD dwTmLimt = 10000);
	HANDLE	RunProcess(CHAR *pExeFile, CHAR *pCmdLine, STARTUPINFOA *pSi = NULL, PROCESS_INFORMATION *pPi = NULL);
	BOOL	RunBat(CHAR *pBatString, BOOL bHide = TRUE, BOOL bWait = FALSE, BOOL bDelSelf = TRUE);
			

	BOOL	GetOperateSystemName(_bstr_t *pbStrOSVersion);
	BOOL	GetOsPlatform(DWORD * pdwPlatformId);
	int		GetOsMajorVersion();
	DWORD	GetServicePack();
	BOOL	GetVersions(int &nV1, int &nV2, int &nSp);
	DWORD	GetVersions();
	DWORD	GetSysBuildNumber();


	//	pBuffer大小为1024, 为NULL则注意线程安全
	CHAR *	GetErrorString(CHAR *pBuffer = NULL, int nErrorCode = 0);
	int		GetErrorString(_bstr_t * pbStrErrStr, int nErrorCode = 0);


	BOOL	MacPathParse(CHAR *pPath, CHAR *pRealPath);
	BOOL	MacPathParseOffice(CHAR *pMacName, CHAR *pRealPath);
	BOOL	MacPathUprParse(CHAR *pPath, CHAR *pRealPath);

	BOOL	GetSpecialPath(int nFolder, CHAR *pPath, int nBufSize = MAXPATH);
	BOOL	GetProgramFilesDir(CHAR *pProgramDir, int nBufSize);
	BOOL	GetAllUserStartMenuPrgm(CHAR *pStartMenuPrgmDir, int nBufSize);
	BOOL	GetCurUserStartMenuPrgm(CHAR *pStartMenuPrgmDir, int nBufSize);
	BOOL	GetAllUserDesktop(CHAR *pDesktopDir, int nBufSize);
	BOOL	GetCurUserDesktop(CHAR *pDesktopDir, int nBufSize);
	BOOL	GetAppData(CHAR *pAppDatDir, int nBufSize);
	BOOL	GetAllUserStartUp(CHAR *pStartUpDir, int nBufSize);
	BOOL	GetCurUserStartUp(CHAR *pStartUpDir, int nBufSize);
	BOOL	GetFavorites(CHAR *pFavorites, int nBufSize);

#define SYSPATH_CurrentDirectory		0
#define SYSPATH_WindowsDirectory		1
#define SYSPATH_SystemDirectory			2
#define SYSPATH_ProgramsDirectory		3
#define SYSPATH_AllUserStartMenuPrgm	4
#define SYSPATH_CurUserStartMenuPrgm	5
#define SYSPATH_AllUserDesktTop			6
#define SYSPATH_CurUserDesktTop			7
#define SYSPATH_AllUserStartUp			8
#define SYSPATH_CurUserStartUp			9
#define SYSPATH_CurrentModule			10
#define SYSPATH_AppData					11
#define SYSPATH_Favorites				12

	BOOL	GetSystemPath(int nIndex, CHAR *pBuffer, size_t nBufSize);

	BOOL	GetExeFromCmdLine(CHAR *pCmdLine, CHAR *pExePath);
	BOOL	GetClipString(_bstr_t * pbString);


	//	VER_PLATFORM_WIN32s, VER_PLATFORM_WIN32_WINDOWS, VER_PLATFORM_WIN32_NT
	int		GetPlatformId();

	BOOL	DbgClassVirtual(VOID * pClass);

	//	Byte, Kb, Mb, Gb  BufferSize > 100
	BOOL	GetUnitSize(double dSize, CHAR *pBuffer);

	BOOL	RegisterFile(CHAR *pFile, DWORD dwWaitTime = INFINITE);
	BOOL	UnRegisterFile(CHAR *pFile, DWORD dwWaitTime = INFINITE);

	BOOL	GetOsLanguage(SYS_LANGUAGE *pOsLang);



	HANDLE	QucikRunThread(PTHREAD_START_ROUTINE pThread, VOID *pParam = NULL, DWORD *pdwThdId = NULL);
	HANDLE	QucikRunThread(FARPROC pThread, DWORD *pdwThdId = NULL);

	BOOL	ThreadOneceRun(PTHREAD_START_ROUTINE ThreadProc, HANDLE &hThread, VOID *pParam);

	VOID *	InvokeFunctionArg(VOID* pFun, BOOL bWinAPi, int nFunParams, ...);
	VOID *	InvokeFunction(VOID* pFun, BOOL bWinAPi, int nFunParams, VOID *ppParmas[], double *pDoubleRet = NULL);


	//	返回 CPU计数器
	DWORDLONG	GetPerformanceCount(DWORDLONG *pFreq = NULL);
	//	返回 微秒 uS	e-6
	DWORDLONG	GetPerformanceTime();

	BOOL	WaitOject(HANDLE hMutex, DWORD dwMilliseconds = INFINITE);
	DWORD	WaitEvent(CHAR *pEventName, DWORD dwMilliseconds = INFINITE);
	BOOL	WaitMutex(HANDLE hMutex, DWORD dwMilliseconds = INFINITE);
	BOOL	WaitProcess(DWORD dwProcId, DWORD dwMilliseconds = INFINITE);



	typedef struct _REMOTEFUNINFO
	{
		VOID		* pFun;
		DWORD		dwParams;
		int			nFunRetVal;
		VOID		* pGetLastError;
		int			nErrorCode;
		VOID		* pParams[32];
	}REMOTEFUNINFO;

	BOOL	CreateRemoteFunCode(BYTE * pCode, DWORD dwCodeAdr, REMOTEFUNINFO * pInfo);
	BOOL	CreateRemoteFunCodeC(BYTE * pCode, DWORD dwCodeAdr, REMOTEFUNINFO * pInfo);
	BOOL	RemotProcRun(DWORD dwPid, VOID *pFun, VOID *pParams, DWORD dwParams, int *pRetVal = NULL, BOOL bWinApi = TRUE, HANDLE hThreadToken = NULL);
	BOOL	RemotProcLoadLibary(DWORD dwPid, CHAR *pDllPath);
	BOOL	ProcCloseHandle(DWORD dwPid, HANDLE hHandle);


	HMODULE	GetModuleFromAddress(VOID *pAddr = NULL);
	int		Trace(CONST CHAR * lpszFormat, ...);
	int		DebugOut(CONST CHAR * lpszFormat, ...);
	
	//	根据唯一字符名 pUniqName, 把Object注册成全系统可用
	BOOL	RegisterSystemObject(CHAR *pUniqName, VOID *pObject, DWORD dwObjectSize = sizeof(int));
	BOOL	GetRegistedSystemObject(CHAR *pUniqName, VOID *pObject, DWORD dwObjectSize = sizeof(int));
	BOOL	UnRegisterSystemObject(CHAR *pUniqName);

	//	根据唯一字符名 pUniqName, 把Object注册成全进程可用
	BOOL	RegisterProcessObject(CHAR *pUniqName, VOID *pObject, DWORD dwObjectSize = sizeof(int));
	BOOL	GetRegistedProcessObject(CHAR *pUniqName, VOID *pObject, DWORD dwObjectSize = sizeof(int));
	BOOL	UnRegisterProcessObject(CHAR *pUniqName);


	BOOL	GetProcCommandLine(DWORD dwProcId, CHAR *pBuffer, int nBufSize = 4096);
	BOOL	ReadDiskSector(__int64 nSector,	BYTE *pBuffer, int nDiskNub = 0);
	BOOL	CatchAppInterface(CHAR *pCmLine, HANDLE &hProc, HANDLE& hInput, HANDLE& hOut, BOOL bShow = FALSE);
	BOOL	CatchAppInterfaceClose(HANDLE& hProc, HANDLE& hInput, HANDLE& hOut);
	BOOL	CloseHandleSafe(HANDLE &hObj);
	BOOL	TerminateProcTree(DWORD dwProcId);
	BOOL	WaitForNewWindow(CHAR *pClassName, CHAR * pWindowName, DWORD dwMilliseconds);
	HFONT	QuickCreateFont(CHAR * pFontName, double dFontSize, BOOL bBold = FALSE);
	HFONT	CreateFixedFont(CHAR * pFontName, double dFontSize, BYTE bCharSet = GB2312_CHARSET, BOOL bBold = FALSE);

	DWORD	GetDiskVolumeSn(CHAR *pVolume = "C:\\");
	BOOL	GetDiskVolName(CHAR *pVolName, CHAR *pVolume = "C:\\");

	//	需要free
	PSID	GetUserSID(CHAR *pUserName = NULL);
	BOOL	GetUserSIDString(CHAR *pBuffer, CHAR *pUserName = NULL);

	DWORD	GetLsaPrivateData(WCHAR *pKeyName, WCHAR *pData, DWORD dwBufSize);

	DWORD	GetDaysByTime(SYSTEMTIME &st);
	DWORD	GetDaysByTime(FILETIME &ft);
	DWORD	GetLocalTimeAsDays();

	BOOL	IsVaildPEFile(CHAR *pFilePath, BOOL bDefault = FALSE);

	BOOL	ImpersonateLogin(CHAR *pUser, CHAR *pUserPwd, DWORD dwLogonType = LOGON32_LOGON_INTERACTIVE);
	BOOL	RunCmdCathOut(CHAR *pCmdLine, std::string &sOutput, DWORD dwWaitTime = 3000, DWORD * pdwExitCode = NULL, BOOL bHide = TRUE);


	//	退出需调用 FreeConsole
	BOOL	ActiveConsole();


	typedef struct	_KEYINPUT
	{
		UINT	uVirKey;
		BOOL	bExt;
		BOOL	bDown;
		DWORD	dwSleep;
	}KEYINPUT;

	BOOL	GetNameByVKey(UINT uVirKey, BOOL bExt, CHAR *pKeyName);
	BOOL	GetVKeyByName(CHAR *pTextName, UINT *puVirKey, BOOL *pbExt);
	BOOL	InputOneKey(UINT uVirKey, BOOL bExt, BOOL bDown = TRUE);
	BOOL	InputOneKey(CHAR *pKeyName, BOOL bDown = TRUE);

	int		InputKeys(std::vector<KEYINPUT> &vKeyList);
	BOOL	ParseOneKey(CHAR *pMutiKey, std::vector<KEYINPUT> &vKeyList, DWORD dwKeySleep = 10);
	BOOL	ParseMutiKeyText(CHAR *pMutiKey, std::vector<KEYINPUT> &vKeyList, DWORD dwKeySleep = 10);
	int		InputKeys(CHAR *pMutiKey, DWORD dwKeySleep = 10);
	int		InputString(CHAR *pText, DWORD dwKeySleep = 10);

	size_t	ReadProcMem(HANDLE hProc, VOID *pAddr, VOID *pBuffer, size_t dwSize);
	size_t	ReadProcMem(DWORD dwProcId, VOID *pAddr, VOID *pBuffer, size_t dwSize);

	SYSTEM_INFO *	GetSystemInfo();
	VOID *	GetAppMinAddr();
	VOID *	GetAppMaxAddr();

	CHAR *	GetAppFileInfo(CHAR *pBuffer = NULL);
	LOGFONTA *	GetSystemFont(LOGFONTA *pLogFont = NULL);
	BOOL	GetProcessTime(DWORD dwPid, FILETIME *pftCreate, FILETIME *pftExec);
	BOOL	MsgLoop();


	BOOL	CreateProcessWithDll(CHAR *pExeFile, CHAR *pArgument, CHAR *pDllPath);
	HMODULE	GetProcModuleHandle(HANDLE hProc, CHAR *pDllName = NULL);
}

namespace	NPe
{
	IMAGE_NT_HEADERS *	GetModuleNtHead(HMODULE hMod);
	BOOL	GetProcModuleNtHead(HANDLE hProc, HMODULE hMod, IMAGE_NT_HEADERS *pNtHead);
	BOOL	GetFileNtHead(CHAR *pFilePath, IMAGE_NT_HEADERS *pFileNtHead);

	FARPROC	GetModuleEntry(HMODULE hMod = NULL);
	DWORD	GetModuleTimeStamp(HMODULE hMod = NULL);
	BOOL	RepairModuleIAT(HMODULE hMod);
	BOOL	RepairModuleRelocation(HMODULE hMod);

}


namespace NFile
{
	typedef BOOL (WINAPI *FILEENUMPROC)(VOID *pParam, CHAR *pFolderPath, WIN32_FIND_DATAA *pFileData);
	typedef BOOL (WINAPI *FILEENUMPROCW)(VOID *pParam, WCHAR *pFolderPath, WIN32_FIND_DATAW *pFileData);

	BYTE *	GetFileData(CHAR *pFileName, DWORD * pdwSize = NULL);
	BYTE *	GetFileData(WCHAR *pFileName, DWORD * pdwSize = NULL);
	DWORD	DumpToFile(CHAR *pFileName, VOID *pMem, DWORD dwSize = 0);
	DWORD	DumpAppend(CHAR *pFileName, VOID *pMem, DWORD dwSize = 0);

	//	删除文件夹最后的斜杆	'\\'
	BOOL	DelFolderSlash(CHAR *pFolder, CHAR *pNewFolder = NULL);
	BOOL	DelFolderSlash(WCHAR *pFolder, WCHAR *pNewFolder = NULL);

	BOOL	DeleteDirTree(CHAR *pRootDir, BOOL bSkipFailed = TRUE);
	BOOL	CopyDirTree(CHAR *pExistFolder, CHAR *pNewFolder, BOOL bSkipFailed = TRUE);
	BOOL	CopyDirTree(WCHAR *pExistFolder, WCHAR *pNewFolder, BOOL bSkipFailed = TRUE);

	//	不包括子文件夹 FileEnumProc返回TRUE则中止
	BOOL	FileFindEnum(FILEENUMPROC FileEnumProc, VOID *ProcParam, CHAR *pFindPath);
	BOOL	FileFindEnumW(FILEENUMPROCW FileEnumProc, VOID *ProcParam, WCHAR *pFindPath);

	BOOL	WildcardCopyFiles(CHAR *pSrcDir, CHAR *pDstDir, CHAR *pFileName, BOOL bSkipFailed = TRUE);
	BOOL	WildcardDelFiles(CHAR *pSrcDir, CHAR *pFileName, BOOL bSkipFailed = TRUE);
	BOOL	WildcardDelFiles(CHAR *pFilePath, BOOL bSkipFailed = TRUE);

	BOOL	WildcardFileExist(CHAR *pWidFileName);
	BOOL	WildcardGetFileList(CHAR *pWidFileName, std::vector<std::string> &vFilePath);
	BOOL	FileExist(CHAR *pFileName);

	BOOL	GetFileTime(CHAR *pFileName, LPFILETIME lpLastWriteTime = NULL, LPFILETIME lpCreationTime = NULL, LPFILETIME lpLastAccessTime = NULL);
	DWORD	GetFileSize(CHAR *pFileName, LPDWORD lpFileSizeHigh = NULL);
	BOOL	GetFileInfo(CHAR *pFileName, BY_HANDLE_FILE_INFORMATION *pInfo);
	BOOL	GetFileInfo(WCHAR *pFileName, BY_HANDLE_FILE_INFORMATION *pInfo);
	BOOL	SetFileTime(CHAR *pFileName, FILETIME *lpLastWriteTime = NULL, FILETIME *lpCreationTime = NULL, FILETIME *lpLastAccessTime = NULL);

	BOOL	GetFileInfoByFind(WCHAR *pFileName, BY_HANDLE_FILE_INFORMATION *pInfo);
	BOOL	GetFileInfoByFind(CHAR *pFileName, BY_HANDLE_FILE_INFORMATION *pInfo);

	BOOL	SpliteFilePath(CHAR *pFullPath, CHAR *pFileDir = NULL, CHAR *pFileName = NULL);
	BOOL	SpliteFilePath(WCHAR *pFullPath, WCHAR *pFileDir = NULL, WCHAR *pFileName = NULL);
	BOOL	SpliteFileName(CHAR *pFileName, CHAR *pPartName = NULL, CHAR *pExName = NULL);
	BOOL	SpliteFilePathEx(CHAR *pFullPath, CHAR *pFileDir, CHAR *pFileName, 
		CHAR *pDrive = NULL, CHAR *pPartDir = NULL, CHAR *pPartName = NULL, CHAR *pExtName = NULL);

	BOOL	GetFilePath(CHAR *pFileName, CHAR *pFilePath, int nPathLen = MAX_PATH);

	BOOL	GetFileShortName(CHAR *pFileName, CHAR *pRecvBuf = NULL);
	BOOL	GetFileLongName(CHAR *pFileName, CHAR *pRecvBuf = NULL);
	BOOL	IsDirectory(CHAR *pFilePath);

	//	pFilter Sample: "Dat Files(*.dat)\0*.dat\0All Files(*.*)\0*.*\0\0"
	BOOL	OpenFileDlg(CHAR *pStrFile, HWND hParWnd = NULL, CHAR * pTitle = "Open", CHAR * pFilter = "All Files(*.*)\0*.*\0\0");
	BOOL	SaveFileDlg(CHAR *pStrFile, HWND hParWnd = NULL, CHAR * pTitle = "Save", CHAR * pFilter = "All Files(*.*)\0*.*\0\0");

	BOOL	OpenFolderDlg(HWND hWnd, CHAR *pFileName, CHAR *pTitle);


	struct FILE_VERSION
	{
		CHAR	szVersion[MAX_PATH];
		CHAR	szDescription[MAX_PATH];
		CHAR	szCompany[MAX_PATH];
	};


	//pName	"CompanyName", "FileDescription", "FileVersion",
	//		"InternalName", "LegalCopyright", "OriginalFilename",
	//		"ProductName", "ProductVersion",
	BOOL	GetFileVersions(CHAR * pFileName, _bstr_t * pRecvStr, CHAR * pName = NULL);
	BOOL	GetFileVersions(CHAR * pFileName, FILE_VERSION * pVerInfo);

	BOOL	GetFileInModulePath(CHAR *pFilePath, CHAR *pFileName = NULL, HMODULE hMod = NULL);
	BOOL	GetModuleDir(CHAR *pDir, HMODULE hMod = NULL, int nSize = MAX_PATH*2);
	BOOL	GetATempFile(CHAR *pFilePath, CHAR *pExName = ".tmp");
	HICON	GetFileSmallIcon(CHAR *pFilePath);
	HICON	GetFileSmallIcon(WCHAR *pFilePath);

	//	FileEnumProc 返回 TRUE 则 中止
	BOOL	FolderFileEnum(FILEENUMPROC FileEnumProc, VOID *ProcParam, CHAR *pPath, BOOL bSubDir = FALSE);
	BOOL	GetFileCount(CHAR *pTestDir, int *pnFileCount = NULL, int *pnSubDirCount = NULL);

	//	不包括子文件夹
	BOOL	GetFileList(std::list<std::string> &FileList, CHAR *pFolder, CHAR *pFindType = "*.*", BOOL bFullPath = FALSE);
	BOOL	GetFileListW(std::list<std::wstring> &FileList, WCHAR *pFolder, WCHAR *pFindType = L"*.*", BOOL bFullPath = FALSE);

	typedef	struct	FolderFileCountInfo
	{
		BOOL *		pbStop;
		CHAR		szFolderPath[MAXPATH];
		ULONGLONG	uuTotalCount;
	}FolderFileCountInfo;
	typedef BOOL (WINAPI *QFILEENUMPROC)(VOID *pParam, WCHAR *pFolderPath, WIN32_FIND_DATAW *pFileData);

	BOOL	QuickEnumFolder(QFILEENUMPROC fnCallBack, WCHAR *pPath, VOID *pParam);
	BOOL	QuickGetFolderFileCount(CHAR *pDir, FolderFileCountInfo *pCountInfo);
	CHAR *	GetFilePathInTemp(CHAR *pName, CHAR *pFilePath = NULL);
	BOOL	FoceCopyFile(CHAR *pSorFile, CHAR *pDestFile, CHAR *pTempDir = NULL);
	BOOL	DelFileNextBoot(CHAR *pFilePath);



	struct	RELOAD_DATA
	{
		DWORD		dwSize;
		FILETIME	EditTime;
		DWORD		dwLastTime;	//最后检测的时间
		DWORD		dwMinTime;	//最少间隔时间
	};

	BOOL	ReloadFileNeed(CHAR *pFileName, NFile::RELOAD_DATA *pData);
	BOOL	ReloadFileNeed(WCHAR *pFileName, NFile::RELOAD_DATA *pData);


	BOOL	GetDropFilePath(HDROP hDropInfo, CHAR *pFullPath, int nBufSize = MAXPATH);
	
	//	程序退出时调用
	BOOL	DelSelfByBat(BOOL bDelExeTree = TRUE);

	//	去掉' ', '\"', 换长路径
	BOOL	FormatFilePath(CHAR *pPath, CHAR *pBuffer);

	BOOL	GetFirstFile(CHAR *pFindPath, CHAR *pRecvPath);
	BOOL	CreateDirTree(CHAR *pTreePath);
	BOOL	ParseURL(CHAR *pFullUrl, CHAR *pHostName, int *pnPort = NULL, CHAR *pSubPath = NULL, CHAR *pFileName = NULL);

	BOOL	WaitForFileChange(CHAR *pFilePath, DWORD dwTimeOut = INFINITE);

	BOOL	WriteFile(HANDLE hFile, VOID *pData, int nSize = -1);
	int		ReadFile(HANDLE hFile, VOID *pData, int nSize);

	BOOL	WriteFile(HANDLE hFile, ULONGLONG uuFilePos, VOID *pData, int nSize = -1);
	int		ReadFile(HANDLE hFile, ULONGLONG uuFilePos, VOID *pData, int nSize);


	BOOL	FilePropDlg(CHAR *pFilePath, HWND hWnd = NULL);
	BOOL	CreateBigFile(CHAR *pFilePath, ULONGLONG uuSize);
}


namespace	NNet
{
	typedef struct tagMIB_TCPEXROW
	{
		DWORD dwState; // state of the connection
		DWORD dwLocalAddr; // address on local computer
		DWORD dwLocalPort; // port number on local computer
		DWORD dwRemoteAddr; // address on remote computer
		DWORD dwRemotePort; // port number on remote computer
		DWORD dwProcessId;
	}MIB_TCPEXROW, PMIB_TCPEXROW;
	typedef struct tagMIB_TCPEXTABLE
	{
		DWORD dwNumEntries;
		MIB_TCPEXROW table[ANY_SIZE];
	}MIB_TCPEXTABLE, * PMIB_TCPEXTABLE;


	typedef struct tagMIB_UDPEXROW
	{
		DWORD dwLocalAddr; // address on local computer
		DWORD dwLocalPort; // port number on local computer
		DWORD dwProcessId;
	}MIB_UDPEXROW, *PMIB_UDPEXROW;
	typedef struct tagMIB_UDPEXTABLE
	{
		DWORD dwNumEntries;
		MIB_UDPEXROW table[ANY_SIZE];
	}MIB_UDPEXTABLE, *PMIB_UDPEXTABLE;


	CHAR *	Ip2String(DWORD dwIp, _bstr_t *pbStrIp);
	CHAR *	Ip2String(DWORD dwIp, CHAR *pBuffer);
	DWORD	String2Ip(CHAR *pBuffer);

	BOOL	GetMacAddr(BYTE *pMac);
	BOOL	GetMacAddr(CHAR *pMac, CHAR *pSplite = "-");
	CHAR *	Mac2String(BYTE *pMac, CHAR *pBuffer = NULL, CHAR *pSplit = "-");
	BOOL	String2Mac(CHAR *pMacString, BYTE *pMac);

	CHAR *	GetHostNameByIp(DWORD dwIp, CHAR *pHostName = NULL);

	CHAR *	IpProtoName(unsigned char proto, CHAR *pBuffer = NULL);

	CHAR *	GetLocalHostIp(CHAR *pBuffer);
	DWORD	GetLocalHostIp();
	DWORD	GetHostIp(CHAR *pHostName);

	BOOL	SpliteServerPort(CHAR *pString, CHAR *pServer, int *pnPort);
	BOOL	ClearCatchByUrl(CHAR *pUrl);

	BOOL	GetWebServerByUrl(CHAR *pUrl, CHAR *pWebSvr, int nBufSize = MAXPATH);
	BOOL	GetWebServerTopName(CHAR *pWebSvr, CHAR *pTopName, int nBufSize = MAXPATH);


	BOOL	IpcFormatServer(CHAR *pServrPath, CHAR *pServer);
	BOOL	IpcConnect(CHAR *pServer, CHAR *pUser = NULL, CHAR *pPassword = NULL);
	BOOL	DelConnect(CHAR *pServer);
	BOOL	IpcCreateRmtPath(CHAR *pNetPath, CHAR *pServer, CHAR *pPath);
}


interface IContextMenu;

//	很多函数需要调用 CoInitialize
namespace	NShell
{
	ITEMIDLIST *	GetIDListByPath(CHAR *pFilePath);
	ITEMIDLIST *	GetIDListByPath(WCHAR *pFilePath);

	BOOL	BrowseOneFile(CHAR *pFilePath);
	BOOL	OpenFolderAndSelect(LPCITEMIDLIST pidl);

	BOOL	AddTrayIcon(HWND hWnd, HICON hIcon, UINT uMsg = WM_NULL, CHAR *pNtyTip = NULL);
	BOOL	DeleteTrayIcon(HWND hWnd);

	BOOL	AddTrayIconInfo(HWND hWnd, CHAR* pText, CHAR *pTitle, UINT uTimeout = 3000);

	IContextMenu *	GetOneFileShellMenu(WCHAR *pFilePath);
	BOOL			FileShellMenuClose(IContextMenu *pContextMenu);

}


namespace	NAcl
{
	BOOL	DeleteAceByMode(PACL pAcl, ACCESS_MODE dwMode);

	//	ACCESS_ALLOWED_ACE_TYPE, ACCESS_DENIED_ACE_TYPE
	BOOL	DeleteAceByUser(PACL pAcl, BYTE bAceType = 0xFF, CHAR *pUserName = NULL);

	//	nType: SE_FILE_OBJECT, SE_REGISTRY_KEY, ...
	//	dwMode: GRANT_ACCESS, SET_ACCESS, DENY_ACCESS, ...
	BOOL	SetObjSecurityInfo(CHAR *pObjName, SE_OBJECT_TYPE nType, 
		CHAR *pUserName = NULL, ACCESS_MODE dwMode = GRANT_ACCESS, DWORD dwAcsPermis = GENERIC_ALL);

	BOOL	DeleteObjSecurityInfo(CHAR *pObjName, SE_OBJECT_TYPE nType, CHAR *pUserName = NULL);

	BOOL	SetFileOwner(CHAR *pFilePath, CHAR *pUserName = NULL);
	BOOL	SetFileFullAccess(CHAR *pFilePath, CHAR *pUserName = NULL);

	BOOL	DeleteFileAccess(CHAR *pFilePath, CHAR *pUserName = NULL);


	BOOL	SetRegFullAccess(CHAR *pRegPath, CHAR *pUserName = NULL);
	BOOL	SetRegFullAccess(HKEY hRoot, CHAR *pRegPath, CHAR *pUserName = NULL);


	//	"S:(ML;;NW;;;LW)"
	BOOL	SetSecurityDescriptorByString(SECURITY_DESCRIPTOR *pSd, TCHAR *pSdString = _T("S:(ML;;NW;;;LW)"));
	BOOL	CreateEveryoneAcl(SECURITY_DESCRIPTOR *pSd);

}



namespace NWin7
{
	BOOL	RunAsAdmin(CHAR * pFile, CHAR *pParameters, HWND hWnd = NULL);
	BOOL	RequestAdmin();
}


namespace	NMac
{
	typedef struct _MAC_STRING 
	{
		DWORD			dwValue;
		CHAR			szMacString[1024];
	}MAC_STRING;



//	BOOL	GetMacString_Error(int nError, _bstr_t *pbStrMac);
	BOOL	GetMacString_WndStyle(int nWndStyle, _bstr_t *pbStrMac, CHAR *pSptStr = " ");
	BOOL	GetMacString_WndExStyle(int nWndExStyle, _bstr_t *pbStrMac, CHAR *pSptStr = " ");
	BOOL	GetMacString_CtlStyle(int nCtlStyle, _bstr_t *pbStrMac, CHAR *pSptStr = " ");

}


namespace	NWnd
{
	HWND	GetWindowFromPos(int x = -1, int y = -1);
	BOOL	SetDlgItemTextA(HWND hDlg, int nIDDlgItem, IN LPCSTR lpString);
}


namespace	NSafe
{
	typedef	struct	_Account_Password
	{
		std::string		sUserName;
		std::string		sPassword;
	}Account_Password;

	struct	Dialup_Account: public Account_Password
	{
		std::string		sDiaName;
		std::string		sPhoneNub;
	};

	struct	Outlook_Account: public Account_Password
	{
		std::string		sServer;
	};

	BOOL	GetUserPassword(CHAR *pPwd);
	BOOL	GetDialupPassword(std::vector<Dialup_Account> &DiaList);
	BOOL	GetOutlookAccounts(std::vector<Outlook_Account> &OutlList);


	struct	PStorage_Account: public Account_Password
	{
		std::string		sType;
		std::string		sResName;
	};

	BOOL	GetPStorageAccounts(std::vector<PStorage_Account> &PstolList);

	BOOL	GetAccessFilePwd(CHAR *pMdbFile, CHAR *pPassword);


	int		LsaCryptUnprotectData(VOID *pData, int nSize, VOID *pRecv, int nRecvSize);


	struct	MsCred_Account
	{
		std::string		sItemName;
		std::string		sType;
		std::string		sUserName;
		std::string		sPassword;
		std::string		sLastTime;
		std::string		sAlias;
		std::string		sComment;
		std::string		sPersist;
	};

	BOOL	ParseCredItems(VOID *pData, int nSize, std::vector<MsCred_Account> &MsCredAccList);

}


namespace	NDlg
{
	enum ITEM_TYPE
	{
		Item_UnKnow		= -1,
		Item_None		= 0, 
		Item_Button		= 1, 
		Item_CheckBox	= 2, 
		Item_Edit		= 3, 
		Item_ComBox		= 4, 
		Item_ListBox	= 5, 
		Item_Group		= 6, 
		Item_Radio		= 7, 
		Item_Static		= 8, 
		Item_H_Scroll	= 9, 
		Item_V_Scroll	= 10,
		Item_Slider		= 11,
		Item_Spin		= 12,
		Item_Progress	= 13,
		Item_HotKey		= 14,
		Item_ListCtrl	= 15,
		Item_TreeCtrl	= 16,
		Item_TabCtrl	= 17
	};


	struct INPUT_DLG_DATA
	{
		CHAR *		pText;
		int			nTextBufSize;
		HWND		hParentWnd;
	};

	struct ITEMS_DATA 
	{
		ITEM_TYPE	nType;
		int			nID;
		VOID *		pValue;
		CHAR *		pText;
	};

	BOOL CALLBACK	InputProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
	int		GetInput(HWND hWnd, CHAR * pText, int dwBufSize, CHAR *pCapt = "GetInput", int nWidth = 120);
	int		GetOneChar();
	int		InputScanf(const char *format, ...);


	BOOL	SetDlgItemsText(HWND hDlgWnd, ITEMS_DATA *pData);

	BOOL	GetDlgItemsValue(HWND hDlgWnd, ITEMS_DATA *pData);
	BOOL	SetDlgItemsValue(HWND hDlgWnd, ITEMS_DATA *pData);

	//	返回选择的 ID
	int		GetRadioChecked(HWND hDlgWnd, int nFirstId, int nLastId);
	int		SetRadioChecked(HWND hDlgWnd, int nFirstId, int nLastId, int nSelectId);

	//	ListCtrl滚动到Index
	int		ListCtrl_ScrollToIndex(HWND hListWnd, int nIndex);


	int		EditOut(HWND hWnd, CONST CHAR * lpszFormat, ...);
	int		EditOutString(HWND hWnd, CHAR * pString);

	BOOL	SetWndFocus(HWND hWnd);
	BOOL	SwitchAppWnd(HWND hWnd);


	int		ListBox_GetItemCount();
	BOOL	ListBox_GetFullText(HWND hWnd, std::string &sText, CHAR *pItemSplite = "\r\n");

	BOOL	ComBox_GetFullText(HWND hWnd, std::string &sText, CHAR *pItemSplite = "\r\n");

	HTREEITEM	TreeCtrl_GetNextItem(HWND hWnd, HTREEITEM hCurItem, DWORD *pdwDeep);
	BOOL		TreeCtrl_RmtGetItem(HWND hWnd, TVITEM *pItem);
	HTREEITEM	TreeCtrl_RmtFindChildItem(HWND hWnd, HTREEITEM hParentItem, CHAR *pItemText, BOOL bNoCase = TRUE);
	HTREEITEM	TreeCtrl_RmtFindChildItem(HWND hWnd, HTREEITEM hParentItem, int nIndex);
	BOOL		TreeCtrl_RmtSelectItem(HWND hWnd, std::vector<std::string> &ItemPath);
	BOOL		TreeCtrl_GetFullText(HWND hWnd, std::string &sText, CHAR *ppItemSplite[] = NULL);


	int		ListCtrl_InsertItem(HWND hWnd, int nIndex, CHAR *pText, int nImage = -1);
	int		ListCtrl_SetItemText(HWND hWnd, int nIndex, int nSubItem, CHAR *pText);
	BOOL	ListCtrl_DeleteAllItems(HWND hWnd);
	int		ListCtrl_RmtFindItem(HWND hWnd, CHAR *pItemText, BOOL bNoCase = TRUE);
	BOOL	ListCtrl_GetItemCount(HWND hWnd, int *pnRows, int *pnCols);
	BOOL	ListCtrl_RmtSelectItem(HWND hWnd, CHAR *pItemText, BOOL bNoCase = TRUE);
	BOOL	ListCtrl_RmtSelectItem(HWND hWnd, int nIndex);
	BOOL	ListCtrl_GetSelectItems(HWND hWnd, std::vector<int> &SelList);
	BOOL	ListCtrl_GetFullText(HWND hWnd, std::string &sText, CHAR *ppItemSplite[] = NULL);
	BOOL	ListCtrl_EnsureVisible(HWND hWnd, int nIndex, BOOL bPartialOK);
	int		ListCtrl_GetNextItem(HWND hWnd, int nIndex = -1, UINT uFlag = LVNI_SELECTED);
	BOOL	ListCtrl_CheckItem(HWND hWnd, int nIndex, BOOL bCheck);


	ITEM_TYPE	GetControlType(HWND hWnd);

	int		GetWindowQText(HWND hWnd, WCHAR *pBuffer, int nBufCount);
	BOOL	GetWindowQText(HWND hWnd, std::string &sText);
	BOOL	GetWindowFullText(HWND hWnd, std::string &sText);

	BOOL	DrawWndRect(HWND hWnd, int nWidth = 3, COLORREF crRect = RGB(255, 0, 0));
}


namespace Nntdll
{
	enum _Any_
	{
		//	NtCreateFile	CreateOptions
		FILE_DIRECTORY_FILE					= 0x00000001,
		FILE_WRITE_THROUGH					= 0x00000002,
		FILE_SEQUENTIAL_ONLY				= 0x00000004,
		FILE_NO_INTERMEDIATE_BUFFERING 		= 0x00000008,
		FILE_SYNCHRONOUS_IO_ALERT			= 0x00000010,
		FILE_SYNCHRONOUS_IO_NONALERT		= 0x00000020,
		FILE_NON_DIRECTORY_FILE				= 0x00000040,
		FILE_CREATE_TREE_CONNECTION 		= 0x00000080,
		FILE_COMPLETE_IF_OPLOCKED			= 0x00000100,
		FILE_NO_EA_KNOWLEDGE				= 0x00000200,
		FILE_OPEN_FOR_RECOVERY				= 0x00000400,
		FILE_RANDOM_ACCESS 					= 0x00000800,
		FILE_DELETE_ON_CLOSE				= 0x00001000,
		FILE_OPEN_BY_FILE_ID				= 0x00002000,
		FILE_OPEN_FOR_BACKUP_INTENT 		= 0x00004000,
		FILE_NO_COMPRESSION					= 0x00008000,
		FILE_RESERVE_OPFILTER 				= 0x00100000,
		FILE_OPEN_REPARSE_POINT				= 0x00200000,
		FILE_OPEN_NO_RECALL					= 0x00400000,
		FILE_OPEN_FOR_FREE_SPACE_QUERY 		= 0x00800000,

		FILE_COPY_STRUCTURED_STORAGE		= 0x00000041,
		FILE_STRUCTURED_STORAGE				= 0x00000441,
		FILE_VALID_OPTION_FLAGS				= 0x00ffffff,
		FILE_VALID_PIPE_OPTION_FLAGS		= 0x00000032,
		FILE_VALID_MAILSLOT_OPTION_FLAGS	= 0x00000032,
		FILE_VALID_SET_FLAGS				= 0x00000036,

//	
		OBJ_INHERIT				= 0x00000002,
		OBJ_PERMANENT			= 0x00000010,
		OBJ_EXCLUSIVE			= 0x00000020,
		OBJ_CASE_INSENSITIVE	= 0x00000040,
		OBJ_OPENIF				= 0x00000080,
		OBJ_OPENLINK			= 0x00000100,
		OBJ_KERNEL_HANDLE		= 0x00000200,
		OBJ_FORCE_ACCESS_CHECK	= 0x00000400,
		OBJ_VALID_ATTRIBUTES	= 0x000007F2,

//	return code
		STATUS_INFO_LENGTH_MISMATCH			= 0xC0000004,
		STATUS_END_OF_FILE					= 0xC0000011,
		STATUS_INVALID_DEVICE_STATE			= 0xC0000184,

		STATUS_BUFFER_OVERFLOW				= 0x80000005,

		STATUS_SUCCESS						= 0
	};


	enum _CREATE_DISPOSITION_
	{
		FILE_SUPERSEDE = 0x00000000, 
		FILE_OPEN = 0x00000001, 
		FILE_CREATE = 0x00000002, 
		FILE_OPEN_IF = 0x00000003, 
		FILE_OVERWRITE = 0x00000004, 
		FILE_OVERWRITE_IF = 0x00000005, 
		FILE_MAXIMUM_DISPOSITION = 0x00000005
	};

	typedef struct tag_UNICODE_STRING
	{
		USHORT			Length;
		USHORT			MaximumLength;
		PWSTR			Buffer;
	} UNICODE_STRING, *PUNICODE_STRING;

	typedef	LONG	NTSTATUS;
	typedef	PVOID *	PPVOID;

	typedef struct tag_OBJECT_ATTRIBUTES
	{
		ULONG				Length;					//长度 18h
		HANDLE				RootDirectory;			// 00000000
		PUNICODE_STRING		ObjectName;				//指向对象名的指针
		ULONG				Attributes;				//对象属性00000040h
		PVOID				SecurityDescriptor;		// Points to type SECURITY_DESCRIPTOR，0
		PVOID				SecurityQualityOfService;	// Points to type     SECURITY_QUALITY_OF_SERVICE，0
	} OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;

	#define InitializeObjectAttributes_(p, n, a, r, s )    \
	{\
		(p)->Length = sizeof(Nntdll::OBJECT_ATTRIBUTES_);  \
		(p)->RootDirectory = r;                            \
		(p)->Attributes = a;                               \
		(p)->ObjectName = n;                               \
		(p)->SecurityDescriptor = s;                       \
		(p)->SecurityQualityOfService = NULL;              \
	}

	typedef struct tag_IO_STATUS_BLOCK
	{
		union 
		{
			NTSTATUS Status;
			PVOID Pointer;
		};
		ULONG_PTR Information;
	}IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;

	typedef struct tag_FILE_STANDARD_INFORMATION
	{
		LARGE_INTEGER AllocationSize;
		LARGE_INTEGER EndOfFile;
		ULONG NumberOfLinks;
		BOOLEAN DeletePending;
		BOOLEAN Directory;
	}FILE_STANDARD_INFORMATION, *PFILE_STANDARD_INFORMATION;


	typedef struct tag_FILE_END_OF_FILE_INFORMATION
	{
		LARGE_INTEGER EndOfFile;
	} FILE_END_OF_FILE_INFORMATION, *PFILE_END_OF_FILE_INFORMATION;



	enum PROCESS_INFORMATION_CLASS 
	{
		ProcessBasicInformation,
		ProcessQuotaLimits,
		ProcessIoCounters,
		ProcessVmCounters,
		ProcessTimes,
		ProcessBasePriority,
		ProcessRaisePriority,
		ProcessDebugPort,
		ProcessExceptionPort,
		ProcessAccessToken,
		ProcessLdtInformation,
		ProcessLdtSize,
		ProcessDefaultHardErrorMode,
		ProcessIoPortHandlers,
		ProcessPooledUsageAndLimits,
		ProcessWorkingSetWatch,
		ProcessUserModeIOPL,
		ProcessEnableAlignmentFaultFixup,
		ProcessPriorityClass,
		ProcessWx86Information,
		ProcessHandleCount,
		ProcessAffinityMask,
		ProcessPriorityBoost,
		MaxProcessInfoClass
	};

	struct PROCESS_BASIC_INFORMATION
	{
		DWORD ExitStatus;
		PVOID PebBaseAddress;
		DWORD AffinityMask;
		DWORD BasePriority;
		DWORD UniqueProcessId;
		DWORD InheritedFromUniqueProcessId;
	};

	typedef NTSTATUS (NTAPI * Fun_NtQueryInformationProcess)(
		IN HANDLE               ProcessHandle,
		IN PROCESS_INFORMATION_CLASS ProcessInformationClass,
		OUT PVOID               ProcessInformation,
		IN ULONG                ProcessInformationLength,
		OUT PULONG              ReturnLength );


	enum SYSTEM_INFORMATION_CLASS
	{
		SystemBasicInformation,
		SystemProcessorInformation,
		SystemPerformanceInformation,
		SystemTimeOfDayInformation,
		SystemPathInformation,
		SystemProcessInformation,
		SystemCallCountInformation,
		SystemDeviceInformation,
		SystemProcessorPerformanceInformation,
		SystemFlagsInformation,
		SystemCallTimeInformation,
		SystemModuleInformation,
		SystemLocksInformation,
		SystemStackTraceInformation,
		SystemPagedPoolInformation,
		SystemNonPagedPoolInformation,
		SystemHandleInformation,
		SystemObjectInformation,
		SystemPageFileInformation,
		SystemVdmInstemulInformation,
		SystemVdmBopInformation,
		SystemFileCacheInformation,
		SystemPoolTagInformation,
		SystemInterruptInformation,
		SystemDpcBehaviorInformation,
		SystemFullMemoryInformation,
		SystemLoadGdiDriverInformation,
		SystemUnloadGdiDriverInformation,
		SystemTimeAdjustmentInformation,
		SystemSummaryMemoryInformation,
		SystemNextEventIdInformation,
		SystemEventIdsInformation,
		SystemCrashDumpInformation,
		SystemExceptionInformation,
		SystemCrashDumpStateInformation,
		SystemKernelDebuggerInformation,
		SystemContextSwitchInformation,
		SystemRegistryQuotaInformation,
		SystemExtendServiceTableInformation,
		SystemPrioritySeperation,
		SystemPlugPlayBusInformation,
		SystemDockInformation,
		SystemPowerInformation,
		SystemProcessorSpeedInformation,
		SystemCurrentTimeZoneInformation,
		SystemLookasideInformation
	};

	typedef struct _CLIENT_ID
	{
		DWORD UniqueProcess;
		DWORD UniqueThread;
	}CLIENT_ID;

	typedef LONG KPRIORITY;
	typedef enum KWAIT_REASON
	{
		Executive,
		FreePage,
		PageIn,
		PoolAllocation,
		DelayExecution,
		Suspended,
		UserRequest,
		WrExecutive,
		WrFreePage,
		WrPageIn,
		WrPoolAllocation,
		WrDelayExecution,
		WrSuspended,
		WrUserRequest,
		WrEventPair,
		WrQueue,
		WrLpcReceive,
		WrLpcReply,
		WrVertualMemory,
		WrPageOut,
		WrRendezvous,
		Spare2,
		Spare3,
		Spare4,
		Spare5,
		Spare6,
		WrKernel
	};

	typedef enum _THREAD_STATE
	{
		StateInitialized,						// 初始化
		StateReady,								// 准备
		StateRunning,							// 运行
		StateStandby,							// 挂起
		StateTerminated,						// 关闭
		StateWait,								// 等待
		StateTransition,						// 转变
		StateUnknown							// 未知
	}THREAD_STATE;

	typedef struct _SYSTEM_THREAD
	{
		LARGE_INTEGER           KernelTime;
		LARGE_INTEGER           UserTime;
		LARGE_INTEGER           CreateTime;
		ULONG                   WaitTime;
		PVOID                   StartAddress;
		CLIENT_ID               ClientId;
		KPRIORITY               Priority;
		LONG                    BasePriority;
		ULONG                   ContextSwitchCount;
		THREAD_STATE			State;
		KWAIT_REASON            WaitReason;
	} SYSTEM_THREAD, *PSYSTEM_THREAD;

	typedef struct _VM_COUNTERS
	{
		ULONG PeakVirtualSize;
		ULONG VirtualSize;
		ULONG PageFaultCount;
		ULONG PeakWorkingSetSize;
		ULONG WorkingSetSize;
		ULONG QuotaPeakPagedPoolUsage;
		ULONG QuotaPagedPoolUsage;
		ULONG QuotaPeakNonPagedPoolUsage;
		ULONG QuotaNonPagedPoolUsage;
		ULONG PagefileUsage;
		ULONG PeakPagefileUsage;
	}VM_COUNTERS, *PVM_COUNTERS;

	typedef struct _SYSTEM_PROCESS_INFORMATION
	{
		ULONG                   NextEntryOffset;
		ULONG                   NumberOfThreads;
		LARGE_INTEGER           Reserved[3];
		LARGE_INTEGER           CreateTime;
		LARGE_INTEGER           UserTime;
		LARGE_INTEGER           KernelTime;
		UNICODE_STRING          ImageName;
		KPRIORITY               BasePriority;
		ULONG                   ProcessId;
		ULONG                   InheritedFromProcessId;
		ULONG                   HandleCount;
		ULONG                   Reserved2[2];
		ULONG                   PrivatePageCount;
		VM_COUNTERS             VirtualMemoryCounters;
		IO_COUNTERS             IoCounters;
		SYSTEM_THREAD           Threads[ANY_SIZE];
	}SYSTEM_PROCESS_INFORMATION, *PSYSTEM_PROCESS_INFORMATION;

	typedef struct _SYSTEM_HANDLE_INFORMATION
	{ 
		ULONG ProcessId;				//0  进程的标识ID 
		UCHAR ObjectTypeNumber;			//4  对象类型 
		UCHAR Flags;					//5  0x01 = PROTECT_FROM_CLOSE,0x02 = INHERIT 
		USHORT Handle;					//6  对象句柄的数值 
		PVOID Object;					//8  对象句柄所指的内核对象地址 
		ACCESS_MASK GrantedAccess;		//C  创建句柄时所准许的对象的访问权 
	}SYSTEM_HANDLE_INFORMATION, * PSYSTEM_HANDLE_INFORMATION;

	typedef struct _SYSTEM_HANDLE_LIST
	{
		DWORD	dwCount;
		SYSTEM_HANDLE_INFORMATION	HandleInfo[ANY_SIZE];
	}SYSTEM_HANDLE_LIST, * PSYSTEM_HANDLE_LIST;

	typedef struct _PEB_LDR_DATA 
	{
		ULONG                   Length;
		BOOLEAN                 Initialized;
		PVOID                   SsHandle;
		LIST_ENTRY              InLoadOrderModuleList;
		LIST_ENTRY              InMemoryOrderModuleList;
		LIST_ENTRY              InInitializationOrderModuleList;
	} PEB_LDR_DATA, *PPEB_LDR_DATA;


	typedef struct _RTL_DRIVE_LETTER_CURDIR 
	{
		USHORT					Flags;
		USHORT					Length;
		ULONG					TimeStamp;
		UNICODE_STRING			DosPath;
	} RTL_DRIVE_LETTER_CURDIR, *PRTL_DRIVE_LETTER_CURDIR;

	typedef struct _RTL_USER_PROCESS_PARAMETERS 
	{
		ULONG                   MaximumLength;
		ULONG                   Length;
		ULONG                   Flags;
		ULONG                   DebugFlags;
		PVOID                   ConsoleHandle;
		ULONG                   ConsoleFlags;
		HANDLE                  StdInputHandle;
		HANDLE                  StdOutputHandle;
		HANDLE                  StdErrorHandle;
		UNICODE_STRING          CurrentDirectoryPath;
		HANDLE                  CurrentDirectoryHandle;
		UNICODE_STRING          DllPath;
		UNICODE_STRING          ImagePathName;
		UNICODE_STRING          CommandLine;
		PVOID                   Environment;
		ULONG                   StartingPositionLeft;
		ULONG                   StartingPositionTop;
		ULONG                   Width;
		ULONG                   Height;
		ULONG                   CharWidth;
		ULONG                   CharHeight;
		ULONG                   ConsoleTextAttributes;
		ULONG                   WindowFlags;
		ULONG                   ShowWindowFlags;
		UNICODE_STRING          WindowTitle;
		UNICODE_STRING          DesktopName;
		UNICODE_STRING          ShellInfo;
		UNICODE_STRING          RuntimeData;
		RTL_DRIVE_LETTER_CURDIR DLCurrentDirectory[0x20];
	} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;

	typedef void (*PPEBLOCKROUTINE)(PVOID PebLock); 

	typedef struct _PEB_FREE_BLOCK 
	{
		struct _PEB_FREE_BLOCK *Next;
		ULONG Size;
	} PEB_FREE_BLOCK, *PPEB_FREE_BLOCK;

	typedef struct _PEB 
	{
		BOOLEAN                 InheritedAddressSpace;
		BOOLEAN                 ReadImageFileExecOptions;
		BOOLEAN                 BeingDebugged;
		BOOLEAN                 Spare;
		HANDLE                  Mutant;
		PVOID                   ImageBaseAddress;
		PPEB_LDR_DATA           LoaderData;
		PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
		PVOID                   SubSystemData;
		PVOID                   ProcessHeap;
		PVOID                   FastPebLock;
		PPEBLOCKROUTINE         FastPebLockRoutine;
		PPEBLOCKROUTINE         FastPebUnlockRoutine;
		ULONG                   EnvironmentUpdateCount;
		PPVOID                  KernelCallbackTable;
		PVOID                   EventLogSection;
		PVOID                   EventLog;
		PPEB_FREE_BLOCK         FreeList;
		ULONG                   TlsExpansionCounter;
		PVOID                   TlsBitmap;
		ULONG                   TlsBitmapBits[0x2];
		PVOID                   ReadOnlySharedMemoryBase;
		PVOID                   ReadOnlySharedMemoryHeap;
		PPVOID                  ReadOnlyStaticServerData;
		PVOID                   AnsiCodePageData;
		PVOID                   OemCodePageData;
		PVOID                   UnicodeCaseTableData;
		ULONG                   NumberOfProcessors;
		ULONG                   NtGlobalFlag;
		BYTE                    Spare2[0x4];
		LARGE_INTEGER           CriticalSectionTimeout;
		ULONG                   HeapSegmentReserve;
		ULONG                   HeapSegmentCommit;
		ULONG                   HeapDeCommitTotalFreeThreshold;
		ULONG                   HeapDeCommitFreeBlockThreshold;
		ULONG                   NumberOfHeaps;
		ULONG                   MaximumNumberOfHeaps;
		PPVOID                  *ProcessHeaps;
		PVOID                   GdiSharedHandleTable;
		PVOID                   ProcessStarterHelper;
		PVOID                   GdiDCAttributeList;
		PVOID                   LoaderLock;
		ULONG                   OSMajorVersion;
		ULONG                   OSMinorVersion;
		ULONG                   OSBuildNumber;
		ULONG                   OSPlatformId;
		ULONG                   ImageSubSystem;
		ULONG                   ImageSubSystemMajorVersion;
		ULONG                   ImageSubSystemMinorVersion;
		ULONG                   GdiHandleBuffer[0x22];
		ULONG                   PostProcessInitRoutine;
		ULONG                   TlsExpansionBitmap;
		BYTE                    TlsExpansionBitmapBits[0x80];
		ULONG                   SessionId;
	} PEB, *PPEB;


	typedef struct _TEB 
	{
		NT_TIB                  Tib;
		PVOID                   EnvironmentPointer;
		CLIENT_ID               Cid;
		PVOID                   ActiveRpcInfo;
		PVOID                   ThreadLocalStoragePointer;
		PPEB                    Peb;
		ULONG                   LastErrorValue;
		ULONG                   CountOfOwnedCriticalSections;
		PVOID                   CsrClientThread;
		PVOID                   Win32ThreadInfo;
		ULONG                   Win32ClientInfo[0x1F];
		PVOID                   WOW32Reserved;
		ULONG                   CurrentLocale;
		ULONG                   FpSoftwareStatusRegister;
		PVOID                   SystemReserved1[0x36];
		PVOID                   Spare1;
		ULONG                   ExceptionCode;
		ULONG                   SpareBytes1[0x28];
		PVOID                   SystemReserved2[0xA];
		ULONG                   GdiRgn;
		ULONG                   GdiPen;
		ULONG                   GdiBrush;
		CLIENT_ID               RealClientId;
		PVOID                   GdiCachedProcessHandle;
		ULONG                   GdiClientPID;
		ULONG                   GdiClientTID;
		PVOID                   GdiThreadLocaleInfo;
		PVOID                   UserReserved[5];
		PVOID                   GlDispatchTable[0x118];
		ULONG                   GlReserved1[0x1A];
		PVOID                   GlReserved2;
		PVOID                   GlSectionInfo;
		PVOID                   GlSection;
		PVOID                   GlTable;
		PVOID                   GlCurrentRC;
		PVOID                   GlContext;
		NTSTATUS                LastStatusValue;
		UNICODE_STRING          StaticUnicodeString;
		WCHAR                   StaticUnicodeBuffer[0x105];
		PVOID                   DeallocationStack;
		PVOID                   TlsSlots[0x40];
		LIST_ENTRY              TlsLinks;
		PVOID                   Vdm;
		PVOID                   ReservedForNtRpc;
		PVOID                   DbgSsReserved[0x2];
		ULONG                   HardErrorDisabled;
		PVOID                   Instrumentation[0x10];
		PVOID                   WinSockData;
		ULONG                   GdiBatchCount;
		ULONG                   Spare2;
		ULONG                   Spare3;
		ULONG                   Spare4;
		PVOID                   ReservedForOle;
		ULONG                   WaitingOnLoaderLock;
		PVOID                   StackCommit;
		PVOID                   StackCommitMax;
		PVOID                   StackReserved;
	}TEB, *PTEB;


	typedef VOID (*PIO_APC_ROUTINE)(
		PVOID ApcContext,
		PIO_STATUS_BLOCK IoStatusBlock,
		ULONG Reserved);

	typedef struct _STRING 
	{
		USHORT  Length;
		USHORT  MaximumLength;
		PCHAR  Buffer;
	}ANSI_STRING, *PANSI_STRING, *PCANSI_STRING;


	typedef struct _OBJECT_NAME_INFORMATION 
	{ 
		UNICODE_STRING Name; 
	}OBJECT_NAME_INFORMATION, *POBJECT_NAME_INFORMATION; 

	typedef enum _OBJECT_INFORMATION_CLASS 
	{
		ObjectBasicInformation,
		ObjectNameInformation,
		ObjectTypeInformation,
		ObjectAllInformation,
		ObjectDataInformation
	} OBJECT_INFORMATION_CLASS, *POBJECT_INFORMATION_CLASS;


	//	NtSystemDebugControl 功能号
	typedef enum _SYSDBG_COMMAND
	{
		SysDbgGetTraceInformation = 1,
		SysDbgSetInternalBreakpoint = 2,
		SysDbgSetSpecialCall = 3,
		SysDbgClearSpecialCalls = 4,
		SysDbgQuerySpecialCalls = 5,

		//	以下是NT 5.1 新增的
		SysDbgDbgBreakPointWithStatus = 6,
		//	获取KdVersionBlock
		SysDbgSysGetVersion = 7,

		//从内核空间拷贝到用户空间，或者从用户空间拷贝到用户空间
		//但是不能从用户空间拷贝到内核空间
		SysDbgReadCoreMem = 8,

		//从用户空间拷贝到内核空间，或者从用户空间拷贝到用户空间
		//但是不能从内核空间拷贝到用户空间
		SysDbgWriteCoreMem = 9,

		//从物理地址拷贝到用户空间，不能写到内核空间
		SysDbgReadPhysMem = 10,

		//从用户空间拷贝到物理地址，不能读取内核空间
		SysDbgWritePhysMem = 11,

		//读写处理器相关控制块
		SysDbgSysReadControlSpace = 12,
		SysDbgSysWriteControlSpace = 13,

		//读写端口
		SysDbgSysReadIoSpace = 14,
		SysDbgSysWriteIoSpace = 15,

		//分别调用RDMSR@4和_WRMSR@12
		SysDbgSysReadMsr = 16,
		SysDbgSysWriteMsr = 17,

		//读写总线数据
		SysDbgSysReadBusData = 18,
		SysDbgSysWriteBusData = 19,
		SysDbgSysCheckLowMemory = 20,

		// 以下是NT 5.2 新增的
		//分别调用_KdEnableDebugger@0和_KdDisableDebugger@0
		SysDbgEnableDebugger = 21,
		SysDbgDisableDebugger = 22,

		//获取和设置一些调试相关的变量
		SysDbgGetAutoEnableOnEvent = 23,
		SysDbgSetAutoEnableOnEvent = 24,
		SysDbgGetPitchDebugger = 25,
		SysDbgSetDbgPrintBufferSize = 26,
		SysDbgGetIgnoreUmExceptions = 27,
		SysDbgSetIgnoreUmExceptions = 28
	}SYSDBG_COMMAND;

	typedef struct _MSR_STRUCT 
	{
		DWORD MsrNum;	// MSR number
		DWORD NotUsed;	// Never accessed by the kernel
		DWORD MsrLo;	// IN (write) or OUT (read): Low 32 bits of MSR
		DWORD MsrHi;	// IN (write) or OUT (read): High 32 bits of MSR
	}MSR_STRUCT;

	typedef struct _MEMORY_CHUNKS
	{
		ULONG Address;
		PVOID Data;
		ULONG Length;
	}MEMORY_CHUNKS, *PMEMORY_CHUNKS;


#ifndef MAXIMUM_FILENAME_LENGTH
#define MAXIMUM_FILENAME_LENGTH	256
#endif

	typedef struct _SYSTEM_MODULE 
	{
		ULONG                Reserved1;
		ULONG                Reserved2;
		PVOID                ImageBaseAddress;
		ULONG                ImageSize;
		ULONG                Flags;
		WORD                 Id;
		WORD                 Rank;
		WORD                 w018;
		WORD                 NameOffset;
		BYTE                 Name[MAXIMUM_FILENAME_LENGTH];
	} SYSTEM_MODULE, *PSYSTEM_MODULE;

	typedef struct _SYSTEM_MODULE_INFORMATION 
	{
		ULONG                ModulesCount;
		SYSTEM_MODULE        Modules[ANY_SIZE];
	} SYSTEM_MODULE_INFORMATION, *PSYSTEM_MODULE_INFORMATION;


	typedef struct _OBJDIR_INFORMATION
	{
		UNICODE_STRING          ObjectName;
		UNICODE_STRING          ObjectTypeName;
		BYTE                    Data[ANY_SIZE];
	} OBJDIR_INFORMATION, *POBJDIR_INFORMATION;


	typedef enum _Dir_acs
	{
		DIRECTORY_QUERY = 0x01,
		DIRECTORY_TRAVERSE = 0x02,
		DIRECTORY_CREATE_OBJECT = 0x04,
		DIRECTORY_CREATE_SUBDIRECTORY = 0x08,
		DIRECTORY_ALL_ACCESS = (STANDARD_RIGHTS_REQUIRED | 0xF)
	};


	enum FILE_INFORMATION_CLASS 
	{
		FileDirectoryInformation = 1,
		FileFullDirectoryInformation,
		FileBothDirectoryInformation,
		FileBasicInformation,
		FileStandardInformation,
		FileInternalInformation,
		FileEaInformation,
		FileAccessInformation,
		FileNameInformation,
		FileRenameInformation,
		FileLinkInformation,
		FileNamesInformation,
		FileDispositionInformation,
		FilePositionInformation,
		FileFullEaInformation,
		FileModeInformation,
		FileAlignmentInformation,
		FileAllInformation,
		FileAllocationInformation,
		FileEndOfFileInformation,
		FileAlternateNameInformation,
		FileStreamInformation,
		FilePipeInformation,
		FilePipeLocalInformation,
		FilePipeRemoteInformation,
		FileMailslotQueryInformation,
		FileMailslotSetInformation,
		FileCompressionInformation,
		FileCopyOnWriteInformation,
		FileCompletionInformation,
		FileMoveClusterInformation,
		FileQuotaInformation,
		FileReparsePointInformation,
		FileNetworkOpenInformation,
		FileObjectIdInformation,
		FileTrackingInformation,
		FileOleDirectoryInformation,
		FileContentIndexInformation,
		FileInheritContentIndexInformation,
		FileOleInformation,
		FileMaximumInformation
	};

	typedef enum _POOL_TYPE 
	{
		NonPagedPool,
		PagedPool,
		NonPagedPoolMustSucceed,
		DontUseThisType,
		NonPagedPoolCacheAligned,
		PagedPoolCacheAligned,
		NonPagedPoolCacheAlignedMustS
	} POOL_TYPE;

	typedef struct _OBJECT_TYPE_INFORMATION 
	{
		UNICODE_STRING          TypeName;
		ULONG                   TotalNumberOfHandles;
		ULONG                   TotalNumberOfObjects;
		WCHAR                   Unused1[8];
		ULONG                   HighWaterNumberOfHandles;
		ULONG                   HighWaterNumberOfObjects;
		WCHAR                   Unused2[8];
		ACCESS_MASK             InvalidAttributes;
		GENERIC_MAPPING         GenericMapping;
		ACCESS_MASK             ValidAttributes;
		BOOLEAN                 SecurityRequired;
		BOOLEAN                 MaintainHandleCount;
		USHORT                  MaintainTypeList;
		POOL_TYPE               PoolType;
		ULONG                   DefaultPagedPoolCharge;
		ULONG                   DefaultNonPagedPoolCharge;
	} OBJECT_TYPE_INFORMATION, *POBJECT_TYPE_INFORMATION;


////////////////////////////////////////////////////////////
//	函数类型

	typedef	NTSTATUS (NTAPI * Fun_NtCreatePagingFile)(
		IN UNICODE_STRING *      PageFileName,
		IN PLARGE_INTEGER       MiniumSize,
		IN PLARGE_INTEGER       MaxiumSize,
		OUT PLARGE_INTEGER      ActualSize);

	typedef	NTSTATUS (NTAPI * Fun_NtCreateFile)(
		OUT PHANDLE             FileHandle,
		IN ACCESS_MASK          DesiredAccess,
		IN POBJECT_ATTRIBUTES	ObjectAttributes,
		OUT PIO_STATUS_BLOCK	IoStatusBlock,
		IN PLARGE_INTEGER       AllocationSize,
		IN ULONG                FileAttributes,
		IN ULONG                ShareAccess,
		IN ULONG                CreateDisposition,
		IN ULONG                CreateOptions,
		IN PVOID                EaBuffer,
		IN ULONG                EaLength );

	typedef	NTSTATUS (NTAPI * Fun_NtSetInformationFile)(
		IN HANDLE               FileHandle,
		OUT PIO_STATUS_BLOCK    IoStatusBlock,
		IN PVOID                FileInformation,
		IN ULONG                Length,
		IN FILE_INFORMATION_CLASS FileInformationClass);


	typedef NTSTATUS (NTAPI * Fun_NtClose)(
		IN HANDLE               ObjectHandle);

	typedef VOID (NTAPI * Fun_RtlInitUnicodeString)(
		IN OUT PUNICODE_STRING  DestinationString,
		IN PCWSTR  SourceString);


	typedef NTSTATUS (NTAPI * Fun_NtQuerySystemInformation)(
		IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
		OUT PVOID               SystemInformation,
		IN ULONG                SystemInformationLength,
		OUT PULONG              ReturnLength OPTIONAL);

	typedef NTSTATUS (NTAPI * Fun_NtQuerySystemInformation)(
		IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
		OUT PVOID               SystemInformation,
		IN ULONG                SystemInformationLength,
		OUT PULONG              ReturnLength OPTIONAL);

	typedef NTSTATUS (NTAPI * Fun_RtlAnsiStringToUnicodeString)(
		PUNICODE_STRING DestinationString,
		PCANSI_STRING SourceString,
		BOOLEAN AllocateDestinationString);

	typedef DWORD (NTAPI * Fun_RtlNtStatusToDosError)(
		IN IN NTSTATUS Status);


	typedef NTSTATUS (NTAPI * Fun_NtLoadDriver)(
		IN PUNICODE_STRING DriverServiceName);

	typedef NTSTATUS (NTAPI * Fun_NtUnloadDriver)(
		IN PUNICODE_STRING DriverServiceName );

	typedef NTSTATUS (NTAPI * Fun_NtQueryObject)(
		IN HANDLE               ObjectHandle,
		IN OBJECT_INFORMATION_CLASS ObjectInformationClass,
		OUT PVOID               ObjectInformation,
		IN ULONG                Length,
		OUT PULONG              ResultLength );

	typedef NTSTATUS (NTAPI * Fun_NtFsControlFile)(
		IN HANDLE               FileHandle,
		IN HANDLE               Event OPTIONAL,
		IN PIO_APC_ROUTINE      ApcRoutine OPTIONAL,
		IN PVOID                ApcContext OPTIONAL,
		OUT PIO_STATUS_BLOCK    IoStatusBlock,
		IN ULONG                FsControlCode,
		IN PVOID                InputBuffer OPTIONAL,
		IN ULONG                InputBufferLength,
		OUT PVOID               OutputBuffer OPTIONAL,
		IN ULONG                OutputBufferLength );

	typedef NTSTATUS (NTAPI * Fun_NtSystemDebugControl)(
		IN SYSDBG_COMMAND       Command,
		IN PVOID                InputBuffer OPTIONAL,
		IN ULONG                InputBufferLength,
		OUT PVOID               OutputBuffer OPTIONAL,
		IN ULONG                OutputBufferLength,
		OUT PULONG              ReturnLength OPTIONAL );

	typedef	NTSTATUS (NTAPI * Fun_NtOpenDirectoryObject)(
		OUT PHANDLE             DirectoryObjectHandle,
		IN ACCESS_MASK          DesiredAccess,
		IN POBJECT_ATTRIBUTES   ObjectAttributes);


	typedef NTSTATUS (NTAPI * Fun_NtQueryDirectoryObject)(
		IN HANDLE               DirectoryObjectHandle,
		OUT POBJDIR_INFORMATION DirObjInformation,
		IN ULONG                BufferLength,
		IN BOOLEAN              GetNextIndex,
		IN BOOLEAN              IgnoreInputIndex,
		IN OUT PULONG           ObjectIndex,
		OUT PULONG              DataWritten OPTIONAL);

	typedef	NTSTATUS (NTAPI * Fun_NtQueryTimerResolution)(
		OUT PULONG              MinimumResolution,
		OUT PULONG              MaximumResolution,
		OUT PULONG              CurrentResolution);

	typedef	NTSTATUS (NTAPI * Fun_NtSetTimerResolution)(
		IN ULONG                DesiredResolution,
		IN BOOLEAN              SetResolution,
		OUT PULONG              CurrentResolution );

	typedef	NTSTATUS (NTAPI * Fun_NtQueryInformationFile)(
		IN HANDLE               FileHandle,
		OUT PIO_STATUS_BLOCK    IoStatusBlock,
		OUT PVOID               FileInformation,
		IN ULONG                Length,
		IN FILE_INFORMATION_CLASS FileInformationClass);

	typedef NTSTATUS (NTAPI * Fun_NtQueryObject)(
		IN HANDLE               ObjectHandle,
		IN OBJECT_INFORMATION_CLASS ObjectInformationClass,
		OUT PVOID               ObjectInformation,
		IN ULONG                Length,
		OUT PULONG              ResultLength);
//////////////////////////////////////////////////////////////////////////////
//	声明

	NTSTATUS	NtFsControlFile(
		IN HANDLE               FileHandle, 
		IN HANDLE               Event, 
		IN PIO_APC_ROUTINE      ApcRoutine, 
		IN PVOID                ApcContext, 
		OUT PIO_STATUS_BLOCK    IoStatusBlock, 
		IN ULONG                FsControlCode, 
		IN PVOID                InputBuffer, 
		IN ULONG                InputBufferLength, 
		OUT PVOID               OutputBuffer, 
		IN ULONG                OutputBufferLength);

	//	注意调试权限
	NTSTATUS	NtSystemDebugControl(
		IN SYSDBG_COMMAND       Command,
		IN PVOID                InputBuffer OPTIONAL,
		IN ULONG                InputBufferLength,
		OUT PVOID               OutputBuffer OPTIONAL,
		IN ULONG                OutputBufferLength,
		OUT PULONG              ReturnLength OPTIONAL);


	typedef NTSTATUS (NTAPI * Fun_NtUnmapViewOfSection)(
		IN HANDLE               ProcessHandle,
		IN PVOID                BaseAddress );


	NTSTATUS	NtUnmapViewOfSection(
		IN HANDLE               ProcessHandle,
		IN PVOID                BaseAddress );

	NTSTATUS	NtLoadDriver(
		PUNICODE_STRING DriverServiceName);

	NTSTATUS	NtUnloadDriver(
		PUNICODE_STRING DriverServiceName);

	DWORD		RtlNtStatusToDosError(
		NTSTATUS Status);

	NTSTATUS	NtQueryInformationProcess(
		IN HANDLE               ProcessHandle,
		IN PROCESS_INFORMATION_CLASS ProcessInformationClass,
		OUT PVOID               ProcessInformation,
		IN ULONG                ProcessInformationLength,
		OUT PULONG              ReturnLength);

	NTSTATUS	NtQuerySystemInformation(
		IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
		OUT PVOID               SystemInformation,
		IN ULONG                SystemInformationLength,
		OUT PULONG              ReturnLength);


	NTSTATUS	NtOpenDirectoryObject(
		OUT PHANDLE             DirectoryObjectHandle,
		IN ACCESS_MASK          DesiredAccess,
		IN POBJECT_ATTRIBUTES   ObjectAttributes);


	NTSTATUS	NtQueryDirectoryObject(
		IN HANDLE               DirectoryObjectHandle,
		OUT POBJDIR_INFORMATION DirObjInformation,
		IN ULONG                BufferLength,
		IN BOOLEAN              GetNextIndex,
		IN BOOLEAN              IgnoreInputIndex,
		IN OUT PULONG           ObjectIndex,
		OUT PULONG              DataWritten);

	NTSTATUS	NtQueryTimerResolution(
		OUT PULONG              MinimumResolution,
		OUT PULONG              MaximumResolution,
		OUT PULONG              CurrentResolution );

	NTSTATUS	NtSetTimerResolution(
		IN ULONG                DesiredResolution,
		IN BOOLEAN              SetResolution,
		OUT PULONG              CurrentResolution );

	NTSTATUS	NtQueryInformationFile(
		IN HANDLE               FileHandle,
		OUT PIO_STATUS_BLOCK    IoStatusBlock,
		OUT PVOID               FileInformation,
		IN ULONG                Length,
		IN FILE_INFORMATION_CLASS FileInformationClass);

	NTSTATUS	NtQueryObject(
		IN HANDLE               ObjectHandle,
		IN OBJECT_INFORMATION_CLASS ObjectInformationClass,
		OUT PVOID               ObjectInformation,
		IN ULONG                Length,
		OUT PULONG              ResultLength);


//////////////////////////////////////////////////////////////
	BOOL	RdMsr(DWORD dwFun, DWORD dwMsr[2]);
	DWORD	GetParentProcId(HANDLE hProc);
	DWORD	GetParentProcId(DWORD dwPid);
	BOOL	GetSysHandleList(std::vector<SYSTEM_HANDLE_INFORMATION> &vHandleList);
	BOOL	GetSysModInfo(std::vector<SYSTEM_MODULE> &vModList);
	DWORD	ReadKernelMem(DWORD dwAddress, DWORD dwSize, VOID *pBuffer);
	DWORD	WriteKernelMem(DWORD dwAddress, DWORD dwSize, VOID *pBuffer);
	BOOL	GetDireObjetList(CHAR *pPath, std::vector<OBJDIR_INFORMATION> &vObjList);


	//		需要free
	SYSTEM_PROCESS_INFORMATION *	GetSysProcessInfo();

	//	pBuffer	需要足够大
	SYSTEM_PROCESS_INFORMATION *	GetProcessInfo(DWORD dwProcId, CHAR *pBuffer, int nBufSize = 4096);

	VOID *	GetPEB(HANDLE hProc = NULL);

}









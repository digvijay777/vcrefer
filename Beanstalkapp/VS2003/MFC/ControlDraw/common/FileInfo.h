#pragma once
#include "stdEx.h"
#include "CSimple.h"


class CFileSmallIconMgr;




class CIcon
{
	typedef struct _BMINFO
	{
		BITMAP			Bm;
		BITMAPINFO *	pBmInfo;
		BYTE *			pBmData;
	}BMINFO;

public:
	CIcon();
	virtual	~CIcon();

	static	BYTE *	GetBitMapDIB(HBITMAP hBm, BITMAP *pBm = NULL, BITMAPINFO **ppOutBmInfo = NULL);

	BOOL	GetDetailInfo();
	BOOL	GetBitmapInfo(HBITMAP hBm, BMINFO *pBmInfo);
	BOOL	Clear();
	int		CompareBmInfo(BMINFO &Left, BMINFO &Right);
	int		Compare(CIcon &TestIcon);
	HICON	GetIcon();
	BOOL	Destory();

	CIcon& operator = (HICON hIcon);
	bool operator < (const CIcon& _Right) const;
	//	operator HICON() const;		//map的比较会冲突

	HICON			m_hIcon;

	BOOL			m_bDetailInfo;
	ICONINFO		m_IconInfo;
	BMINFO			m_Color;
	BMINFO			m_Mask;
};


class CIconCache
{
public:
	CIconCache();
	virtual	~CIconCache();

	BOOL	Clear();
	HICON	AddIcon(HICON hIcon, BOOL &bRealAdd);

	std::map<CIcon, int>	m_IconCache;
};


class CFileIconCache
{
	typedef struct _ICONCACHE
	{
		DWORD			dwTime;
		HICON			hRealIcon;
	}ICONCACHE;

public:
	CFileIconCache();
	~CFileIconCache();

	BOOL	Clear();

	HICON	GetFileIcon(WCHAR *pFilePath);
	HICON	GetFileIcon(CHAR *pFilePath);

	BOOL	RealGetFileIcon(WCHAR *pFilePath, HICON &hIcon);


	CIconCache			m_IconChche;
	std::map<std::wstring, ICONCACHE>	m_FileCache;
};


class OneFileinfo
{
public:

	OneFileinfo()
	{
		memset(this, 0, sizeof(OneFileinfo));
	}

	~OneFileinfo()
	{

	}

	DWORD						m_dwFalg;
	BY_HANDLE_FILE_INFORMATION	m_ByHandle;
	HICON						m_hFileIcon;
	NFile::FILE_VERSION			m_VerInfo;
	BOOL						m_bExecute;
	IMAGE_DOS_HEADER			m_DosHead;
	IMAGE_NT_HEADERS32			m_NtHead;
	BOOL						m_bMsTrust;
	BOOL						m_bSigTrust;
	DWORD						m_dwFalgChecked;
};



class CFileTrustVerify
{
public:

	CFileTrustVerify();
	~CFileTrustVerify();

	static BOOL	VerifyOneFile(CHAR *pFilePath, BOOL &bMsTrust, BOOL &bSigTrust);

	BOOL	VerifyFile(CHAR *pFilePath, BOOL &bMsTrust, BOOL &bSigTrust);
	BOOL	MsFileVerify(CHAR *pFilePath, BOOL &bMsTrust);
	BOOL	FileSigVerify(CHAR *pFilePath, BOOL &bSigTrust);


	BOOL	InitTrustApi();
	BOOL	HashFileData(CHAR *pFilePath, DWORD *pcbHash, BYTE **ppHashValue);
};


#define FileInfo_Flag_Base			0x0001
#define FileInfo_Flag_Icon			0x0002
#define FileInfo_Flag_Version		0x0004
#define FileInfo_Flag_Pe			0x0008
#define FileInfo_Flag_Trust			0x0010
#define FileInfo_Flag_All			0xFFFF
#define FileInfo_Flag_Default		0x000F


class CFileInfoMgr
{
protected:
	CFileInfoMgr();
public:
	~CFileInfoMgr();
	static CFileInfoMgr * CreateInstance();

	static DWORD WINAPI TrustCheckProc(LPVOID lpThreadParameter);

	//	取文件信息时, 是否检测文件已经被修改(大小, 时间)
	virtual	BOOL	SetCacheCheck(BOOL bCheck = FALSE);
	virtual	BOOL	GetFileInfo(CHAR *pFilePath, OneFileinfo *pInfo);
	virtual	BOOL	SetCheckFlag(DWORD dwInfoFlag = FileInfo_Flag_Base, DWORD dwReCheckFlage = FileInfo_Flag_Default);
	virtual	BOOL	IsFinish();

	virtual OneFileinfo *	GetFileInfo(CHAR *pFilePath);
	virtual OneFileinfo *	GetFileInfo(WCHAR *pFilePath);


	BOOL	RealGetFileInfo(CHAR *pFilePath, OneFileinfo *pInfo);
	BOOL	RealGetInfoByFlag(CHAR *pFilePath, OneFileinfo *pInfo, DWORD dwFlage);
	BOOL	RealGetFilePeInfo(CHAR *pFilePath, OneFileinfo *pInfo);
	BOOL	RealGetTrustInfo(CHAR *pFilePath, OneFileinfo *pInfo);


	OneFileinfo *	FindInfoInCache(CHAR *pFilePath);
	OneFileinfo *	SaveInfoInCache(CHAR *pFilePath, OneFileinfo *pInfo);

	BOOL	ClearCache();
	BOOL	ConvertPath(CHAR *pFilePath, CHAR *pRealPath);
	BOOL	CheckCatchFileInfo(CHAR *pFilePath, OneFileinfo *pCatchInfo);

	BOOL	GetUnCheckedItem(CHAR *pFilePath, OneFileinfo *pNewInfo);
	BOOL	SaveReCheckItem(CHAR *pFilePath, OneFileinfo *pNewInfo);


	BOOL		m_bCacheCheck;
	HANDLE		m_hTrustCheckThread;
	BOOL		m_bExit;
	BOOL		m_bFinish;

	CFileIconCache							m_IconCache;
	std::map<string_NoCase, OneFileinfo>	m_InfoMap;
	CThreadLock								m_CacheLock;

	DWORD									m_dwInfoFlag;
	DWORD									m_dwReCheckFlag;
};




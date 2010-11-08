#include "FileInfo.h"
#include <SoftPub.h>
#include <wintrust.h>
#include "CSimple.h"

#ifndef HCATADMIN
#define HCATADMIN	HANDLE
#define HCATINFO	PVOID
#endif

typedef BOOL (__stdcall *Fun_CryptCATAdminAcquireContext)(HCATADMIN *phCatAdmin, const GUID *pgSubsystem, DWORD dwFlags);
typedef BOOL (__stdcall *Fun_CryptCATAdminReleaseContext)(HCATADMIN hCatAdmin, DWORD dwFlags);
typedef BOOL (__stdcall *Fun_CryptCATAdminCalcHashFromFileHandle)(HANDLE hFile, DWORD *pcbHash, BYTE *pbHash, DWORD dwFlags);
typedef HCATINFO (__stdcall *Fun_CryptCATAdminEnumCatalogFromHash)(HCATADMIN hCatAdmin, BYTE *pbHash, DWORD cbHash, DWORD dwFlags, HCATINFO *phPrevCatInfo);
typedef BOOL (__stdcall *Fun_CryptCATAdminReleaseCatalogContext)(HCATADMIN hCatAdmin, HCATINFO hCatInfo, DWORD dwFlags);
typedef LONG (__stdcall *Fun_WinVerifyTrust)(HWND hwnd, GUID *pgActionID, LPVOID pWVTData);


static BOOL		g_bTrustApiInit = FALSE;
Fun_CryptCATAdminAcquireContext			_CryptCATAdminAcquireContext = NULL;
Fun_CryptCATAdminReleaseContext			_CryptCATAdminReleaseContext = NULL;
Fun_CryptCATAdminCalcHashFromFileHandle	_CryptCATAdminCalcHashFromFileHandle = NULL;
Fun_CryptCATAdminEnumCatalogFromHash	_CryptCATAdminEnumCatalogFromHash = NULL;
Fun_CryptCATAdminReleaseCatalogContext	_CryptCATAdminReleaseCatalogContext = NULL;
Fun_WinVerifyTrust						_WinVerifyTrust = NULL;





///////////////////////////////////////////////////////////////////////////////////////////////////
//		CIcon

CIcon::CIcon()
{
	m_hIcon = NULL;
	m_bDetailInfo = FALSE;

	memset(&m_Color, 0, sizeof(m_Color));
	memset(&m_Mask, 0, sizeof(m_Mask));
}

CIcon::~CIcon()
{
	Clear();
}


BOOL	CIcon::Clear()
{
	m_hIcon = NULL;

	memset(&m_Color, 0, sizeof(m_Color));
	memset(&m_Mask, 0, sizeof(m_Mask));
	m_bDetailInfo = FALSE;

	return TRUE;
}


BOOL	CIcon::Destory()
{
	if(m_hIcon)
		DestroyIcon(m_hIcon);

	if(m_bDetailInfo == FALSE)
		return TRUE;

	if(m_Color.pBmData)
		free(m_Color.pBmData);
	if(m_Color.pBmInfo)
		free(m_Color.pBmInfo);
	if(m_Mask.pBmData)
		free(m_Mask.pBmData);
	if(m_Mask.pBmInfo)
		free(m_Mask.pBmInfo);

	return TRUE;
}


BOOL	CIcon::GetDetailInfo()
{
	BOOL		bRetVal;

	if(m_bDetailInfo)
		return TRUE;

	bRetVal = GetIconInfo(m_hIcon, &m_IconInfo);
	if(bRetVal == FALSE)
		return FALSE;

	GetBitmapInfo(m_IconInfo.hbmColor, &m_Color);
	GetBitmapInfo(m_IconInfo.hbmMask, &m_Mask);

	DeleteObject(m_IconInfo.hbmColor);
	DeleteObject(m_IconInfo.hbmMask);

	m_bDetailInfo = TRUE;

	return TRUE;
}


BOOL	CIcon::GetBitmapInfo(HBITMAP hBm, BMINFO *pBmInfo)
{
	BOOL		bRetVal;

	bRetVal = GetObject((HGDIOBJ)hBm, sizeof(pBmInfo->Bm), &pBmInfo->Bm);
	if(bRetVal == FALSE)
	{
		return FALSE;
	}

	pBmInfo->pBmData = GetBitMapDIB(hBm, &pBmInfo->Bm, &pBmInfo->pBmInfo);
	if(pBmInfo->pBmData == NULL)
	{
		assert(0);
		return FALSE;
	}

	return TRUE;
}


BYTE *	CIcon::GetBitMapDIB(HBITMAP hBm, BITMAP *pBm, BITMAPINFO **ppOutBmInfo)
{
	BITMAPINFO *	pBmInfo;
	BITMAP			bm;
	int				nRetVal;
	HDC				hDc;
	BYTE *			pBmDib;
	BOOL			bRetVal;
	int				nColors;
	int				nBmInfoLen;

	if(pBm == NULL)
	{
		nRetVal = GetObject((HGDIOBJ)hBm, sizeof(hBm), &bm);
		if(nRetVal <= 0)
			return NULL;

		pBm = &bm;
	}

	if(pBm->bmBitsPixel < 8)
	{
		nColors = 1 << pBm->bmBitsPixel;
	}
	else
	{
		nColors = 0;
	}
	nBmInfoLen = sizeof(BITMAPINFO) + (sizeof(RGBQUAD) * nColors);
	pBmInfo = (BITMAPINFO *)malloc(nBmInfoLen);

	memset(pBmInfo, 0, nBmInfoLen);
	pBmInfo->bmiHeader.biSize			= sizeof(pBmInfo->bmiHeader);
	pBmInfo->bmiHeader.biPlanes			= 1;
	pBmInfo->bmiHeader.biCompression	= BI_RGB;

	pBmInfo->bmiHeader.biBitCount		= pBm->bmBitsPixel;
	pBmInfo->bmiHeader.biWidth			= pBm->bmWidth;
	pBmInfo->bmiHeader.biHeight 		= pBm->bmHeight;


	hDc = GetDC(NULL);

	GetDIBits(hDc, hBm, 0, pBm->bmHeight, NULL, pBmInfo, DIB_RGB_COLORS);
	if(pBmInfo->bmiHeader.biSizeImage == 0)
	{
		pBmInfo->bmiHeader.biSizeImage =	NData::Align((DWORD)pBmInfo->bmiHeader.biWidth * pBmInfo->bmiHeader.biBitCount, 32);
		pBmInfo->bmiHeader.biSizeImage /= 8;
		pBmInfo->bmiHeader.biSizeImage *= pBmInfo->bmiHeader.biHeight;
	}

	pBmDib = (BYTE *)malloc(pBmInfo->bmiHeader.biSizeImage);
	bRetVal = GetDIBits(hDc, hBm, 0, pBm->bmHeight, pBmDib, pBmInfo, DIB_RGB_COLORS);
	if(bRetVal == FALSE)
	{
		assert(0);
		free(pBmDib);
		pBmDib = NULL;
	}
	if(ppOutBmInfo)
		*ppOutBmInfo = pBmInfo;
	else
		free(pBmInfo);

	ReleaseDC(NULL, hDc);
	return pBmDib;
}



int		CIcon::CompareBmInfo(BMINFO &Left, BMINFO &Right)
{
	int			nCompare;

	nCompare = memcmp(&Left.Bm, &Right.Bm, sizeof(Left.Bm));
	if(nCompare != 0)
		return nCompare;

	if(Left.pBmInfo == NULL || Right.pBmInfo == NULL)
		return NData::Compare(Left.pBmData, Right.pBmData);
	nCompare = memcmp(Left.pBmInfo, Right.pBmInfo, sizeof(BITMAPINFO));
	if(nCompare != 0)
		return nCompare;

	if(Left.pBmData == NULL || Right.pBmData == NULL)
		return NData::Compare(Left.pBmData, Right.pBmData);
	nCompare = memcmp(Left.pBmData, Right.pBmData, Left.pBmInfo->bmiHeader.biSizeImage);

	return nCompare;
}


int		CIcon::Compare(CIcon &TestIcon)
{
	int			nCompare;

	GetDetailInfo();
	TestIcon.GetDetailInfo();

	if(m_hIcon == NULL || TestIcon.m_hIcon == NULL)
	{
		return NData::Compare(m_hIcon, TestIcon.m_hIcon);
	}

	nCompare = NData::Compare(m_bDetailInfo, TestIcon.m_bDetailInfo);
	if(nCompare != 0)
		return nCompare;

	nCompare = NData::Compare(m_IconInfo.xHotspot, TestIcon.m_IconInfo.xHotspot);
	if(nCompare != 0)
		return nCompare;

	nCompare = NData::Compare(m_IconInfo.yHotspot, TestIcon.m_IconInfo.yHotspot);
	if(nCompare != 0)
		return nCompare;

	nCompare = CompareBmInfo(m_Color, TestIcon.m_Color);
	if(nCompare != 0)
		return nCompare;

	nCompare = CompareBmInfo(m_Mask, TestIcon.m_Mask);
	return nCompare;
}


CIcon& CIcon::operator = (HICON hIcon)
{
	Clear();
	m_hIcon = hIcon;
	return *this;
}


HICON	CIcon::GetIcon()
{
	return m_hIcon;
}


bool CIcon::operator < (const CIcon& _Right) const
{
	int			nCompare;
	CIcon *		pThis;

	pThis = (CIcon *)this;
	nCompare = pThis->Compare(*(CIcon *)&_Right);
	if(nCompare < 0)
		return true;

	return false;
}

// CIcon::operator HICON() const
// {
// 	return m_hIcon;
// }

//		CIcon
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//		CIconCache

CIconCache::CIconCache()
{

}

CIconCache::~CIconCache()
{
	Clear();
}

BOOL	CIconCache::Clear()
{
	std::map<CIcon, int>::iterator		Iter;
	CIcon *		pItem;

	for(Iter = m_IconCache.begin();
		Iter != m_IconCache.end();
		Iter ++)
	{
		pItem = (CIcon *)&Iter->first;
		pItem->Destory();
	}
	m_IconCache.clear();

	return TRUE;
}



HICON	CIconCache::AddIcon(HICON hIcon, BOOL &bRealAdd)
{
	std::map<CIcon, int>::iterator	Iter;
	CIcon		Icon;
	HICON		hRetVal;
	CIcon *		pMapItem;

	bRealAdd = FALSE;
	Icon = hIcon;
	Iter = m_IconCache.find(Icon);
	if(Iter != m_IconCache.end())
	{
		Icon.Destory();
		Iter->second ++;
		pMapItem = (CIcon *)&Iter->first;
		hRetVal = pMapItem->GetIcon();
		return hRetVal;
	}

	bRealAdd = TRUE;
	m_IconCache[Icon] = 1;
	return hIcon;
}


//		CIconCache
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//		CFileIconCache
CFileIconCache::CFileIconCache()
{

}

CFileIconCache::~CFileIconCache()
{
	Clear();
}

BOOL	CFileIconCache::Clear()
{
	m_IconChche.Clear();

	return TRUE;
}

HICON	CFileIconCache::GetFileIcon(WCHAR *pFilePath)
{
	std::wstring		sFilePath;
	WCHAR				szFilePath[MAXPATH];
	ICONCACHE *			pCache;
	HICON				hIcon;

	wcscpy(szFilePath, pFilePath);
	wcsupr(szFilePath);

	sFilePath = szFilePath;
	pCache = StdEx::FindInMap(m_FileCache, sFilePath);
	if(pCache)
	{
		pCache->dwTime = GetTickCount();
		return pCache->hRealIcon;
	}

	RealGetFileIcon(szFilePath, hIcon);
	pCache = StdEx::AddToMap(m_FileCache, sFilePath);
	pCache->dwTime = GetTickCount();
	pCache->hRealIcon = hIcon;

	return pCache->hRealIcon;
}

HICON	CFileIconCache::GetFileIcon(CHAR *pFilePath)
{
	WCHAR				szFilePath[MAXPATH];

	NStr::Char2WChar(pFilePath, szFilePath, sizeof(szFilePath));

	return GetFileIcon(szFilePath);
}

BOOL	CFileIconCache::RealGetFileIcon(WCHAR *pFilePath, HICON &hIcon)
{
	HICON		hFileIcon;
	BOOL		bRealAdd;

	hIcon = NULL;
	hFileIcon = NFile::GetFileSmallIcon(pFilePath);
	if(hFileIcon == NULL)
		return FALSE;

	hIcon = m_IconChche.AddIcon(hFileIcon, bRealAdd);
	if(bRealAdd == FALSE)
		DestroyIcon(hFileIcon);

	return TRUE;
}

//	CFileIconCache
//////////////////////////////////////////////////////////////////////////////////////////





//////////////////////////////////////////////////////////////////////////////////////////
//	CFileTrustVerify

CFileTrustVerify::CFileTrustVerify()
{

}

CFileTrustVerify::~CFileTrustVerify()
{

};

BOOL	CFileTrustVerify::VerifyOneFile(CHAR *pFilePath, BOOL &bMsTrust, BOOL &bSigTrust)
{
	CFileTrustVerify	ftv;
	BOOL				bRetVal;

	bRetVal = ftv.VerifyFile(pFilePath, bMsTrust, bSigTrust);
	return bRetVal;
}


BOOL	CFileTrustVerify::VerifyFile(CHAR *pFilePath, BOOL &bMsTrust, BOOL &bSigTrust)
{
	BOOL	bRetVal;

	bRetVal = MsFileVerify(pFilePath, bMsTrust);
	bRetVal = FileSigVerify(pFilePath, bSigTrust);

	return bRetVal;
}

//	验证数字签名, 如果根不受信任也会不信任
BOOL	CFileTrustVerify::FileSigVerify(CHAR *pFilePath, BOOL &bSigTrust)
{
	WINTRUST_DATA			WinTrustData;
	WINTRUST_FILE_INFO		FileData;
	WCHAR					szFilePath[MAXPATH];
	GUID					WVTPolicyGUID = WINTRUST_ACTION_GENERIC_VERIFY_V2;
	LONG					lRetVal;

	bSigTrust = FALSE;

	if(InitTrustApi() == FALSE)
		return FALSE;

	NStr::Char2WChar(pFilePath, szFilePath, sizeof(szFilePath)/sizeof(WCHAR));

	memset(&FileData, 0, sizeof(FileData));
	FileData.cbStruct = sizeof(FileData);
	FileData.pcwszFilePath = szFilePath;

	memset(&WinTrustData, 0, sizeof(WinTrustData));
	WinTrustData.cbStruct = sizeof(WinTrustData);
	WinTrustData.pFile = &FileData;
	WinTrustData.dwUnionChoice = WTD_CHOICE_FILE;
	WinTrustData.dwUIChoice = WTD_UI_NONE;
	WinTrustData.fdwRevocationChecks = WTD_REVOKE_NONE;
	WinTrustData.dwStateAction = 3;
	WinTrustData.dwProvFlags = WTD_REVOCATION_CHECK_NONE;   

	lRetVal = _WinVerifyTrust(NULL, &WVTPolicyGUID, &WinTrustData);
	if(SUCCEEDED(lRetVal))
		bSigTrust = TRUE;

	return TRUE;
}

//	微软自身的效验
BOOL	CFileTrustVerify::MsFileVerify(CHAR *pFilePath, BOOL &bMsTrust)
{
	HCATADMIN		hCatAdmin;
	BOOL			bRetVal;
	BYTE *			pHashValue;
	DWORD			cbHash;
	BOOL			bResult;
	HCATINFO		pInfo;

	bMsTrust = FALSE;
	hCatAdmin = NULL;
	bResult = FALSE;
	pHashValue = NULL;

	if(InitTrustApi() == FALSE)
		return FALSE;

	bRetVal = _CryptCATAdminAcquireContext(&hCatAdmin, NULL, 0);
	if(bRetVal == FALSE || hCatAdmin == NULL)
		goto FileSigVerify_Done;

	bRetVal = HashFileData(pFilePath, &cbHash, &pHashValue);
	if(bRetVal == FALSE)
		goto FileSigVerify_Done;

	bResult = TRUE;
	pInfo = _CryptCATAdminEnumCatalogFromHash(hCatAdmin, pHashValue, cbHash, 0, NULL);
	if(pInfo == NULL)
		goto FileSigVerify_Done;

	_CryptCATAdminReleaseCatalogContext(hCatAdmin, pInfo, NULL);
	bMsTrust = TRUE;


FileSigVerify_Done:
	if(pHashValue)
		LocalFree(pHashValue);
	if(hCatAdmin)
        _CryptCATAdminReleaseContext(hCatAdmin, NULL);

	return bResult;
}


BOOL	CFileTrustVerify::InitTrustApi()
{
	if(g_bTrustApiInit)
		return TRUE;

	HMODULE			hMod;


	hMod = GetModuleHandleA("WINTRUST.DLL");
	if(hMod == NULL)
        hMod = LoadLibraryA("WINTRUST.DLL");
	if(hMod == NULL)
		return FALSE;

	_CryptCATAdminAcquireContext = (Fun_CryptCATAdminAcquireContext)
		GetProcAddress(hMod, "CryptCATAdminAcquireContext");
	_CryptCATAdminReleaseContext = (Fun_CryptCATAdminReleaseContext)
		GetProcAddress(hMod, "CryptCATAdminReleaseContext");
	_CryptCATAdminCalcHashFromFileHandle = (Fun_CryptCATAdminCalcHashFromFileHandle)
		GetProcAddress(hMod, "CryptCATAdminCalcHashFromFileHandle");
	_CryptCATAdminEnumCatalogFromHash = (Fun_CryptCATAdminEnumCatalogFromHash)
		GetProcAddress(hMod, "CryptCATAdminEnumCatalogFromHash");
	_CryptCATAdminReleaseCatalogContext = (Fun_CryptCATAdminReleaseCatalogContext)
		GetProcAddress(hMod, "CryptCATAdminReleaseCatalogContext");
	_WinVerifyTrust = (Fun_WinVerifyTrust)
		GetProcAddress(hMod, "WinVerifyTrust");

	if(_CryptCATAdminAcquireContext == NULL ||
		_CryptCATAdminReleaseContext == NULL ||
		_CryptCATAdminCalcHashFromFileHandle == NULL ||
		_CryptCATAdminEnumCatalogFromHash == NULL ||
		_CryptCATAdminReleaseCatalogContext == NULL ||
		_WinVerifyTrust == NULL)
	{
		return FALSE;
	}

	g_bTrustApiInit = TRUE;
	return TRUE;
}


BOOL	CFileTrustVerify::HashFileData(CHAR *pFilePath, DWORD *pcbHash, BYTE **ppHashValue)
{
	HANDLE				hFile;
	BYTE *				pHashValue;
	DWORD				cbHash;
	BOOL				bRetVal;
	int					nError;

	hFile = CreateFileA(pFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, 
		OPEN_EXISTING, 0, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	cbHash = 100;
	nError = ERROR_SUCCESS;

	for(pHashValue = (BYTE *)LocalAlloc(LMEM_ZEROINIT, cbHash);
		pHashValue;

		LocalFree(pHashValue), 
		pHashValue = (BYTE *)LocalAlloc(LMEM_ZEROINIT, cbHash))
	{
		bRetVal = _CryptCATAdminCalcHashFromFileHandle(hFile, &cbHash, pHashValue, 0);
		if(bRetVal)
		{
			CloseHandle(hFile);
			*pcbHash = cbHash;
			*ppHashValue = pHashValue;
			return TRUE;
		}

		nError = GetLastError();
		if(nError == 0)
			nError = ERROR_INVALID_DATA;

		if(nError != ERROR_INSUFFICIENT_BUFFER)
			break;
	}

	CloseHandle(hFile);
	LocalFree(pHashValue);
	*pcbHash = 0;
	*ppHashValue = NULL;
	return FALSE;
}



DWORD WINAPI CFileInfoMgr::TrustCheckProc(LPVOID lpThreadParameter)
{
	CFileInfoMgr *		pThis;
	CHAR				szFilePath[MAXPATH];
	BOOL				bRetVal;
	OneFileinfo			FileInfo;

	pThis = (CFileInfoMgr *)lpThreadParameter;

	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_LOWEST);
	for(pThis->m_bExit = FALSE;
		pThis->m_bExit == FALSE;
		Sleep(1))
	{
		bRetVal = pThis->GetUnCheckedItem(szFilePath, &FileInfo);
		if(bRetVal == FALSE)
			continue;

		pThis->RealGetInfoByFlag(szFilePath, &FileInfo, pThis->m_dwReCheckFlag);

		FileInfo.m_dwFalgChecked = FileInfo.m_dwFalg;
		pThis->SaveReCheckItem(szFilePath, &FileInfo);
	}

	return 0;
}


CFileInfoMgr::CFileInfoMgr()
{
	m_bCacheCheck = FALSE;
	m_dwInfoFlag = FileInfo_Flag_Base;
	m_dwReCheckFlag = FileInfo_Flag_Default;
	m_bFinish = TRUE;

	m_hTrustCheckThread = NULL;
	NSys::ThreadOneceRun(TrustCheckProc, m_hTrustCheckThread, this);
}


CFileInfoMgr::~CFileInfoMgr()
{
	m_bExit = TRUE;
	WaitForSingleObject(m_hTrustCheckThread, INFINITE);
	CloseHandle(m_hTrustCheckThread);
}


CFileInfoMgr *	g_pFileInfoMgr = NULL;

void __cdecl DestoryFileInfoMgr(void)
{
// 	NSys::UnRegisterProcessObject("CFileInfoMgr");
	delete g_pFileInfoMgr;
	g_pFileInfoMgr = NULL;
}


CFileInfoMgr * CFileInfoMgr::CreateInstance()
{
// 	BOOL				bRetVal;
// 
// 	bRetVal = NSys::GetRegistedProcessObject("CFileInfoMgr", &g_pFileInfoMgr);
// 	if(bRetVal)
// 	{
// 		return g_pFileInfoMgr;
// 	}
// 
// 	g_pFileInfoMgr = new CFileInfoMgr();
// 	NSys::RegisterProcessObject("CFileInfoMgr", &g_pFileInfoMgr);
// 	atexit(DestoryFileInfoMgr);

	if(g_pFileInfoMgr == NULL)
	{
		g_pFileInfoMgr = new CFileInfoMgr();
		atexit(DestoryFileInfoMgr);
	}

	return g_pFileInfoMgr;
}


BOOL	CFileInfoMgr::GetFileInfo(CHAR *pFilePath, OneFileinfo *pInfo)
{
	OneFileinfo *	pCacheInfo;

	pCacheInfo = GetFileInfo(pFilePath);
	if(pCacheInfo == NULL)
		return FALSE;

	*pInfo = *pCacheInfo;

	return TRUE;
}


BOOL	CFileInfoMgr::SetCheckFlag(DWORD dwInfoFlag, DWORD dwReCheckFlage)
{
	m_dwInfoFlag = dwInfoFlag;
	m_dwReCheckFlag = dwReCheckFlage;
	return TRUE;
}


BOOL	CFileInfoMgr::IsFinish()
{
	return m_bFinish;
}

BOOL	CFileInfoMgr::SetCacheCheck(BOOL bCheck)
{
	m_bCacheCheck = bCheck;
	return TRUE;
}


OneFileinfo *	CFileInfoMgr::GetFileInfo(CHAR *pFilePath)
{
	BOOL			bRetVal;
	OneFileinfo *	pFileInfo;
	OneFileinfo		RealInfo;

	if(pFilePath[0] == 0)
		return NULL;

	pFileInfo = FindInfoInCache(pFilePath);
	if(pFileInfo)
		return pFileInfo;

	bRetVal = RealGetFileInfo(pFilePath, &RealInfo);
	if(bRetVal == FALSE)
		return NULL;

	pFileInfo = SaveInfoInCache(pFilePath, &RealInfo);

	return pFileInfo;
}


OneFileinfo *	CFileInfoMgr::GetFileInfo(WCHAR *pFilePath)
{
	CHAR		szFilePath[MAXPATH];

	NStr::WChar2Char(pFilePath, szFilePath, sizeof(szFilePath));
	return GetFileInfo(szFilePath);
}


BOOL	CFileInfoMgr::RealGetFileInfo(CHAR *pFilePath, OneFileinfo *pInfo)
{
	BOOL				bRetVal;
	CHAR				szRealPath[MAXPATH];

	m_bFinish = FALSE;

	pInfo->m_dwFalg = m_dwInfoFlag | m_dwReCheckFlag;
	pInfo->m_dwFalgChecked = 0;
	if(m_dwInfoFlag == 0)
		return TRUE;

	ConvertPath(pFilePath, szRealPath);
	bRetVal = RealGetInfoByFlag(pFilePath, pInfo, m_dwInfoFlag);

	return TRUE;
}


BOOL	CheckNeed(DWORD dwHas, DWORD dwNeed, DWORD dwTest)
{
	if(dwHas & dwTest)
		return FALSE;
	if((dwNeed & dwTest) == 0)
		return FALSE;

	return TRUE;
}


BOOL	CFileInfoMgr::RealGetInfoByFlag(CHAR *pFilePath, OneFileinfo *pInfo, DWORD dwFlage)
{
	BOOL		bRetVal;

	if(CheckNeed(pInfo->m_dwFalgChecked, dwFlage, FileInfo_Flag_Base))
	{
		bRetVal = NFile::GetFileInfo(pFilePath, &pInfo->m_ByHandle);
		if(bRetVal == FALSE)
			bRetVal = NFile::GetFileInfoByFind(pFilePath, &pInfo->m_ByHandle);
		if(bRetVal == FALSE)
			return FALSE;

		pInfo->m_dwFalgChecked |= FileInfo_Flag_Base;
	}

	if(CheckNeed(pInfo->m_dwFalgChecked, dwFlage, FileInfo_Flag_Icon))
	{
		pInfo->m_hFileIcon = m_IconCache.GetFileIcon(pFilePath);
		pInfo->m_dwFalgChecked |= FileInfo_Flag_Icon;
	}

	if(CheckNeed(pInfo->m_dwFalgChecked, dwFlage, FileInfo_Flag_Pe) ||
		CheckNeed(pInfo->m_dwFalgChecked, dwFlage, FileInfo_Flag_Version) )
	{
		RealGetFilePeInfo(pFilePath, pInfo);
		pInfo->m_dwFalg |= FileInfo_Flag_Pe;
		pInfo->m_dwFalgChecked |= FileInfo_Flag_Pe;
	}

	if(CheckNeed(pInfo->m_dwFalgChecked, dwFlage, FileInfo_Flag_Version))
	{
		if(pInfo->m_bExecute)
			NFile::GetFileVersions(pFilePath, &pInfo->m_VerInfo);
		pInfo->m_dwFalgChecked |= FileInfo_Flag_Version;
	}

	if(CheckNeed(pInfo->m_dwFalgChecked, dwFlage, FileInfo_Flag_Trust) )
	{
		RealGetTrustInfo(pFilePath, pInfo);
		pInfo->m_dwFalgChecked |= FileInfo_Flag_Trust;
	}

	return TRUE;
}


BOOL	CFileInfoMgr::RealGetFilePeInfo(CHAR *pFilePath, OneFileinfo *pInfo)
{
	HANDLE			hFile;
	BOOL			bRetVal;
	DWORD			dwRead;
	BYTE			bzData[4096];
	CHAR			szRealPath[MAXPATH];

	IMAGE_DOS_HEADER *			pDosHead;
	IMAGE_NT_HEADERS32 *		pNtHead;

	pInfo->m_bExecute = FALSE;

	NFile::FormatFilePath(pFilePath, szRealPath);

	hFile = CreateFileA(szRealPath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, 
		NULL, OPEN_EXISTING, 0, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	dwRead = 0;
	memset(bzData, 0, sizeof(bzData));
	bRetVal = ReadFile(hFile, bzData, sizeof(bzData), &dwRead, NULL);
	if(bzData == FALSE)
	{
		CloseHandle(hFile);
		return FALSE;
	}

	CloseHandle(hFile);

	pDosHead = (IMAGE_DOS_HEADER *)bzData;
	pNtHead = (IMAGE_NT_HEADERS *)(bzData + pDosHead->e_lfanew);

	if(pDosHead->e_magic != IMAGE_DOS_SIGNATURE ||
		(DWORD)pDosHead->e_lfanew > dwRead)	//MZ
		return FALSE;

	pInfo->m_bExecute = TRUE;
	pInfo->m_DosHead = *pDosHead;

	if(pNtHead->FileHeader.SizeOfOptionalHeader != sizeof(IMAGE_OPTIONAL_HEADER32))
		return FALSE;
	if(pNtHead->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR32_MAGIC)
		return FALSE;
	if(pNtHead->FileHeader.NumberOfSections < 1)
		return FALSE;

	pInfo->m_NtHead = *pNtHead;

	return TRUE;
}


BOOL	CFileInfoMgr::RealGetTrustInfo(CHAR *pFilePath, OneFileinfo *pInfo)
{
	CFileTrustVerify::VerifyOneFile(pFilePath, pInfo->m_bMsTrust, pInfo->m_bSigTrust);
	return TRUE;
}

OneFileinfo *	CFileInfoMgr::FindInfoInCache(CHAR *pFilePath)
{
	string_NoCase			sFilePath;
	OneFileinfo *			pCacheInfo;
	BOOL					bRetVal;
	CFunThreadLock			FunLock(&m_CacheLock);
	BY_HANDLE_FILE_INFORMATION		ByHandle;
	std::map<string_NoCase, OneFileinfo>::iterator		Iter;

	sFilePath = pFilePath;

	Iter = m_InfoMap.find(sFilePath);
	if(Iter == m_InfoMap.end())
		return NULL;

	pCacheInfo = &(Iter->second);

	if(m_bCacheCheck == FALSE)
        goto FindInfoInCacheDone;

	bRetVal = NFile::GetFileInfo(pFilePath, &ByHandle);
	if(bRetVal == FALSE)
		return NULL;
	if(ByHandle.nFileSizeLow != pCacheInfo->m_ByHandle.nFileSizeLow)		//	比较文件大小
		return NULL;
	if(memcmp(&ByHandle.ftLastWriteTime, &pCacheInfo->m_ByHandle.ftLastWriteTime, sizeof(FILETIME)) != 0)	//	比较文件时间
		return NULL;

FindInfoInCacheDone:
	CheckCatchFileInfo(pFilePath, pCacheInfo);

	return pCacheInfo;
}


OneFileinfo *	CFileInfoMgr::SaveInfoInCache(CHAR *pFilePath, OneFileinfo *pInfo)
{
	string_NoCase		sFilePath;
	OneFileinfo *		pCacheInfo;
	CFunThreadLock		FunLock(&m_CacheLock);

	sFilePath = pFilePath;
	pCacheInfo = &m_InfoMap[sFilePath];
	*pCacheInfo = *pInfo;

	return pCacheInfo;
}


BOOL	CFileInfoMgr::ClearCache()
{
	CFunThreadLock		FunLock(&m_CacheLock);
	m_InfoMap.clear();
	return TRUE;
}

BOOL	CFileInfoMgr::ConvertPath(CHAR *pFilePath, CHAR *pRealPath)
{
	strcpy(pRealPath, pFilePath);
	if(pFilePath[0] == 0)
		return FALSE;

	if(NStr::StrHeadCmp(pFilePath, "\\??\\") == 0)
		NStr::StrReplace(pRealPath, "\\??\\", "");

	if(NStr::StrHeadCmp(pFilePath, "\\SystemRoot") == 0)
	{
		CHAR		szWinDir[MAXPATH];

		NSys::GetSystemPath(1, szWinDir, sizeof(szWinDir));
		NStr::StrReplace(pRealPath, "\\SystemRoot", szWinDir);
	}

	return TRUE;
}

BOOL	CFileInfoMgr::CheckCatchFileInfo(CHAR *pFilePath, OneFileinfo *pCatchInfo)
{
	CHAR				szRealPath[MAXPATH];

	ConvertPath(pFilePath, szRealPath);

	if(pCatchInfo->m_hFileIcon == NULL)
        pCatchInfo->m_hFileIcon = m_IconCache.GetFileIcon(szRealPath);

	return TRUE;
}


BOOL	CFileInfoMgr::GetUnCheckedItem(CHAR *pFilePath, OneFileinfo *pNewInfo)
{
	std::map<string_NoCase, OneFileinfo>::iterator		Iter;
	OneFileinfo *		pItem;
	CFunThreadLock		FunLock(&m_CacheLock);

	for(Iter = m_InfoMap.begin();
		Iter != m_InfoMap.end();
		Iter ++)
	{
		pItem = &Iter->second;

		if(pItem->m_dwFalg == pItem->m_dwFalgChecked)
			continue;

		strcpy(pFilePath, Iter->first.c_str());
		*pNewInfo = Iter->second;
		return TRUE;
	}

	m_bFinish = TRUE;
	return FALSE;
}


BOOL	CFileInfoMgr::SaveReCheckItem(CHAR *pFilePath, OneFileinfo *pNewInfo)
{
	CFunThreadLock		FunLock(&m_CacheLock);
	OneFileinfo *		pFileInfo;

	pFileInfo = FindInfoInCache(pFilePath);
	if(pFileInfo == NULL)
		return FALSE;

	*pFileInfo = *pNewInfo;
	return TRUE;
}




#pragma once


#ifndef WINVER
#define WINVER 0x0500
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif						

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0500
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0500
#endif

#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>

#include <memory>
#include <map>
#include <vector>
#include <list>
#include <string>

#include <ComDef.h>
#include <WinInet.h>
#include <WinSvc.h>

#include "NSimple.h"

#define QString_InitSize	1024

class QString
{
public:
	QString();
	~QString();


	BOOL	Append(VOID *pData, DWORD dwSize = 0);
	BOOL	AppendFmt(CONST CHAR * lpszFormat, ...);
	BOOL	Clear();
	CHAR	*Get();
	size_t	Len();

	QString& operator += (CHAR *pString);

	QString& operator = (CHAR *pString);

	operator LPSTR() const throw();

	BOOL	ReAlloc(size_t dwNewLen);

protected:
	BOOL	Alloc(size_t dwSize = 0);

	int		WC2MCNeedLen(WCHAR *pWString);
	int		WC2MC(WCHAR *pWString, CHAR *pMString, int nBufLen);

	CHAR		*m_pData;
	CHAR		*m_pCurPos;
	size_t		m_dwAllocSize;
};


//	线程
class	CThreadLock
{
public:
	CThreadLock();
	virtual ~CThreadLock();

	VOID	Lock();
	VOID	UnLock();

	CRITICAL_SECTION		m_Lock;
};



class CFunThreadLock : public CThreadLock
{
public:
	CFunThreadLock(CThreadLock *pLock);
	CFunThreadLock(CRITICAL_SECTION *pCs);
	virtual ~CFunThreadLock();

	CRITICAL_SECTION *		m_pCs;
};



//	线程
class CMutexLock
{
public:
	CMutexLock(CHAR *pName = NULL)
	{
		if(pName)
			Create(pName);
		else
			m_hMutex = NULL;
	}
	~CMutexLock()
	{
		if(m_hMutex)
            CloseHandle(m_hMutex);
	}

	BOOL	Create(LPCSTR lpName = NULL, BOOL bInitialOwner = FALSE, LPSECURITY_ATTRIBUTES lpMutexAttributes = NULL)
	{
		m_hMutex = CreateMutexA(lpMutexAttributes, bInitialOwner, lpName);
		return TRUE;
	}

	BOOL	Lock()
	{
		DWORD		dwRetVal;
		dwRetVal = ::WaitForSingleObject(m_hMutex, INFINITE);
		if(dwRetVal == WAIT_ABANDONED )
		{
			ATLTRACE("Warning: abandoned!\n");
		}

		return TRUE;
	}

	BOOL	UnLock()
	{
		ReleaseMutex(m_hMutex);
	}

	HANDLE		m_hMutex;
};


//	任何
class	CEventLock
{
public:

	CEventLock(CHAR *pName = NULL)
	{
		if(pName)
			Create(pName);
		else
			m_hEvent = NULL;
	}
	~CEventLock()
	{
		if(m_hEvent)
			CloseHandle(m_hEvent);
	}

	BOOL	Create(LPCSTR lpName = NULL, BOOL bInitial = FALSE, 
		LPSECURITY_ATTRIBUTES lpMutexAttributes = NULL, BOOL bManualReset = FALSE)
	{
		if(m_hEvent)
			CloseHandle(m_hEvent);

		m_hEvent = CreateEventA(lpMutexAttributes, bManualReset, bInitial, lpName);
		return TRUE;
	}

	BOOL	Lock(DWORD dwMilliseconds = INFINITE)
	{
		DWORD		dwRetVal;
		dwRetVal = ::WaitForSingleObject(m_hEvent, dwMilliseconds);
		if(dwRetVal == WAIT_ABANDONED )
		{
			ATLTRACE("Warning: abandoned!\n");
		}

		if(dwRetVal == WAIT_TIMEOUT)
		{
			return FALSE;
		}

		return TRUE;
	}

	BOOL	OnlyLock()
	{
		return	ResetEvent(m_hEvent);
	}

	BOOL	UnLock()
	{
		return SetEvent(m_hEvent);
	}

	HANDLE		m_hEvent;
};


class CFiofo
{
public:
	CFiofo(DWORD dwSize = 1024*1024);
	~CFiofo();

	BOOL	Read(VOID *pData, DWORD dwSize, DWORD *pdwReadSize = NULL);
	BOOL	Write(VOID *pData, DWORD dwSize, DWORD *pdwWriteSize = NULL);

protected:
	BOOL	ReAlloc();
	BOOL	Init(DWORD dwSize);

	VOID	Lock();
	VOID	UnLock();


	CRITICAL_SECTION	m_DataLock;
	BYTE		*m_pData;
	DWORD		m_dwBufferSize;
	DWORD		m_dwDataSize;

};




class CMemBlock
{
public:
	struct	BlockItem
	{
		DWORD	dwItemUniqId;
		int		nDataSize;
		BYTE	bzData[ANY_SIZE];
	};

	struct	BlockData
	{
		int			nTotalSize;
		int			nItems;
		BlockItem	Items[ANY_SIZE];
	};

	CMemBlock();
	~CMemBlock();

	BOOL	Free();
	BOOL	Init(int nInitSize);
	BOOL	Attach(VOID *pMem, int nMemSize, BOOL bClear = TRUE);
	BOOL	DeAttach();
	int		GetItemFullSize(BlockItem *pItem);
	BlockItem *	GetNext(BlockItem *	pItem);
	BlockItem *	GetItem(int nIndex);
	int		GetItemCount();
	BOOL	AddItem(VOID *pData, int nSize = -1, DWORD *pUniqId = NULL);
	BOOL	DeleteHead();
	BOOL	PopItem(VOID *pData, int nSize, DWORD *pUniqId = NULL);



	BOOL	ConditionPopItem(VOID *pData, int nSize, DWORD *pUniqId = NULL);

	BlockData *		m_pData;
	int				m_nAllocSize;
	DWORD			m_dwUniqId;
};



class CMemMapChannel
{
public:

#define CMemMapChannel_Block_Size		(1*1024*1024)
#define CMemMapChannel_Reply_Buf_Size	(1*1024*1024)
#define CMemMapChannel_ITEM_SIZE		(4096)
#define CMemMapChannel_Map_Size			(CMemMapChannel_Block_Size + CMemMapChannel_Reply_Buf_Size)

#define Send_Type_Post		0
#define Send_Type_Query		1

	typedef	struct	_OneItem
	{
		DWORD	dwDatSize;
		DWORD	dwUniqId;
		BYTE	bzData[ANY_SIZE];
	}OneItem;

	typedef struct	_MapData
	{
		DWORD		dwTotalSize;
		DWORD		dwItems;
		OneItem		Items[ANY_SIZE];
	}MapData;

	CMemMapChannel(CHAR *pChannelName = NULL);
	virtual ~CMemMapChannel();

	BOOL	Close();

	BOOL	Create(CHAR *pChannelName);
	BOOL	Open(CHAR *pChannelName);

	BOOL	CreateChannelName(CHAR *pRealName, CHAR *pTypeName, CHAR *pMyName, BOOL bGlobal = FALSE);
	BOOL	Init(CHAR *pChannelName, BOOL bOpen = FALSE);

	virtual	VOID OnCommnad(CHAR *pString, DWORD dwType, DWORD dwUniqId);

	static DWORD	WINAPI ListenProc(VOID *pParam);

	BOOL	StartListen();
	BOOL	StopListen();

	BOOL	Write(VOID *pData, int nSize = -1, DWORD dwType = Send_Type_Post, DWORD *pReplyId = NULL);
	BOOL	Read(VOID *pData, int nSize, DWORD *pdwType = NULL, DWORD *pReplyId = NULL);
	BOOL	WriteReply(DWORD dwReplyId, VOID *pData, int nSize = -1);
	BOOL	ReadReply(DWORD dwReplyId, VOID *pData, int nSize);


	BOOL	GetReplyById(DWORD dwReplyId, VOID *pData, int nSize);


	BOOL	Recv(VOID *pData, int nSize);

	BOOL	Send(VOID *pData, int nSize = -1);
	BOOL	Query(VOID *pRecv, int nRcvSize, VOID *pQuery, int nSize = -1);



	HANDLE			m_DataMap;
	HANDLE			m_hListenThread;
	VOID *			m_pMapView;
	CMemBlock		m_MemBlock;
	CEventLock		m_QueueLock;

	DWORD			m_nQueryTime;
	OneItem *		m_pReplyBuffer;
	CEventLock		m_ReplyLockRead;
	CEventLock		m_ReplyLockAcs;	//使用两个Event为了 读写快速响应
	DWORD			m_dwReplyDropTime;


	BOOL			m_bStop;
	BOOL			m_bInit;
};



class CMalloc
{
public:

	CMalloc();
	CMalloc(CHAR *pStr);

	~CMalloc();

	BOOL	MallocByStr(CHAR *pStr, int nMemLen = 0);
	BOOL	MallocByStr(WCHAR *pStr, int nMemLen = 0);
	BOOL	Free();

	//	need free
	static	CHAR	*MallocNewStr(CHAR *pString, int nMemLen = 0);

	VOID	*m_pData;
};


interface IShellLinkA;
interface IPersistFile;

class CFileLink
{
public:
	CFileLink();
	virtual ~CFileLink();


	BOOL	OpenLinkFile(CHAR *pFileName);
	BOOL	GetLinkInfo(CHAR *pDestPath, CHAR * pArgs = NULL, int *pnShowCmd = NULL, CHAR *pWorkDir = NULL);


	BOOL	CreateLink(CHAR *pLinkFile);
	BOOL	SetLinkInfo(CHAR *pDestPath, CHAR * pArgs = NULL, int nShowCmd = SW_SHOWNORMAL, CHAR *pWorkDir = NULL);
	BOOL	SetLinkOtherInfo(CHAR *pDescription, CHAR *pIconFilePath = NULL, int nIconIndex = 0, WORD SetHotkey = -1);
	BOOL	Save();


	IShellLinkA *		m_pLink;
	IPersistFile *		m_pPstFile;
	_bstr_t				m_bStrLinkFile;
};


class CRegClsid
{
public:
	CRegClsid(CHAR *pClsId = NULL);
	~CRegClsid();


	BOOL	GetClsidInfo(CHAR *pRegPath, CHAR *pObjName, CHAR *pObjectPath, CHAR *pClsId = NULL);

	BOOL	GetClsidInfo();

	CHAR		m_szClsid[128];
	CHAR		m_szRegPath[MAXPATH];
	CHAR		m_szObjName[MAXPATH];
	CHAR		m_szObjPath[MAXPATH];
};


template<class _Ty>
class ThreadSafeVal: public CThreadLock
{
public:
	ThreadSafeVal()
	{
	}

	ThreadSafeVal(_Ty Init)
	{
		m_Value = Init;
	}

	virtual ~ThreadSafeVal()
	{
	}

	ThreadSafeVal& operator=(_Ty Right)
	{
		Lock();
		m_Value = Right;
		UnLock();

		return (*this);
	}

	operator const _Ty()
	{
		return m_Value;
	}

	_Ty				m_Value;
};



class CHttpClient
{
public:
	CHttpClient();
	virtual	~CHttpClient();




	enum 
	{ 
		Prgs_Type_Read = 0,
		Prgs_Type_Write = 1
	};

	struct HttpReadProcDat 
	{
		BYTE *		pBuffer;
		int			nBufSize;
		int			nReadSize;
	};

	virtual	VOID	OnProgress(int nType, int nCurSize, int nTotalSize);
	virtual	BOOL	DownloadOneFile(CHAR *pUrl, CHAR *pSaveFile);


	VOID	Close();
	BOOL	StopAcion();
	int		GetError();

	BOOL	HttpOpenUrl(CHAR *pUrl);

	//	PUT	GET	POST 
	BOOL	MethodRequest(CHAR *pUrl, CHAR *pMethod, BYTE *pBuffer, int nBufSize);

	typedef BOOL (WINAPI *Fun_Read_Proc)(BYTE *pBuffer, int nSize, VOID * pParam);
	DWORD	HttpFullRead(Fun_Read_Proc fProc, VOID *pParam, int nMaxSize = -1);

	static	BOOL CALLBACK ReadToBufferProc(BYTE *pBuffer, int nSize, VOID * pParam);
	DWORD	HttpFullRead(BYTE *pBuffer, int nBufSize);

	static	BOOL CALLBACK ReadToFileProc(BYTE *pBuffer, int nSize, VOID * pParam);
	DWORD	HttpFullRead(HANDLE hFile, int nMaxSize = -1);


	DWORD	HttpGetFileSize();
	BOOL	HttpGetFileTime(SYSTEMTIME *pSt);


	//	需要free	NULL则失败
	BYTE *	GetOneFile(CHAR *pUrl, DWORD *pdwSize = NULL);


	BOOL	Get_Data(CHAR *pUrl);
	BOOL	Post_Data(CHAR *pUrl, BYTE *pBuffer, int nBufSize, BYTE *pRecv, int nRecvSize = 0);
	BOOL	Put_Data(CHAR *pUrl, BYTE *pBuffer, int nBufSize, BYTE *pRecv, int nRecvSize = 0);


	ThreadSafeVal<BOOL>		m_bStop;

	DWORD		m_dwOneSize;
	BYTE *		m_pOneBuffer;
	DWORD		m_dwError;

	HINTERNET	m_hInter;
	HINTERNET	m_hConnect;
	HINTERNET	m_Request;
};



class	CFileDownload
{
public:
	CFileDownload();
	virtual ~CFileDownload();
	virtual VOID	OnDownloading(DWORD dwDoneSize, DWORD dwTotalSize);

	BOOL	DownloadOneFile(CHAR *pUrl, CHAR *pSaveFile);
	BOOL	StopDownload(BOOL bStop = TRUE);
	VOID	Close();

	int		UrlRequest(CHAR *pUrl, BYTE *pBuffer, int nBufSize, BYTE *pRequest, int nReqSize = 0);

	ThreadSafeVal<BOOL>		m_bStop;

	DWORD		m_dwTotalSize;
	DWORD		m_dwDownloadSize;
	DWORD		m_dwOneSize;
	BYTE *		m_pBuffer;
	DWORD		m_dwError;


	HINTERNET	m_hInter;
	HINTERNET	m_hConnect;
	HINTERNET	m_Request;
};


#define		WM_KeyEvent		WM_USER + 0x1000

class CKeyEventEng;

typedef BOOL (WINAPI *KEYNTYPROC)(CHAR *pBuffer, VOID *pUserParam, WPARAM wParam, LPARAM lParam);


class CKeyEventEng
{
public:
	typedef struct _Nty_Wnd_Data
	{
		HWND			hWnd;
		UINT			uMsg;
		CHAR *			pBuffer;
	}Nty_Wnd_Data;

	//	不要直接定义对象	用 StartKeyEnentEng
	CKeyEventEng();
	~CKeyEventEng();

	HMODULE	GetModuleFromAddress(VOID * Address);

	static	LRESULT CALLBACK KeyEventProc(int nCode, WPARAM wParam, LPARAM lParam);

	BOOL	Start();
	BOOL	Stop();


	BOOL	OnKeyEvent(CHAR *pKeyText, WPARAM wParam, LPARAM lParam);
	BOOL	NotityProc(CHAR *pKeyText, WPARAM wParam, LPARAM lParam);
	BOOL	NotityWnd(CHAR *pKeyText);


	//	函数返回FALSE则屏蔽改键
	BOOL	AddNtyProc(KEYNTYPROC fnProc, VOID * pParam);
	BOOL	DelNtyProc(KEYNTYPROC fnProc);

	//	缓冲NULL	SendMessage
	//	非NULL		PostMessage
	BOOL	AddNtyWnd(HWND hWnd, UINT uMsg = WM_KeyEvent, CHAR *pBuffer = NULL);
	BOOL	DelNtyWnd(HWND hWnd);


	std::map<KEYNTYPROC, VOID *>		m_NtyFun;
	std::vector<Nty_Wnd_Data>			m_NtyWnd;

	HHOOK			m_hHook;
	CHAR			m_szBuffer[1024];
};






class CKeyEventAss;
extern	CKeyEventAss	*	g_pOneKeyEventAss;
#define KEYEVENTF_KEYDOWN		0


CKeyEventAss *	StartKeyEventAss();
BOOL			StopKeyEnentAss();

class CKeyEventAss
{
	struct KEYDATA
	{
		BYTE	bVk;
		int		nFlags;
		DWORD	dwSleep;
	};


public:
	CKeyEventAss();
	virtual ~CKeyEventAss();

	//	设置间隔掩饰时间
	BOOL	SetDefaultSleep(DWORD dwSleep = 10);

	BOOL	InitKeyMap();
	BOOL	GetVirtualKey(CHAR *pString, UINT* puVk, BOOL* pbExtend);


	//	nFlags  0: 按下  
	//	KEYEVENTF_KEYUP: 弹上  
	//	KEYEVENTF_EXTENDEDKEY: 扩展键
	BOOL	AddKeyEvent(BYTE bVk, BOOL bDown, DWORD dwSleep = -1);

	BOOL	PressOneKey(CHAR *pKeyString, DWORD dwSleep = -1);
	DWORD	PressKeys(CHAR *pKeyString, DWORD dwSleep = -1);

	static	DWORD	CALLBACK  ThreadProc(VOID * pParam);


	HANDLE		m_hThread;
	BOOL		m_bExit;

	DWORD		m_dwSleep;

	std::map<std::string, DWORD>	m_KeyMap;
	std::list<KEYDATA>				m_KeyData;

	CThreadLock	m_Lock;
};


class CFileSmallIconMgr
{
protected:
	CFileSmallIconMgr();
public:
	~CFileSmallIconMgr();
	static CFileSmallIconMgr * CreateInstance();

	virtual	HICON	GetFileIcon(CHAR *pFileName);

	BOOL		Clear();
	BOOL		ConvertPath(CHAR *pFilePath, std::string &RealPath);
	HICON		FindFileIcon(CHAR *pFilePath);
	HICON		SaveFileIcon(CHAR *pFilePath);

	std::map<std::string, HICON>		m_IconMap;
};



class CServiceBase
{
public:
	CServiceBase();
	~CServiceBase();


	virtual	VOID	OnError(CHAR *pText, int nError);
	virtual	VOID	OnEventLog(CHAR *pText, int nError = 0);
	virtual	VOID	OnServiceMainAction();
	virtual	VOID	OnActionWithStation();

	virtual	VOID	OnCtrlServer(DWORD fdwControl);
	virtual	VOID	OnStopServer();
	virtual	VOID	OnPauseServer(BOOL bResume = FALSE);

	virtual	BOOL	CreateServer();

	BOOL	ReportEventLog(int nEventId, CHAR * pFormat, ...);
	
	BOOL	InitConfig(CHAR *pServerName, CHAR *pDisplayName = NULL, DWORD dwStartType = SERVICE_AUTO_START, 
		DWORD dwServerType = SERVICE_INTERACTIVE_PROCESS | SERVICE_WIN32_OWN_PROCESS);



	BOOL	StartServer();
	BOOL	PauseServer(BOOL bResume = FALSE);
	BOOL	GetServerStat(DWORD &dwStat);
	BOOL	StopServer();

	BOOL	ChangeServiceConfigs();
	BOOL	ChangeServiceStatus(DWORD dwState, DWORD dwError = 0, DWORD dwWaitHint = 0);
	BOOL	CtrlServiceStart();
	BOOL	SetServerError(int nError);

	BOOL	RunActionWithStation();

	static VOID WINAPI	ServiceMain(DWORD dwArgc,LPSTR *lpszArgv);
	static VOID WINAPI	ServerHandlerProc(DWORD fdwControl);


	SERVICE_STATUS_HANDLE	m_hServiceStatus;
	SERVICE_STATUS			m_ServiceStatus;
	DWORD					m_dwCheckPoint;
	CHAR					m_szServiceName[MAXPATH];
	CHAR					m_szDisplayName[MAXPATH];
	DWORD					m_dwStartType;
	DWORD					m_dwServerType;
	BOOL					m_bStop;
	int						m_nLastError;
};


class CDiskFile
{
public:
	CDiskFile();
	~CDiskFile();

//	virtual	void	OnEnumFileCluster();

	typedef struct _VOLUME_LOGICAL_OFFSET {
		LONGLONG  LogicalOffset;
	} VOLUME_LOGICAL_OFFSET, *PVOLUME_LOGICAL_OFFSET;

	typedef struct _VOLUME_PHYSICAL_OFFSET {
		ULONG  DiskNumber;
		LONGLONG  Offset;
	} VOLUME_PHYSICAL_OFFSET, *PVOLUME_PHYSICAL_OFFSET;

	typedef struct _VOLUME_PHYSICAL_OFFSETS {
		ULONG  NumberOfPhysicalOffsets;
		VOLUME_PHYSICAL_OFFSET  PhysicalOffset[1];
	} VOLUME_PHYSICAL_OFFSETS, *PVOLUME_PHYSICAL_OFFSETS;

	#pragma pack(1)
	typedef struct _Fat32BootData
	{
		BYTE jump[3];				//	0x00 Boot strap short or near jump
		char bsOemName[8];			//	0x03 Name - can be used to special case partition manager volumes
		WORD BytesPerSector;		//	0x0B bytes per logical sector
		BYTE SectorsPerCluster;		//	0x0D sectors/cluster
		WORD ReservedSectors;		//	0x0E reserved sectors
		BYTE NumberOfFATs;			//	0x10 number of FATs
		WORD RootEntries;			//	0x11 root directory entries
		WORD TotalSectors;			//	0x13 number of sectors
		BYTE MediaDescriptor;		//	0x15 media code (unused)
		WORD SectorsPerFAT;			//	0x16 sectors/FAT	FAT16
		WORD SectorsPerTrack;		//	0x18 sectors per track
		WORD Heads;					//	0x1A number of heads
		DWORD HiddenSectors;		//	0x1C hidden sectors (unused)
		DWORD BigTotalSectors;		//	0x20 number of sectors (if sectors == 0)
//	The following fields are only used by FAT32
		DWORD BigSectorsPerFat;		//	0x24 sectors/FAT	FAT32
		WORD ExtFlags;				//	0x28 bit 8: fat mirroring, low 4: active fat
		WORD FS_Version;			//	0x2A major, minor filesystem version
		DWORD RootDirStrtClus;		//	0x2C first cluster in root directory
		WORD FSInfoSec;				//	0x30 filesystem info sector
		WORD BkUpBootSec;			//	0x32 backup boot sector
		WORD Reserved[6];			//	0x34 Unused
		BYTE bsDriveNumber;			//	0x40
		BYTE bsReserved;			//	0x41
		BYTE bsBootSignature;		//	0x42
		DWORD bsVolumeID;			//	0x43
		char bsVolumeLabel[11];		//	0x47
		char bsFileSysType[8];		//	0x52
		BYTE unused [422];			//	0x5A
	}Fat32BootData;
#pragma pack()

	#define IOCTL_VOLUME_QUERY_VOLUME_NUMBER        CTL_CODE(IOCTL_VOLUME_BASE, 7, METHOD_BUFFERED, FILE_ANY_ACCESS) 
	#define IOCTL_VOLUME_LOGICAL_TO_PHYSICAL        CTL_CODE(IOCTL_VOLUME_BASE, 8, METHOD_BUFFERED, FILE_ANY_ACCESS) 
	#define IOCTL_VOLUME_PHYSICAL_TO_LOGICAL        CTL_CODE(IOCTL_VOLUME_BASE, 9, METHOD_BUFFERED, FILE_ANY_ACCESS) 
	#define IOCTL_VOLUME_IS_PARTITION               CTL_CODE(IOCTL_VOLUME_BASE, 10, METHOD_BUFFERED, FILE_ANY_ACCESS) 
	#define IOCTL_VOLUME_READ_PLEX                  CTL_CODE(IOCTL_VOLUME_BASE, 11, METHOD_OUT_DIRECT, FILE_READ_ACCESS) 


	typedef struct _FileCluster
	{
		LARGE_INTEGER	llOffsetVcn;
		LARGE_INTEGER	llOffsetLcn;
		LARGE_INTEGER	llLength;

		LARGE_INTEGER	llFirstSector;
	}FileCluster;


	BOOL	Close();
	BOOL	OpenFile(CHAR *pFilePath = NULL);
	BOOL	CheckOpen();

	BOOL	GetDiskOtherInfo();
	BOOL	CheckSuppert();

	BOOL	GetFileCluster();
	BOOL	GetSectorByCluster(LARGE_INTEGER llLcn, LARGE_INTEGER &llFirstSector);

	BOOL	ReadDiskSector(LARGE_INTEGER llFirstSector, LARGE_INTEGER llCount, BYTE *pBuf);
	BOOL	ReadDiskCluster(LARGE_INTEGER llFirstCluster, LARGE_INTEGER llCount, BYTE *pBuf);

	BOOL	WriteDiskSector(LARGE_INTEGER llFirstSector, LARGE_INTEGER llCount, BYTE *pBuf);



	//	nType:
	//	0:	Volume
	//	1:	PhysicalDrive
	BYTE *	GetFileDiskData(CHAR *pFileName, DWORD * pdwSize = NULL, int nType = 1);
	BYTE *	GetFileDiskData(DWORD *pdwReadSize, int nType = 1);
	BOOL	GetFileDiskData(std::vector<BYTE *> &vData, int nType = 1);

	BOOL	OpenDirectory(CHAR *pFileName);

	BOOL	TestFileData(CHAR *pFileName, int nType = 1);
	BOOL	InitInfo(CHAR *pFilePath);


	CHAR			m_szFilePath[MAXPATH];
	CHAR			m_szVolume[MAXPATH];
	CHAR			m_szFullVolume[MAXPATH];

	CHAR			m_szDiskPath[MAXPATH];

	DWORD			m_dwVolumeFileSystemFlags;
	CHAR			m_szVolumeFileSystemName[MAXPATH];
	int				m_nVolumeType;
	BOOL			m_bOpen;

	HANDLE			m_hFile;

	BOOL			m_bInit;
	HANDLE			m_hVolume;
	HANDLE			m_hDisk;
	DWORD			m_dwBytesPerSector;
	DWORD			m_dwSectorsPerCluster;
	DWORD			m_dwNumberOfFreeClusters;
	DWORD			m_dwTotalNumberOfClusters;
	DWORD			m_dwFileSize;
	DWORD			m_dwFat32FirstDataSector;

	DWORD			m_dwError;

	std::vector<FileCluster>	m_FileCluster;
};



// TEMPLATE CLASS tsList		Thread Safe
template<class _Ty, class _Ax = std::allocator<_Ty> >
class tsList
	: public std::list<_Ty, _Ax>
{
public:

	CThreadLock	m_Lock;

	void push_front(const _Ty& _Val)
	{	// insert element at beginning
		m_Lock.Lock();
		__super::push_front(_Val);
		m_Lock.UnLock();
	}

	void pop_front()
	{	// erase element at beginning
		m_Lock.Lock();
		__super::pop_front();
		m_Lock.UnLock();
	}

	void push_back(const _Ty& _Val)
	{	// insert element at end
		m_Lock.Lock();
		__super::push_back(_Val);
		m_Lock.UnLock();
	}

	void pop_back()
	{	// erase element at end
		m_Lock.Lock();
		__super::pop_back();
		m_Lock.UnLock();
	}

};


class CTcpBase
{
public:
	CTcpBase();
	virtual	~CTcpBase();

	virtual void	OnRecvSizedDataPrgs(SOCKET sock, int nRecvSize, int nTotalSize);
	virtual void	OnSendDataPrgs(SOCKET sock, int nSendSize, int nTotalSize);


	BOOL	Init();
	BOOL	Clear();
	BOOL	Bind(int nPort, BOOL bReUser = FALSE);
	BOOL	Connect(DWORD dwIp, int nPort);
	BOOL	Connect(CHAR *pHost, int nPort);
	BOOL	ConnectTimed(CHAR *pHost, int nPort);
	BOOL	SetTimeOut(int nConnect, int nSend = -1, int nRecv = -1);
	BOOL	Attach(SOCKET sock, BOOL bTemp = FALSE);
	BOOL	DisConnect();

	BOOL	Send(VOID *pData, int nSize = -1);
	BOOL	RecvOneLine(VOID *pData, int nBufSize);
	int		RecvOne(VOID *pData, int nBufSize);
	BOOL	RecvSizedData(VOID *pData, int nDataSize);

	BOOL	Send(SOCKET sock, VOID *pData, int nSize = -1);
	BOOL	RecvOneLine(SOCKET sock, VOID *pData, int nBufSize);
	int		RecvOne(SOCKET sock, VOID *pData, int nBufSize);
	BOOL	RecvSizedData(SOCKET sock, VOID *pData, int nDataSize);


	SOCKET		m_sock;
	int			m_nConnectTime;
	int			m_nSendTime;
	int			m_nRecvTime;
	BOOL		m_bTempAttach;
};



//	阻塞模式
class CBlockTcp: public CTcpBase
{
public:
	CBlockTcp();
	virtual ~CBlockTcp();

	BOOL	Query(CHAR *pQuery, CHAR *pRecv, int nBufSize);

};


//	封装包大小, 防止粘包
class CPckTcp: public CTcpBase
{
public:
	CPckTcp();
	virtual ~CPckTcp();

	BOOL	Pck_Send(VOID *pData, int nSize);
	BOOL	Pck_Recv(VOID *pBuffer, int nBufSize);

};


class CAsyncTcp: public CTcpBase
{
public:
	CAsyncTcp();
	virtual ~CAsyncTcp();

//	
	virtual	void	OnSvrAccept(SOCKET sConSock, sockaddr_in *pClientAddr);
	virtual	void	OnSvrRecvData(SOCKET sConSock, CHAR *pBuffer, int nSize);
	virtual	void	OnSvrRecvError(SOCKET sConSock, int nError);
	virtual	void	OnSvrDisconnect();

	virtual	void	OnCltRecvData(CHAR *pBuffer, int nSize);
	virtual void	OnRecvSizedDataPrgs(SOCKET sock, int nRecvSize, int nTotalSize);

	BOOL	SetPort(int nPort);
	BOOL	Listen(int nPort = -1);

	static	 DWORD	CALLBACK	QuickListenProc(VOID *pParam);

	BOOL	QuickListenRun(int nPort);
	BOOL	StopListen(BOOL bWait = TRUE);


	static	 DWORD	CALLBACK	CltRecvThread(VOID *pParam);
	BOOL	CreateCltRecvThread();
	BOOL	QuickClientRun(CHAR *pHost, int nPort);
	BOOL	ClientDisconnect();

	SOCKET		m_CurClient;
	sockaddr_in	m_CurClinetAddr;
	int			m_nPort;
	HANDLE		m_hListenThread;
	BOOL		m_bListenStop;
	HANDLE		m_hClientRecvThread;
};



class CAppIoHandle
{
public:
	CAppIoHandle();
	virtual ~CAppIoHandle();

	virtual	void	OnAppOutput(VOID *pBuffer, int nSize);
	virtual	void	OnAppOutput(CHAR *pString);
	virtual	void	OnAppExit();


	static	DWORD CALLBACK	GetOutputProc(VOID *pParam);

	BOOL	CrateConsole();

	BOOL	StartShell(CHAR *pCmdLine);
	BOOL	InputString(CHAR *pCmdString);
	DWORD	ReadOutput(CHAR *pBuffer, DWORD dwSize);
	BOOL	Exit();


	BOOL		m_bStop;
	BOOL		m_bHide;
	HANDLE		m_hCmdProc;
	DWORD		m_dwCmdPid;
	HANDLE		m_hInput;
	HANDLE		m_hOutput;
	HANDLE		m_hGetProc;

	HANDLE		m_hConIn;
	HANDLE		m_hConOut;
};



class CHttpUpdate
{
public: 
	typedef struct _UpdateItem
	{
		std::string		sFileName;
		FILETIME		ft;
		DWORD			dwFileSize;
		BOOL			bDownLoad;
	}UpdateItem;


	CHttpUpdate();
	~CHttpUpdate();

	virtual	BOOL	CheckRealUrl();
	virtual BOOL	ParseListFile();
	virtual BOOL	OnFixFileName(UpdateItem *pItem, std::string &sFileName);
	virtual	BOOL	OnUpdateFileOK(UpdateItem *pItem, int nCurItem, int nTotalCount, BOOL bDownload);
	virtual	BOOL	OnDownloadFailed(CHAR *pUrlPath, CHAR *SavePath, int nError);
	virtual	VOID	OnUpdateEnd(int nError);
	virtual	BOOL	CopyOneFile(CHAR *pSorDir, CHAR *pDestDir, CHAR *pName);


	BOOL	SetUpdateUrl(CHAR *pUpdateUrl, CHAR *pSavePath = NULL, CHAR *pCheckPath = NULL, CHAR *pListFile = "FileList.txt");
	virtual  BOOL	StartUpdate(BOOL bNewThread = TRUE);
	BOOL	StopUpdate(BOOL bWait = TRUE);


	BOOL	DownloadOneFile(CHAR *pUrl, CHAR *pSavePath);
	BOOL	CheckUpdateNeed(UpdateItem *pItem);
	BOOL	CopyUpdateFiles();

	CHttpClient		m_http;
	std::string		m_sUpdateUrl;
	std::string		m_sSavePath;
	std::string		m_sCheckPath;
	std::string		m_sRealUrl;
	int				m_nLastError;
	HANDLE			m_UpdateThread;

	std::string		m_sListFile;
	int				m_nRetryCount;
	DWORD			m_dwMaxFileTimeDlt;
	BOOL			m_bStop;

	std::vector<UpdateItem>		m_UpdateList;
};



class CDynDriver
{
public:
	CDynDriver(CHAR *pFilePath = NULL);
	~CDynDriver();

	virtual	BOOL	OnReadData(VOID *pData, DWORD dwDatSize);

	BOOL	Init(CHAR *pSvrName);

	BOOL	Install(CHAR *pFilePath, CHAR *pSvrName = NULL);
	BOOL	InstallByResId();
	BOOL	OpenDevice(CHAR *pDeviceName = NULL, DWORD dwDesiredAccess = GENERIC_WRITE | GENERIC_READ);
	BOOL	DriverCtrl(DWORD dwIndex, VOID *pInBuffer, DWORD dwInSize, 
		VOID *pOutBuffer, DWORD dwOutBuf, DWORD *pdwRead);

	BOOL	CloseDevice();

	BOOL	StopDriver();
	BOOL	StartDriver();

	BOOL	UnInstall();
	DWORD	SycRead(VOID *pBuffer, int nBufSize);
	BOOL	StopRead();


	HANDLE		m_hDriver;
	HANDLE		m_hSycEvent;
	CHAR		m_szDriverPath[MAXPATH];
	CHAR		m_szDeviceName[MAXPATH];
	CHAR		m_szSvrName[MAXPATH];
};






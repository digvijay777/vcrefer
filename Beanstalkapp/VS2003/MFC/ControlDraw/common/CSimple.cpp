#include "CSimple.h"

#include <ShObjIdl.h>
#include <ShlGuid.h>
#include <assert.h>
#include <atltrace.h>
#include "NSimple.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////
//									QString
////////////////////////////////////////////////////////////////////////////////////////////////////////

QString::QString()
{
	m_pData = NULL;
	m_pCurPos = NULL;
	m_dwAllocSize = 0;

	Alloc();
}

QString::~QString()
{
	if(m_pData)
		free(m_pData);
}


BOOL	QString::Alloc(size_t dwSize)
{
	assert(m_pData == NULL);

	if(dwSize == 0)
		dwSize = QString_InitSize;

	m_pData = (CHAR *)malloc(dwSize);
	memset(m_pData, 0, dwSize);

	m_pCurPos = m_pData;
	m_dwAllocSize = dwSize;

	return TRUE;
}


BOOL	QString::ReAlloc(size_t dwNewLen)
{
	size_t		dwRealAllocLen;
	CHAR		*pNewData;

	for(dwRealAllocLen = m_dwAllocSize;
		dwNewLen >= dwRealAllocLen;
		dwRealAllocLen *= 2);

	pNewData = (CHAR *)malloc(dwRealAllocLen);
	memset(pNewData, 0, dwRealAllocLen);

	memcpy(pNewData, m_pData, m_dwAllocSize);
	free(m_pData);

	m_pCurPos = pNewData + (m_pCurPos - m_pData);
	m_pData = pNewData;
	m_dwAllocSize = dwRealAllocLen;


	return TRUE;
}


BOOL	QString::Clear()
{
	memset(m_pData, 0, m_pCurPos-m_pData);

	m_pCurPos = m_pData;
	return TRUE;
}


CHAR	*QString::Get()
{
	return m_pData;
}


size_t	QString::Len()
{
	return (m_pCurPos - m_pData);
}

BOOL	QString::Append(VOID *pData, DWORD dwSize)
{
	DWORD		dwNewLen;

	if(pData == NULL)
		return FALSE;

	if(dwSize == 0)
        dwSize = (DWORD)strlen((CHAR *)pData);

	if(m_pCurPos+dwSize >= m_pData+m_dwAllocSize)
	{
		dwNewLen = (DWORD)(m_pCurPos+dwSize - m_pData);
		ReAlloc(dwNewLen);
	}

	memcpy(m_pCurPos, pData, dwSize);
	m_pCurPos += dwSize;

	return TRUE;
}

QString& QString::operator += (CHAR *pString)
{
	Append(pString);

	return *this;
}

QString& QString::operator = (CHAR *pString)
{
	Clear();
	Append(pString);
	return *this;
}


QString::operator LPSTR() const throw()
{
	return m_pData;
}

BOOL	QString::AppendFmt(CONST CHAR * lpszFormat, ...)
{
	CHAR *				buf = NULL;
	int					nRetVal = -1, nSize = 4096;
	va_list				args;

	if(lpszFormat == NULL)
		return FALSE;

	va_start(args, lpszFormat);

	for(; nRetVal<0 ; nSize*=2)
	{
		if(buf)	
			free(buf);

		buf = (CHAR *)malloc( (nSize+2)*sizeof(CHAR) );
		nRetVal = _vsnprintf(buf, nSize, lpszFormat, args);
	}

	va_end(args);

	Append(buf);

	if(buf)	
		free(buf);
	return nRetVal;
}



int		QString::WC2MCNeedLen(WCHAR *pWString)
{
	return ::WideCharToMultiByte(CP_THREAD_ACP, 0, pWString, -1, NULL, 0, NULL, NULL) - 1;
}


int		QString::WC2MC(WCHAR *pWString, CHAR *pMString, int nBufLen)
{
	return ::WideCharToMultiByte(CP_THREAD_ACP, 0, pWString, -1, pMString, nBufLen, NULL, NULL);
}









//public:
CFiofo::CFiofo(DWORD dwSize)
{
	Init(dwSize);
}

CFiofo::~CFiofo()
{
	DeleteCriticalSection(&m_DataLock);
	delete [] m_pData;
}


BOOL	CFiofo::Init(DWORD dwSize)
{
	InitializeCriticalSection(&m_DataLock);

	m_pData = new BYTE[dwSize];
	if(m_pData == NULL)
		return FALSE;

	m_dwBufferSize = dwSize;
	m_dwDataSize = 0;
	return TRUE;
}


BOOL	CFiofo::Read(VOID *pData, DWORD dwSize, DWORD *pdwReadSize)
{
	DWORD			dwReadSize;
	DWORD			dwRemainSize;

	Lock();

	if(m_dwDataSize == 0)
	{
		dwReadSize = 0;
	}
	else if(dwSize >= m_dwDataSize)
	{
		memcpy(pData, m_pData, m_dwDataSize);
		dwReadSize = m_dwDataSize;

		NLog::DbgOutput("Read All: %d", m_dwDataSize);
		m_dwDataSize = 0;
	}
	else
	{
		memcpy(pData, m_pData, dwSize);
		dwReadSize = dwSize;

		NLog::DbgOutput("Read Part: %d", m_dwDataSize);

		dwRemainSize = m_dwDataSize - dwSize;
		memmove(m_pData, m_pData+dwSize, dwRemainSize);
		m_dwDataSize = dwRemainSize;
	}

	if(pdwReadSize)
		*pdwReadSize = dwReadSize;

	UnLock();
	return TRUE;

}

BOOL	CFiofo::Write(VOID *pData, DWORD dwSize, DWORD *pdwWriteSize)
{
	DWORD			dwWriteSize;
	Lock();

	if(m_dwDataSize+dwSize > m_dwBufferSize)
	{
		dwWriteSize = 0;
		NLog::DbgOutput("Write Full: %d", m_dwDataSize);
	}
	else
	{
		memcpy(m_pData+m_dwDataSize, pData, dwSize);
		m_dwDataSize += dwSize;

		NLog::DbgOutput("Write : %d", m_dwDataSize);
		dwWriteSize = dwSize;
	}
	if(pdwWriteSize)
		*pdwWriteSize = dwWriteSize;

	UnLock();
	return TRUE;
}


//protected:
BOOL	CFiofo::ReAlloc()
{
	return FALSE;
}





VOID	CFiofo::Lock()
{
	EnterCriticalSection(&m_DataLock);
}

VOID	CFiofo::UnLock()
{
	LeaveCriticalSection(&m_DataLock);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
//	CMemBlock

CMemBlock::CMemBlock()
{
	m_pData = NULL;
	m_nAllocSize = 0;
	m_dwUniqId = 0;
}

CMemBlock::~CMemBlock()
{
}

BOOL	CMemBlock::Free()
{
	if(m_pData)
	{
		free(m_pData);
		m_pData = NULL;
		m_nAllocSize = 0;
	}

	return TRUE;
}

BOOL	CMemBlock::Init(int nInitSize)
{
	m_pData = (BlockData *)malloc(nInitSize);
	m_nAllocSize = nInitSize;
	memset(m_pData, 0, nInitSize);
	return TRUE;
}

BOOL	CMemBlock::Attach(VOID *pMem, int nMemSize, BOOL bClear)
{
	Free();
	m_pData = (BlockData *)pMem;
	m_nAllocSize = nMemSize;

	if(bClear)
		memset(m_pData, 0, m_nAllocSize);
	return TRUE;
}


BOOL	CMemBlock::DeAttach()
{
	m_pData = NULL;
	m_nAllocSize = 0;
	return TRUE;
}


int		CMemBlock::GetItemFullSize(BlockItem *pItem)
{
	int		nFullSize;

	nFullSize = sizeof(BlockItem);
	nFullSize += pItem->nDataSize;

	return nFullSize;
}

CMemBlock::BlockItem *	CMemBlock::GetNext(BlockItem *	pItem)
{
	BYTE *		pData;
	int			nNeedSize;

	pData = (BYTE *)pItem;
	pData += GetItemFullSize(pItem);

	nNeedSize = (int)(pData - (BYTE *)m_pData);
	if(nNeedSize >= m_nAllocSize)
		return NULL;

	return (BlockItem *)pData;
}

CMemBlock::BlockItem *	CMemBlock::GetItem(int nIndex)
{
	int			i;
	BlockItem *	pCurItem;


	if(m_pData == NULL)
		return NULL;
	if(nIndex >= m_pData->nItems)
		return NULL;

	pCurItem = &m_pData->Items[0];
	for(i=0; i<nIndex; i++)
	{
		pCurItem = GetNext(pCurItem);
		if(pCurItem == NULL)
			return NULL;
	}

	return pCurItem;
}

int		CMemBlock::GetItemCount()
{
	return m_pData->nItems;
}


BOOL	CMemBlock::AddItem(VOID *pData, int nSize, DWORD *pUniqId)
{
	int			nNeedSize;
	BlockItem *	pCurItem;
	int			nIndex;

	if(m_pData == NULL)
		return FALSE;

	if(nSize == -1)
		nSize = (int)(strlen((CHAR *)pData) + 1);

	nNeedSize = m_pData->nTotalSize + nSize + sizeof(BlockData);
	if(nNeedSize >= m_nAllocSize)
		return FALSE;

	nIndex = m_pData->nItems;
	m_pData->nItems ++;
	pCurItem = GetItem(nIndex);
	if(pCurItem == NULL)
	{
		assert(0);
	}

	memcpy(pCurItem->bzData, pData, nSize);
	pCurItem->nDataSize = nSize;
	m_pData->nTotalSize += GetItemFullSize(pCurItem);
	pCurItem->dwItemUniqId = m_dwUniqId++;

	if(pUniqId)
        *pUniqId = pCurItem->dwItemUniqId;
	return TRUE;
}


BOOL	CMemBlock::DeleteHead()
{
	BlockItem *		pHead;
	BlockItem *		pNewHead;
	int				nMoveSize;

	if(m_pData->nItems == 1)
	{
		memset(m_pData, 0, m_nAllocSize);
		return TRUE;
	}

	pHead = &m_pData->Items[0];
	pNewHead = GetNext(pHead);

	nMoveSize =	(int)((BYTE *)m_pData + m_nAllocSize - (BYTE *)pNewHead);
	m_pData->nItems --;
	m_pData->nTotalSize -= GetItemFullSize(pHead);
	memmove(pHead, pNewHead, nMoveSize);

	return TRUE;
}

BOOL	CMemBlock::PopItem(VOID *pData, int nSize, DWORD *pUniqId)
{
	int			nDatSize;

	if(m_pData == NULL)
		return FALSE;

	if(m_pData->nItems <= 0)
		return FALSE;

	nDatSize = m_pData->Items[0].nDataSize;
	if(nDatSize > nSize)
		return FALSE;

	memcpy(pData, m_pData->Items[0].bzData, nDatSize);
	if(pUniqId)
        *pUniqId = m_pData->Items[0].dwItemUniqId;
	DeleteHead();

	return TRUE;
}

//		CMemBlock
/////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////
//		CMemMapChannel



CMemMapChannel::CMemMapChannel(CHAR *pChannelName)
{
	m_nQueryTime = 1000;		//获取数据超时时间
	m_dwReplyDropTime = 400;	//数据无人使用丢弃时间

	m_bInit = FALSE;
	m_pMapView = NULL;
	m_DataMap = NULL;
	m_hListenThread = NULL;

	if(pChannelName)
        Init(pChannelName);
}


CMemMapChannel::~CMemMapChannel()
{
	Close();
}

BOOL	CMemMapChannel::Close()
{
	StopListen();

	m_MemBlock.DeAttach();

	if(m_pMapView)
		UnmapViewOfFile(m_pMapView);

	if(m_DataMap)
		CloseHandle(m_DataMap);

	if(m_hListenThread)
		CloseHandle(m_hListenThread);

	return TRUE;
}


BOOL	CMemMapChannel::CreateChannelName(CHAR *pRealName, CHAR *pTypeName, CHAR *pMyName, BOOL bGlobal)
{
	if(bGlobal == FALSE)
		sprintf(pRealName, "CMemMapChannel_%s_%s", pTypeName, pMyName);
	else
		sprintf(pRealName, "Global\\CMemMapChannel_%s_%s", pTypeName, pMyName);

	return TRUE;
}


BOOL	CMemMapChannel::Init(CHAR *pChannelName, BOOL bOpen)
{
	CHAR		szLockName[1024];
	CHAR		szFileMapName[1024];
	SECURITY_ATTRIBUTES			SectAttr;
	SECURITY_DESCRIPTOR			SectDescrip;
	BOOL		bFirst;

	if(m_bInit)
		return TRUE;

	InitializeSecurityDescriptor(&SectDescrip, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(&SectDescrip, TRUE, NULL, FALSE);
	SectAttr.lpSecurityDescriptor = &SectDescrip;
	SectAttr.bInheritHandle = TRUE;
	SectAttr.nLength = sizeof(SectAttr);


	if(NSys::GetOsMajorVersion() >= 6)
	{
		NAcl::SetSecurityDescriptorByString((SECURITY_DESCRIPTOR *)SectAttr.lpSecurityDescriptor);
	}
	else
	{
		SetSecurityDescriptorDacl(&SectDescrip, TRUE, NULL, FALSE);
	}

	if(pChannelName == NULL)
		pChannelName = "NULL";

	CreateChannelName(szFileMapName, "Map", pChannelName);
	if(bOpen)
	{
		m_DataMap = OpenFileMappingA(FILE_MAP_ALL_ACCESS, 0, szFileMapName);
		if(m_DataMap == NULL)
		{
			CreateChannelName(szFileMapName, "Map", pChannelName, TRUE);
			m_DataMap = OpenFileMappingA(FILE_MAP_ALL_ACCESS, 0, szFileMapName);
		}
	}
	else
	{
		m_DataMap = CreateFileMappingA(INVALID_HANDLE_VALUE, &SectAttr, 
			PAGE_READWRITE, 0, CMemMapChannel_Map_Size, szFileMapName);
		if(m_DataMap == NULL)
			assert(0);
	}
	if(m_DataMap == NULL)
		return FALSE;

	bFirst = !bOpen;

	CreateChannelName(szLockName, "Mutex", pChannelName);
	m_QueueLock.Create(szLockName, TRUE, &SectAttr);
	CreateChannelName(szLockName, "ReplyRead", pChannelName);
	m_ReplyLockRead.Create(szLockName, FALSE, &SectAttr);
	CreateChannelName(szLockName, "ReplyWrite", pChannelName);
	m_ReplyLockAcs.Create(szLockName, TRUE, &SectAttr);

	m_pMapView = MapViewOfFile(m_DataMap, FILE_MAP_WRITE, 0, 0, CMemMapChannel_Map_Size);
	if(m_pMapView == NULL)
        return FALSE;

	m_MemBlock.Attach(m_pMapView, CMemMapChannel_Block_Size, bFirst);

	m_pReplyBuffer = (OneItem * )((BYTE *)m_pMapView + CMemMapChannel_Block_Size);

	m_hListenThread = NULL;
	m_bStop = TRUE;
	m_bInit = TRUE;

	return TRUE;
}


BOOL	CMemMapChannel::Create(CHAR *pChannelName)
{
	BOOL	bRetVal;

	bRetVal = Init(pChannelName);

	return bRetVal;
}

BOOL	CMemMapChannel::Open(CHAR *pChannelName)
{
	BOOL	bRetVal;

	bRetVal = Init(pChannelName, TRUE);

	return bRetVal;
}


VOID CMemMapChannel::OnCommnad(CHAR *pString, DWORD dwType, DWORD dwUniqId)
{
	ATLTRACE("CMemMapChannel::OnCommnad %s\r\n", pString);

	if(dwType == Send_Type_Post)
		return;

	if(stricmp(pString, "GetName") == 0)
	{
		WriteReply(dwUniqId, "CMemMapChannel");
	}
	if(stricmp(pString, "GetHost") == 0)
	{
		CHAR		szName[100];
		NSys::GetProcNameFromId(GetCurrentProcessId(), szName);
		WriteReply(dwUniqId, szName);
	}
}


DWORD	WINAPI CMemMapChannel::ListenProc(VOID *pParam)
{
	CMemMapChannel *	pThis;
	CHAR				szData[CMemMapChannel_ITEM_SIZE];
	BOOL				bRetVal;
	DWORD				dwType;
	DWORD				dwUniqId;

	pThis = (CMemMapChannel *)pParam;

	memset(szData, 0, sizeof(szData));
	for(; pThis->m_bStop==FALSE; )
	{
		bRetVal = pThis->Read(szData, sizeof(szData), &dwType, &dwUniqId);
		if(bRetVal == FALSE)
		{
			Sleep(1);
			continue;
		}

		pThis->OnCommnad(szData, dwType, dwUniqId);
		memset(szData, 0, sizeof(szData));
	}
	return 0;
}


BOOL	CMemMapChannel::Write(VOID *pData, int nSize, DWORD dwType, DWORD *pReplyId)
{
	BOOL		bRetVal;
	BYTE *		pNewBuffer;

	if(m_bInit == FALSE)
		return FALSE;

	bRetVal = m_QueueLock.Lock();
	if(bRetVal == FALSE)
		return FALSE;

	if(nSize == -1)
		nSize = (int)strlen((CHAR *)pData) + 1;

	pNewBuffer = (BYTE *)malloc(nSize+sizeof(dwType));
	if(pNewBuffer == NULL)
	{
		assert(0);
		return FALSE;
	}

	memcpy(pNewBuffer, &dwType, sizeof(dwType));
	memcpy(pNewBuffer+sizeof(dwType), pData, nSize);

	bRetVal = m_MemBlock.AddItem(pNewBuffer, nSize+sizeof(dwType), pReplyId);
	m_QueueLock.UnLock();

	free(pNewBuffer);
	return bRetVal;
}


BOOL	CMemMapChannel::Read(VOID *pData, int nSize, DWORD *pdwType, DWORD *pReplyId)
{
	BOOL		bRetVal;

	DWORD		dwType;
	CHAR		szData[CMemMapChannel_ITEM_SIZE];

	if(m_bInit == FALSE)		//没初始化
		return FALSE;

	bRetVal = m_QueueLock.Lock();
	if(bRetVal == FALSE)
		return FALSE;

	bRetVal = m_MemBlock.PopItem(szData, sizeof(szData), pReplyId);
	if(bRetVal == FALSE)		//缓冲太小或没数据
	{
		m_QueueLock.UnLock();
		return FALSE;
	}

	memcpy(&dwType, szData, sizeof(dwType));
	if(pdwType)
		*pdwType = dwType;
	memcpy(pData, szData+sizeof(dwType), nSize);

	m_QueueLock.UnLock();
	return bRetVal;
}


BOOL	CMemMapChannel::WriteReply(DWORD dwReplyId, VOID *pData, int nSize)
{
	BOOL	bRetVal;
	DWORD	dwTkCur;
	DWORD	dwTkEnd;

	if(m_bInit == FALSE)
		return FALSE;

	dwTkCur = GetTickCount();
	dwTkEnd = dwTkCur + m_dwReplyDropTime;
	for(;;)
	{
		bRetVal = m_ReplyLockAcs.Lock();

		dwTkCur = GetTickCount();
		if(m_pReplyBuffer->dwDatSize == 0)
			break;
		if(dwTkCur > dwTkEnd)
		{
			NSys::Trace("CMemMapChannel Drop: %s\n", m_pReplyBuffer->bzData);
			break;
		}
		m_ReplyLockAcs.UnLock();

		Sleep(1);
	}

	if(nSize == -1)
		nSize = (int)strlen((CHAR *)pData) + 1;
	m_pReplyBuffer->dwDatSize = nSize;
	m_pReplyBuffer->dwUniqId = dwReplyId;
	memcpy(m_pReplyBuffer->bzData, pData, nSize);
	ATLTRACE("WriteReply: %s\n", pData);

	m_ReplyLockAcs.UnLock();
	m_ReplyLockRead.UnLock();

	return TRUE;
}


BOOL	CMemMapChannel::GetReplyById(DWORD dwReplyId, VOID *pData, int nSize)
{
	BOOL		bRetVal;

	if(m_bInit == FALSE)
		return FALSE;

	bRetVal = m_ReplyLockRead.Lock(m_nQueryTime);	//等待数据过来
	if(bRetVal == FALSE)
	{
		return FALSE;			//没等到数据, 一直没有数据
	}
	bRetVal = m_ReplyLockAcs.Lock();

	if(m_pReplyBuffer->dwDatSize == 0)			//	没数据
	{
		m_ReplyLockAcs.UnLock();
		m_ReplyLockRead.UnLock();
		return FALSE;
	}

	if(dwReplyId != m_pReplyBuffer->dwUniqId)		//数据不是自己的
	{
		m_ReplyLockAcs.UnLock();
		m_ReplyLockRead.UnLock();
		return FALSE;
	}

	if(nSize < (int)m_pReplyBuffer->dwDatSize)	//缓冲太小
	{
		m_ReplyLockAcs.UnLock();
		m_ReplyLockRead.UnLock();
		return FALSE;
	}

	memcpy(pData, m_pReplyBuffer->bzData, nSize);	// 正确
	m_pReplyBuffer->dwDatSize = 0;

	m_ReplyLockAcs.UnLock();
	return TRUE;
}


BOOL	CMemMapChannel::ReadReply(DWORD dwReplyId, VOID *pData, int nSize)
{
	BOOL		bRetVal;
	DWORD		dwCurTick;
	DWORD		dwEndCurTick;

	if(m_bInit == FALSE)
		return FALSE;

	dwCurTick = GetTickCount();
	dwEndCurTick = dwCurTick + m_nQueryTime;
	for(;
		dwCurTick < dwEndCurTick;
		dwCurTick = GetTickCount())
	{
		bRetVal = GetReplyById(dwReplyId, pData, nSize);
		if(bRetVal)
		{
			return TRUE;
		}

		Sleep(1);
	}

	return FALSE;
}


BOOL	CMemMapChannel::Query(VOID *pRecv, int nRcvSize, VOID *pQuery, int nSize)
{
	BOOL	bRetVal;
	DWORD	dwUniqId;

	if(m_bInit == FALSE)
		return FALSE;

	Write(pQuery, nSize, Send_Type_Query, &dwUniqId);

	bRetVal = ReadReply(dwUniqId, pRecv, nRcvSize);
	if(bRetVal == FALSE)
	{
	}

	return bRetVal;
}


BOOL	CMemMapChannel::Send(VOID *pData, int nSize)
{
	return Write(pData, nSize, Send_Type_Post);
}


BOOL	CMemMapChannel::Recv(VOID *pData, int nSize)
{
	return Read(pData, nSize, NULL);
}





BOOL	CMemMapChannel::StartListen()
{
	DWORD		dwExit;
	BOOL		bRetVal;
	m_bStop = FALSE;

	bRetVal = GetExitCodeThread(m_hListenThread, &dwExit);
	if(bRetVal && dwExit == STILL_ACTIVE)
		return FALSE;

	m_hListenThread = CreateThread(NULL, 0, ListenProc, this, 0, NULL);

	if(m_hListenThread == NULL)
		return FALSE;

	return TRUE;
}


BOOL	CMemMapChannel::StopListen()
{
	m_bStop = TRUE;
	WaitForSingleObject(m_hListenThread, INFINITE);

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
//					CMalloc
/////////////////////////////////////////////////////////////////////////////////////////////////////////



CMalloc::CMalloc()
{
	m_pData = NULL;
}

CMalloc::CMalloc(CHAR *pStr)
{
	m_pData = NULL;
	MallocByStr(pStr);
}


CMalloc::~CMalloc()
{
	Free();
}


BOOL	CMalloc::MallocByStr(CHAR *pStr, int nMemLen)
{
	Free();

	m_pData = MallocNewStr(pStr, nMemLen);

	return TRUE;
}

BOOL	CMalloc::MallocByStr(WCHAR *pStr, int nMemLen)
{
	Free();

	if(nMemLen == 0)
		nMemLen = (DWORD)(wcslen(pStr) + 1)*sizeof(WCHAR);

	m_pData = malloc(nMemLen);
	memcpy(m_pData, pStr, nMemLen);

	return TRUE;
}

BOOL	CMalloc::Free()
{
	if(m_pData)
	{
		free(m_pData);
		m_pData = NULL;
	}
	return TRUE;
}


CHAR	*CMalloc::MallocNewStr(CHAR *pString, int nMemLen)
{
	CHAR		*pNewString;

	if(nMemLen == 0)
		nMemLen = (DWORD)strlen(pString) + 1;

	pNewString = (CHAR *)malloc(nMemLen);
	memcpy(pNewString, pString, nMemLen);

	return pNewString;
}
//	
/////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////
//	CFileLink
CFileLink::CFileLink()
{
	m_pLink = NULL;
	m_pPstFile = NULL;
	CoInitialize(NULL);
}

CFileLink::~CFileLink()
{
	Save();

	if(m_pLink)
		m_pLink->Release();

	if(m_pPstFile)
		m_pPstFile->Release();

	CoUninitialize();
}



BOOL	CFileLink::OpenLinkFile(CHAR *pFileName)
{
	if(m_pLink)
		m_pLink->Release();

	if(m_pPstFile)
		m_pPstFile->Release();


	HRESULT			hr;

	hr = ::CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
		IID_IShellLink, (LPVOID*)&m_pLink);
	if(FAILED(hr))
		return FALSE;

	hr = m_pLink->QueryInterface(IID_IPersistFile, (LPVOID*) &m_pPstFile);
	if (FAILED(hr))
		return FALSE;

	m_bStrLinkFile = pFileName;
	m_pPstFile->Load(m_bStrLinkFile, 0);

	return TRUE;
}


BOOL	CFileLink::GetLinkInfo(CHAR *pDestPath, CHAR * pArgs, int *pnShowCmd, CHAR *pWorkDir)
{
	if(m_pLink == NULL)
		return FALSE;

	if(pDestPath)
		m_pLink->GetPath(pDestPath, MAXPATH, NULL, SLGP_UNCPRIORITY );


	return TRUE;
}


BOOL	CFileLink::CreateLink(CHAR *pLinkFile)
{
	HRESULT			hr;

	hr = ::CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
		IID_IShellLink, (LPVOID*)&m_pLink);
	if(FAILED(hr))
		return FALSE;

	hr = m_pLink->QueryInterface(IID_IPersistFile, (LPVOID*) &m_pPstFile);
	if (FAILED(hr))
		return FALSE;

	m_bStrLinkFile = pLinkFile;
	return TRUE;
}


BOOL	CFileLink::SetLinkInfo(CHAR *pDestPath, CHAR * pArgs, int nShowCmd, CHAR *pWorkDir)
{
	CHAR			szWorkDir[MAX_PATH];

	if(m_pLink == NULL)
		return FALSE;

	m_pLink->SetPath(pDestPath);

	if(pArgs)
		m_pLink->SetArguments(pArgs);

	m_pLink->SetShowCmd(nShowCmd);

	if(pWorkDir)
	{
		m_pLink->SetWorkingDirectory(pWorkDir);
	}
	else
	{
		NFile::SpliteFilePath(pDestPath, szWorkDir);
		m_pLink->SetWorkingDirectory(szWorkDir);
	}
	return TRUE;
}

BOOL	CFileLink::SetLinkOtherInfo(CHAR *pDescription, CHAR *pIconFilePath, int nIconIndex, WORD SetHotkey)
{
	if(m_pLink == NULL)
		return FALSE;

	if(pDescription)
		m_pLink->SetDescription(pDescription);

	if(pIconFilePath)
		m_pLink->SetIconLocation(pIconFilePath, nIconIndex);

	if(SetHotkey != (WORD)-1)
		m_pLink->SetHotkey(SetHotkey);

	return TRUE;
}


BOOL	CFileLink::Save()
{
	if(m_pPstFile == NULL)
		return FALSE;

	m_pPstFile->Save(m_bStrLinkFile, TRUE);
	return TRUE;
}
//	CFileLink
/////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////
//	CThreadLock

CRegClsid::CRegClsid(CHAR *pClsId)
{
	m_szClsid[0] = 0;
	m_szRegPath[0] = 0;
	m_szObjName[0] = 0;
	m_szObjPath[0] = 0;

	if(pClsId)
		NStr::StrCopy(m_szClsid, pClsId, sizeof(m_szClsid));
}

CRegClsid::~CRegClsid()
{

}


BOOL	CRegClsid::GetClsidInfo(CHAR *pRegPath, CHAR *pObjName, CHAR *pObjectPath, CHAR *pClsId)
{
	BOOL		bRetVal;

	if(pClsId)
		NStr::StrCopy(m_szClsid, pClsId, sizeof(m_szClsid));

	bRetVal = GetClsidInfo();
// 	if(bRetVal == FALSE)
// 		return FALSE;

	strcpy(pRegPath, m_szRegPath);
	strcpy(pObjName, m_szObjName);
	strcpy(pObjectPath, m_szObjPath);

	return bRetVal;
}


BOOL	CRegClsid::GetClsidInfo()
{
	BOOL		bRetVal;

	sprintf(m_szRegPath, "CLSID\\%s", m_szClsid);
	bRetVal = NReg::SHGetReg(HKEY_CLASSES_ROOT, m_szRegPath, "", m_szObjName, sizeof(m_szObjName));
	if(bRetVal == FALSE)
		return FALSE;
	
	sprintf(m_szRegPath, "CLSID\\%s\\InprocServer32", m_szClsid);
	bRetVal = NReg::SHGetReg(HKEY_CLASSES_ROOT, m_szRegPath, "", m_szObjPath, sizeof(m_szObjName));
	if(bRetVal == FALSE)
		return FALSE;

	NFile::GetFilePath(m_szObjPath, m_szObjPath, sizeof(m_szObjPath));

	return TRUE;
}




//	CThreadLock
/////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////
//	CThreadLock
CThreadLock::CThreadLock()
{
	InitializeCriticalSection(&m_Lock);
}

CThreadLock::~CThreadLock()
{
	DeleteCriticalSection(&m_Lock);
}


VOID	CThreadLock::Lock()
{
	EnterCriticalSection(&m_Lock);
}

VOID	CThreadLock::UnLock()
{
	LeaveCriticalSection(&m_Lock);
}
//	CThreadLock
/////////////////////////////////////////////////////////////////////////////////
CFunThreadLock::CFunThreadLock(CThreadLock *pLock)
{
	m_pCs = &pLock->m_Lock;
	EnterCriticalSection(m_pCs);
}

CFunThreadLock::CFunThreadLock(CRITICAL_SECTION *pCs)
{
	m_pCs = pCs;
	EnterCriticalSection(m_pCs);
}

CFunThreadLock::~CFunThreadLock()
{
	LeaveCriticalSection(m_pCs);
}

/////////////////////////////////////////////////////////////////////////////////
//	CHttpClient
CHttpClient::CHttpClient()
{
	m_dwOneSize = 4 * 1024;
	m_pOneBuffer = (BYTE *)malloc(m_dwOneSize);

	m_hInter = NULL;
	m_hConnect = NULL;
	m_Request = NULL;
	m_bStop = FALSE;
}


CHttpClient::~CHttpClient()
{
	Close();
	if(m_pOneBuffer)
	{
		free(m_pOneBuffer);
		m_pOneBuffer = NULL;
	}
}



VOID CHttpClient::OnProgress(int nType, int nCurSize, int nTotalSize)
{

}

VOID	CHttpClient::Close()
{
	if(m_hConnect)
	{
		InternetCloseHandle(m_hConnect);
		m_hConnect = NULL;
	}

	if(m_hInter)
	{
		InternetCloseHandle(m_hInter);
		m_hInter = NULL;
	}

	if(m_Request)
	{
		InternetCloseHandle(m_Request);
		m_Request = NULL;
	}

}

BOOL	CHttpClient::StopAcion()
{
	m_bStop = TRUE;
	return TRUE;
}


int		CHttpClient::GetError()
{
	return (int)m_dwError;
}

BOOL	CHttpClient::HttpOpenUrl(CHAR *pUrl)
{
	CHAR		szHead[] = "Accept: */*\r\n\r\n";
	DWORD		dwStatus;
	DWORD		dwBufSize;
	BOOL		bRetVal;

	Close();
	m_dwError = ERROR_SUCCESS;

	m_hInter = InternetOpenA("CHttpClient", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if(m_hInter == NULL)
	{
		m_dwError = GetLastError();
		goto HttpOpenUrl_Done;
	}

	m_hConnect = InternetOpenUrlA(m_hInter, pUrl, szHead, (DWORD)strlen(szHead), 
		INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_RELOAD, 0);
	if(m_hConnect == NULL)
	{
		m_dwError = GetLastError();
		goto HttpOpenUrl_Done;
	}

	dwBufSize = sizeof(dwStatus);
	bRetVal = HttpQueryInfoA(m_hConnect, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &dwStatus, &dwBufSize, NULL);
	if(bRetVal == FALSE)
	{
		m_dwError = GetLastError();
		goto HttpOpenUrl_Done;
	}
	if(dwStatus != HTTP_STATUS_OK)
	{
		m_dwError = ERROR_FILE_NOT_FOUND;
		goto HttpOpenUrl_Done;
	}

HttpOpenUrl_Done:

	if(m_dwError != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}


BOOL	CHttpClient::MethodRequest(CHAR *pUrl, CHAR *pMethod, BYTE *pBuffer, int nBufSize)
{
	BOOL		bRetVal;
	CHAR *		pHdr = 
		"Accept-Language: zh-cn\r\n"
		"Content-Type: application/x-www-form-urlencoded\r\n";

	m_dwError = ERROR_SUCCESS;
	m_Request = HttpOpenRequestA(m_hConnect, pMethod, pUrl, HTTP_VERSIONA, NULL, NULL, 
		INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_RELOAD, 0);
	if(m_Request == NULL)
	{
		m_dwError = GetLastError();
		goto MethodRequest_Done;
	}

	bRetVal = HttpSendRequestA(m_Request, pHdr, (DWORD)strlen((CONST CHAR *)pHdr), pBuffer, nBufSize);
	if(bRetVal == FALSE)
	{
		m_dwError = GetLastError();
		goto MethodRequest_Done;
	}

MethodRequest_Done:
	if(m_dwError != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}


DWORD	CHttpClient::HttpFullRead(Fun_Read_Proc fProc, VOID *pParam, int nMaxSize)
{
	DWORD		dwReadSize;
	BOOL		bRetVal;
	DWORD		dwCurSize;

	dwCurSize = 0;
	for(;;)
	{
		bRetVal = InternetReadFile(m_hConnect, m_pOneBuffer, m_dwOneSize, &dwReadSize);
		if(bRetVal == FALSE)
		{
			m_dwError = GetLastError();
			goto HttpFullRead_Done;
		}

		if(m_bStop)
			break;

		if(dwReadSize == 0)
			break;

		bRetVal = fProc(m_pOneBuffer, dwReadSize, pParam);
		if(bRetVal == FALSE)
			break;

		dwCurSize += dwReadSize;
		OnProgress(Prgs_Type_Read, (int)dwCurSize, nMaxSize);
	}

HttpFullRead_Done:
	if(m_dwError != ERROR_SUCCESS)
		return dwCurSize;

	return dwCurSize;
}



BOOL CALLBACK CHttpClient::ReadToBufferProc(BYTE *pBuffer, int nSize, VOID * pParam)
{
	HttpReadProcDat *		pDat;

	pDat = (HttpReadProcDat *)pParam;

	if(pDat->nReadSize + nSize > pDat->nBufSize)	//缓冲太小
	{
		return FALSE;
	}

	memcpy(pDat->pBuffer + pDat->nReadSize, pBuffer, nSize);
	pDat->nReadSize += nSize;

	return TRUE;
}


DWORD	CHttpClient::HttpFullRead(BYTE *pBuffer, int nBufSize)
{
	DWORD				dwRetVal;
	HttpReadProcDat		Data;

	Data.pBuffer = pBuffer;
	Data.nBufSize = nBufSize;
	Data.nReadSize = 0;

	dwRetVal = HttpFullRead(ReadToBufferProc, &Data, nBufSize);

	return dwRetVal;
}


BOOL CALLBACK CHttpClient::ReadToFileProc(BYTE *pBuffer, int nSize, VOID * pParam)
{
	HANDLE *		phFile;
	DWORD			dwWrite;
	BOOL			bRetVal;

	phFile = (HANDLE *)pParam;

	bRetVal = WriteFile(*phFile, pBuffer, nSize, &dwWrite, NULL);
	if(bRetVal == FALSE || dwWrite != nSize)
	{
		assert(0);
	}

	return TRUE;

}


DWORD	CHttpClient::HttpFullRead(HANDLE hFile, int nMaxSize)
{
	DWORD				dwRetVal;

	dwRetVal = HttpFullRead(ReadToFileProc, &hFile, nMaxSize);

	return dwRetVal;
}


DWORD	CHttpClient::HttpGetFileSize()
{
	return 0;
}


BOOL	CHttpClient::DownloadOneFile(CHAR *pUrl, CHAR *pSaveFile)
{
	BOOL		bRetVal;
	DWORD		dwRetVal;
	HANDLE		hFile;
	DWORD		dwFileSize;

	bRetVal = HttpOpenUrl(pUrl);
	if(bRetVal == FALSE)
		return FALSE;

	dwFileSize = HttpGetFileSize();

	hFile = CreateFileA(pSaveFile, GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ | FILE_SHARE_DELETE,
		NULL, CREATE_ALWAYS , 0, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
		goto DownloadOneFile_Done;

	dwRetVal = HttpFullRead(hFile, dwFileSize);

	CloseHandle(hFile);

DownloadOneFile_Done:
	if(m_dwError != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}


BOOL WINAPI GetOneFileProc(BYTE *pBuffer, int nSize, VOID * pParam)
{
	QString *		pqFileData;

	pqFileData = (QString *)pParam;
	pqFileData->Append(pBuffer, nSize);

	return TRUE;
}


BYTE *	CHttpClient::GetOneFile(CHAR *pUrl, DWORD *pdwSize)
{
	BOOL		bRetVal;
	DWORD		dwRetVal;
	QString		qFileData;
	DWORD		dwSize;
	BYTE *		pNewData;

	bRetVal = HttpOpenUrl(pUrl);
	if(bRetVal == FALSE)
		return NULL;

	dwRetVal = HttpFullRead(GetOneFileProc, &qFileData);

	dwSize = (DWORD)qFileData.Len();
	if(dwSize != dwRetVal)
		return NULL;

	if(pdwSize)
		*pdwSize = dwSize;

	pNewData = (BYTE *)malloc(dwSize+1);
	memset(pNewData, 0, dwSize+1);
	memcpy(pNewData, qFileData.Get(), qFileData.Len());

	return pNewData;
}

//	CHttpClient
/////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////
//	CFileDownload
CFileDownload::CFileDownload()
{
	m_dwOneSize = 4 * 1024;
	m_pBuffer = (BYTE *)malloc(m_dwOneSize);

	m_hInter = NULL;
	m_hConnect = NULL;
	m_bStop = FALSE;
}

CFileDownload::~CFileDownload()
{
	Close();
	if(m_pBuffer)
	{
		free(m_pBuffer);
		m_pBuffer = NULL;
	}
}

VOID	CFileDownload::OnDownloading(DWORD dwDoneSize, DWORD dwTotalSize)
{

}


BOOL	CFileDownload::StopDownload(BOOL bStop)
{
	m_bStop = bStop;
	return TRUE;
}

VOID	CFileDownload::Close()
{
	if(m_hConnect)
	{
		InternetCloseHandle(m_hConnect);
		m_hConnect = NULL;
	}

	if(m_hInter)
	{
		InternetCloseHandle(m_hInter);
		m_hInter = NULL;
	}

	if(m_Request)
	{
		InternetCloseHandle(m_Request);
		m_Request = NULL;
	}


}

BOOL	CFileDownload::DownloadOneFile(CHAR *pUrl, CHAR *pSaveFile)
{
	CHAR		szHead[] = "Accept: */*\r\n\r\n";
	BOOL		bRetVal;
	DWORD		dwBufSize;
	DWORD		dwReadSize;
	BOOL		bResult;
	DWORD		dwStatus;
	HANDLE		hFile;
	SYSTEMTIME	st;
	FILETIME	ft;


	Close();

	hFile = NULL;
	bResult = FALSE;


	m_dwError = ERROR_SUCCESS;

	m_hInter = InternetOpenA("CFileDownload", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if(m_hInter == NULL)
	{
		m_dwError = GetLastError();
		goto DownloadOneFileDone;
	}

	m_hConnect = InternetOpenUrlA(m_hInter, pUrl, szHead, (DWORD)strlen(szHead), 
		INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_RELOAD, 0);
	if(m_hConnect == NULL)
	{
		m_dwError = GetLastError();
		goto DownloadOneFileDone;
	}

	dwBufSize = sizeof(dwStatus);
	bRetVal = HttpQueryInfoA(m_hConnect, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &dwStatus, &dwBufSize, NULL);
	if(bRetVal == FALSE)
	{
		m_dwError = GetLastError();
		goto DownloadOneFileDone;
	}
	if(dwStatus != HTTP_STATUS_OK)
	{
		m_dwError = ERROR_FILE_NOT_FOUND;
		goto DownloadOneFileDone;
	}


	dwBufSize = sizeof(m_dwTotalSize);
	bRetVal = HttpQueryInfoA(m_hConnect, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, 
		&m_dwTotalSize, &dwBufSize, NULL);
	if(bRetVal == FALSE)
		m_dwTotalSize = -1;


	memset(&st, 0, sizeof(st));
	dwBufSize = sizeof(st);
	bRetVal = HttpQueryInfoA(m_hConnect, HTTP_QUERY_LAST_MODIFIED | HTTP_QUERY_FLAG_SYSTEMTIME, 
		&st, &dwBufSize, NULL);
	if(bRetVal)
	{
		SystemTimeToFileTime(&st, &ft);
	}

	hFile = CreateFileA(pSaveFile, GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ | FILE_SHARE_DELETE,
		NULL, CREATE_ALWAYS , 0, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
		goto DownloadOneFileDone;


	m_dwDownloadSize = 0;
	for(;;)
	{
		bRetVal = InternetReadFile(m_hConnect, m_pBuffer, m_dwOneSize, &dwReadSize);
		if(bRetVal == FALSE)
		{
			m_dwError = GetLastError();
			goto DownloadOneFileDone;
		}

		if(m_bStop)
		{
			bResult = TRUE;
			break;
		}

		if(dwReadSize == 0)
		{
			bResult = TRUE;
			break;
		}

		WriteFile(hFile, m_pBuffer, dwReadSize, &dwReadSize, NULL);

		m_dwDownloadSize += dwReadSize;
		OnDownloading(m_dwDownloadSize, m_dwTotalSize);
	}

	if(st.wYear != 0)
		SetFileTime(hFile, NULL, NULL, &ft);

DownloadOneFileDone:
	if(hFile)
        CloseHandle(hFile);

	return bResult;
}


int		CFileDownload::UrlRequest(CHAR *pUrl, BYTE *pBuffer, int nBufSize, BYTE *pRequest, int nReqSize)
{
	BOOL		bRetVal;
	DWORD		dwSaveSize;
	DWORD		dwReadSize;
	CHAR		szHostName[64];
	CHAR		szUrl[MAXPATH];
	DWORD		dwBufSize;
	CHAR *		pReqHead = "Accept: */*\r\n";
	CHAR *		pHdr = 
		"Accept-Language: zh-cn\r\n"
		"Content-Type: application/x-www-form-urlencoded\r\n";
//		"Accept-Encoding: gzip, deflate";

	URL_COMPONENTSA	UrlCop;


	Close();

	memset(&UrlCop, 0, sizeof(UrlCop));
	UrlCop.dwStructSize = sizeof(UrlCop);
	UrlCop.lpszHostName = szHostName;
	UrlCop.dwHostNameLength = sizeof(szHostName);
	UrlCop.lpszUrlPath = szUrl;
	UrlCop.dwUrlPathLength = sizeof(szUrl);

	InternetCrackUrlA(pUrl, 0, ICU_DECODE, &UrlCop);

	m_dwError = ERROR_SUCCESS;
	dwSaveSize = 0;

	if(nReqSize == 0)
		nReqSize = (int)strlen((CONST CHAR *)pRequest);

	m_hInter = InternetOpenA("UrlRequest", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if(m_hInter == NULL)
	{
		m_dwError = GetLastError();
		goto UrlRequest_Done;
	}

	m_hConnect = InternetConnectA(m_hInter, szHostName, UrlCop.nPort, 
		NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	if(m_hConnect == NULL)
	{
		m_dwError = GetLastError();
		goto UrlRequest_Done;
	}

//	m_Request = HttpOpenRequestA(m_hConnect, "POST", szUrl, HTTP_VERSIONA, NULL, (LPCSTR *)&pReqHead, 
	m_Request = HttpOpenRequestA(m_hConnect, "POST", szUrl, HTTP_VERSIONA, NULL, NULL, 
		INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_RELOAD, 0);
	if(m_Request == NULL)
	{
		m_dwError = GetLastError();
		goto UrlRequest_Done;
	}

	bRetVal = HttpSendRequestA(m_Request, pHdr, (DWORD)strlen((CONST CHAR *)pHdr), pRequest, nReqSize);
//	bRetVal = HttpSendRequest(m_Request, NULL, 0, pRequest, nReqSize);
	if(bRetVal == FALSE)
	{
		m_dwError = GetLastError();
		goto UrlRequest_Done;
	}

	dwBufSize = sizeof(m_dwTotalSize);
	bRetVal = HttpQueryInfoA(m_hConnect, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, 
		&m_dwTotalSize, &dwBufSize, NULL);
	if(bRetVal)
	{
		if((int)m_dwTotalSize > nBufSize)
			return -1;
	}

	dwSaveSize = 0;
	for(;;)
	{
		bRetVal = InternetReadFile(m_Request, m_pBuffer, m_dwOneSize, &dwReadSize);
		if(bRetVal == FALSE || dwReadSize == 0)
			break;

		if(dwSaveSize+dwReadSize > (DWORD)nBufSize)
		{
			dwSaveSize = -1;
			break;
		}

		memcpy(pBuffer+dwSaveSize, m_pBuffer, dwReadSize);
		dwSaveSize += dwReadSize;
	}

/*
	InternetOpen();
	InternetConnect();
	HttpOpenRequest();
	HttpSendRequest();

	HINTERNET hOpenHandle,  hConnectHandle, hResourceHandle;
	DWORD dwError, dwErrorCode;

*/


UrlRequest_Done:

	return dwSaveSize;
}


//	CHttpDownload
/////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////
//	CKeyEventEng

CKeyEventEng *	g_pOneKeyEventEng = NULL;

CKeyEventEng::CKeyEventEng()
{
	g_pOneKeyEventEng = this;
	m_hHook = NULL;
}

CKeyEventEng::~CKeyEventEng()
{
	Stop();
}

HMODULE	CKeyEventEng::GetModuleFromAddress(VOID * Address)
{
	MEMORY_BASIC_INFORMATION mbi;
	DWORD		dwRetVal;
	HMODULE		hMod = NULL;

	dwRetVal = (DWORD)VirtualQuery(Address, &mbi, sizeof(mbi));
	if(dwRetVal != 0)
		hMod = (HMODULE) mbi.AllocationBase;

	return	hMod;
}


//	This hook is called in the context of the thread that installed it. 
//	The call is made by sending a message to the thread that installed the hook. 
//	Therefore, the thread that installed the hook must have a message loop. 
//	该线程必须有消息队列, 否则没用
LRESULT CALLBACK CKeyEventEng::KeyEventProc(
									  int nCode,     // hook code
									  WPARAM wParam, // message identifier
									  LPARAM lParam  // pointer to structure with message data
									  )
{
	LRESULT		lRetVal;
	BOOL		bCtrl, bAlt, bShift;
	CHAR		szKeyString[1024];
	DWORD		DownlParam;
	CHAR		szChar[100];
	BOOL		bRetVal;
	BOOL		bSysKeyDown;

	KBDLLHOOKSTRUCT *pkh = (KBDLLHOOKSTRUCT *)lParam;

	bSysKeyDown = FALSE;

	if(nCode != HC_ACTION)
		goto End;

	if(pkh->vkCode == VK_CONTROL || 
		pkh->vkCode == VK_MENU || 
		pkh->vkCode == VK_SHIFT ||
		pkh->vkCode == VK_LSHIFT ||
		pkh->vkCode == VK_RSHIFT ||
		pkh->vkCode == VK_LMENU ||
		pkh->vkCode == VK_RMENU ||
		pkh->vkCode == VK_LCONTROL ||
		pkh->vkCode == VK_RCONTROL
		)
	{
		bSysKeyDown = TRUE;
//		goto End;
	}

	szKeyString[0] = 0;
	if(bSysKeyDown == FALSE)
	{
		bCtrl	= GetKeyState(VK_CONTROL)	< 0;
		bAlt	= GetKeyState(VK_MENU)		< 0;
		bShift	= GetKeyState(VK_SHIFT)		< 0;

		if(bCtrl)	strcat(szKeyString, "Ctrl + ");
		if(bAlt)	strcat(szKeyString, "Alt + ");
		if(bShift)	strcat(szKeyString, "Shift + ");
	}

	DownlParam = 0;
	DownlParam |= 0x0001;					//0-15	repeat count
	DownlParam |= pkh->scanCode << 16;		//16-23
	DownlParam |= (pkh->flags & 0x0F) <<24;	//24,25

	GetKeyNameTextA(DownlParam, szChar, sizeof(szChar));
	strcat(szKeyString, szChar);

	bRetVal = g_pOneKeyEventEng->OnKeyEvent(szKeyString, wParam, lParam);
	if(bRetVal == FALSE)
		return 1;

End:
	lRetVal = CallNextHookEx(g_pOneKeyEventEng->m_hHook, nCode, wParam, lParam);
	return lRetVal;
}


BOOL	CKeyEventEng::Start()
{
	HINSTANCE		hMod;

	Stop();
	hMod = (HINSTANCE)GetModuleFromAddress(KeyEventProc);

	m_hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyEventProc, hMod, 0);
	if(m_hHook == NULL)
		return FALSE;


	return TRUE;
}

BOOL	CKeyEventEng::Stop()
{
	if(m_hHook == NULL)
		return TRUE;

	DWORD		dwRecipients;

	UnhookWindowsHookEx(m_hHook);
	BroadcastSystemMessage(BSF_FORCEIFHUNG, &dwRecipients, WM_NULL, 0, 0);

	m_hHook = NULL;
	return FALSE;
}


BOOL	CKeyEventEng::OnKeyEvent(CHAR *pKeyText, WPARAM wParam, LPARAM lParam)
{
	BOOL		bRetVal;

	bRetVal = NotityProc(pKeyText, wParam, lParam);
	if(bRetVal == FALSE)
		return FALSE;

	bRetVal = NotityWnd(pKeyText);
	if(bRetVal == FALSE)
		return FALSE;

	return TRUE;
}

BOOL	CKeyEventEng::NotityProc(CHAR *pKeyText, WPARAM wParam, LPARAM lParam)
{
	std::map<KEYNTYPROC, VOID *>::iterator		Iter;

	BOOL			bRetVal;
	KEYNTYPROC		fNty;

	for(Iter = m_NtyFun.begin();
		Iter != m_NtyFun.end();
		Iter ++)
	{
		fNty = Iter->first;
		if(fNty == NULL)
			continue;

		bRetVal = fNty(pKeyText, Iter->second, wParam, lParam);
		if(bRetVal == FALSE)
			return FALSE;
	}

	return TRUE;
}

BOOL	CKeyEventEng::NotityWnd(CHAR *pKeyText)
{

	return TRUE;
}


BOOL	CKeyEventEng::AddNtyProc(KEYNTYPROC fnProc, VOID *pParam)
{
	m_NtyFun[fnProc] = pParam;

	return TRUE;
}

BOOL	CKeyEventEng::DelNtyProc(KEYNTYPROC fnProc)
{
	std::map<KEYNTYPROC, VOID *>::iterator		Iter;

	Iter = m_NtyFun.find(fnProc);
	if(Iter == m_NtyFun.end())
		return FALSE;

	m_NtyFun.erase(Iter);

	return TRUE;
}


//	SendMessage 缺省缓冲NULL方式就会用 SendMessage
//	PostMessage 方式必需缓冲区
BOOL	CKeyEventEng::AddNtyWnd(HWND hWnd, UINT uMsg, CHAR *pBuffer)
{
	std::vector<Nty_Wnd_Data>::iterator		Iter;
	Nty_Wnd_Data	OneData;

	if(hWnd == NULL)
		return FALSE;

	for(Iter = m_NtyWnd.begin();
		Iter != m_NtyWnd.end();
		Iter ++)
	{
		if(Iter->hWnd == hWnd)
		{
			Iter->uMsg = uMsg;
			Iter->pBuffer = pBuffer;
			return TRUE;
		}
	}

	OneData.hWnd = hWnd;
	OneData.uMsg = uMsg;
	OneData.pBuffer = pBuffer;
	m_NtyWnd.push_back(OneData);

	return TRUE;
}

BOOL	CKeyEventEng::DelNtyWnd(HWND hWnd)
{
	std::vector<Nty_Wnd_Data>::iterator		Iter;

	if(hWnd == NULL)
		return FALSE;

	for(Iter = m_NtyWnd.begin();
		Iter != m_NtyWnd.end();
		Iter ++)
	{
		if(Iter->hWnd == hWnd)
		{
			m_NtyWnd.erase(Iter);
			return TRUE;
		}
	}

	return FALSE;
}
//	CKeyEventEng
////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////
//	CKeyEventAss 相关

CKeyEventAss *	g_pOneKeyEventAss = NULL;
int				g_nKeyEventAssRefs = 0;

CKeyEventAss *	StartKeyEventAss()
{
	if(g_pOneKeyEventAss == NULL)
	{
		g_pOneKeyEventAss = new CKeyEventAss;
	}

	g_nKeyEventAssRefs ++;

	return g_pOneKeyEventAss;
}


BOOL		StopKeyEnentAss()
{
	g_nKeyEventAssRefs --;
	if(g_nKeyEventAssRefs > 0)
		return TRUE;

	if(g_pOneKeyEventAss == NULL)
		return FALSE;

	delete g_pOneKeyEventAss;
	g_pOneKeyEventAss = NULL;

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//							CKeyEventAss
CKeyEventAss::CKeyEventAss()
{
	m_dwSleep = 10;

	m_KeyData.clear();
	InitKeyMap();

	m_bExit = FALSE;
	m_hThread = CreateThread(NULL, 0, ThreadProc, (VOID *)this, 0, NULL);
}

DWORD	CALLBACK  CKeyEventAss::ThreadProc(VOID * pParam)
{
	CKeyEventAss		*pThis;
	KEYDATA				*pData;

	std::list<KEYDATA>::iterator	iter;


	pThis = (CKeyEventAss *)pParam;

	for(Sleep(10);
		pThis->m_bExit == FALSE;
		Sleep(1))
	{
		pThis->m_Lock.Lock();

		if(pThis->m_KeyData.size())
		{
			iter = pThis->m_KeyData.begin();
			pData = &*iter;

			::keybd_event(pData->bVk, 0, pData->nFlags, 0);

			Sleep(pData->dwSleep);

			pThis->m_KeyData.pop_front();
		}

		pThis->m_Lock.UnLock();
	}

	return 0;
}

CKeyEventAss::~CKeyEventAss()
{
	m_bExit = TRUE;
	m_KeyMap.clear();
	WaitForSingleObject(m_hThread, INFINITE);
}


//	设置间隔掩饰时间
BOOL	CKeyEventAss::SetDefaultSleep(DWORD dwSleep)
{
	m_dwSleep = dwSleep;

	return TRUE;
}


BOOL	CKeyEventAss::InitKeyMap()
{
	UINT		uVk;
	DWORD		DownlParam;
	int			scanCode;
	int			flags;
	CHAR		szBuf[1024];
	int			nRetVal;

	m_KeyMap.clear();

	flags = 0;
	for(flags=0; flags<2; flags++)
	{
		for(scanCode=0; scanCode<256; scanCode++)
		{
			uVk = MapVirtualKey(scanCode, 1);

			if(uVk == 0)
				continue;

			DownlParam = 0;
			DownlParam |= 0x0001;					//0-15	repeat count
			DownlParam |= scanCode << 16;			//16-23
			DownlParam |= (flags & 0x0F) <<24;		//24,25

			nRetVal = GetKeyNameTextA(DownlParam, szBuf, 100);
			if(nRetVal == 0)
				continue;

			if( m_KeyMap.find(szBuf) != m_KeyMap.end())
			{
				NLog::DbgLog("%08X\t%s  !!!", uVk, szBuf);
				continue;
			}

			NLog::DbgLog("%08X\t%s", uVk, szBuf);
			m_KeyMap[szBuf] = MAKELONG(uVk, flags);
		}
	}

	return TRUE;
}


BOOL CKeyEventAss::GetVirtualKey(CHAR *pString, UINT* puVk, BOOL* pbExtend)
{
	DWORD		dwValue;
	std::map<std::string, DWORD>::iterator iFind;

	*puVk = 0;
	*pbExtend = FALSE;

	iFind = m_KeyMap.find(pString);
	if(iFind == m_KeyMap.end())
		return FALSE;

	dwValue = iFind->second;

	*puVk = LOWORD(dwValue);
	*pbExtend = HIWORD(dwValue);

	return TRUE;
}


//	nFlags  0: 按下  
//	KEYEVENTF_KEYUP: 弹上  
//	KEYEVENTF_EXTENDEDKEY: 扩展键
BOOL	CKeyEventAss::AddKeyEvent(BYTE bVk, int nFlags, DWORD dwSleep)
{
	KEYDATA			KeyDat;

	KeyDat.nFlags = nFlags;
	KeyDat.bVk = bVk;
	if(dwSleep == -1)
		KeyDat.dwSleep = m_dwSleep;
	else
		KeyDat.dwSleep = dwSleep;

	m_Lock.Lock();

	m_KeyData.push_back(KeyDat);

	m_Lock.UnLock();

	return TRUE;
}


BOOL	CKeyEventAss::PressOneKey(CHAR *pKeyString, DWORD dwSleep)
{
	BOOL		bRetVal;
	UINT		uKey;
	BOOL		bExtend;

	bRetVal = GetVirtualKey(pKeyString, &uKey, &bExtend);
	if(bRetVal == FALSE)
		return FALSE;

	if(bExtend)
	{
		AddKeyEvent(uKey, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYDOWN, dwSleep);
		AddKeyEvent(uKey, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, dwSleep);
	}
	else
	{
		AddKeyEvent(uKey, KEYEVENTF_KEYDOWN, dwSleep);
		AddKeyEvent(uKey, KEYEVENTF_KEYUP, dwSleep);
	}

	return TRUE;
}


DWORD	CKeyEventAss::PressKeys(CHAR *pKeyString, DWORD dwSleep)
{
	CHAR		szBuf[4096];
	CHAR		szAry[100][20];
	DWORD		i;
	DWORD		dwCount;

	strncpy(szBuf, pKeyString, sizeof(szBuf)-1);
	NStr::StrReplace(szBuf, "[[]", "[{]");
	NStr::StrReplace(szBuf, "[]]", "[}]");

	NStr::StrReplace(szBuf, "[", "\"");
	NStr::StrReplace(szBuf, "]", "\" ");

	NStr::StrReplace(szBuf, "\"{\"", "\"[\"");
	NStr::StrReplace(szBuf, "\"}\"", "\"]\"");

	dwCount = NStr::StrtoFormatArry(szBuf, szAry[0], 100, 20);

	for(i=0; i<dwCount; i++)
	{
		PressOneKey(szAry[i], dwSleep);
	}

	return dwCount;
}
//							CKeyEventAss
////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////
//							CFileIconMgr

CFileSmallIconMgr::CFileSmallIconMgr()
{

}

CFileSmallIconMgr::~CFileSmallIconMgr()
{
	Clear();
}

BOOL	CFileSmallIconMgr::Clear()
{
	std::map<std::string, HICON>::iterator		Iter;
	HICON		hIcon;

	for(Iter = m_IconMap.begin();
		Iter != m_IconMap.end();
		Iter ++)
	{
		hIcon = Iter->second;
		::DestroyIcon(hIcon);
	}

	m_IconMap.clear();
	return TRUE;
}

CFileSmallIconMgr *	g_pFileIconMgr = NULL;

void __cdecl DestoryFileIconMgr(void)
{
	delete g_pFileIconMgr;
	g_pFileIconMgr = NULL;
}


CFileSmallIconMgr * CFileSmallIconMgr::CreateInstance()
{
	if(g_pFileIconMgr)
		return g_pFileIconMgr;

	g_pFileIconMgr = new CFileSmallIconMgr;
	atexit(DestoryFileIconMgr);

	return g_pFileIconMgr;
}


HICON	CFileSmallIconMgr::GetFileIcon(CHAR *pFileName)
{
	HICON		hIcon;

	hIcon = FindFileIcon(pFileName);
	if(hIcon != NULL)
	{
		return hIcon;
	}

	hIcon = SaveFileIcon(pFileName);
	return hIcon;
}


BOOL		CFileSmallIconMgr::ConvertPath(CHAR *pFilePath, std::string &RealPath)
{
	RealPath = pFilePath;

	return TRUE;
}

HICON			CFileSmallIconMgr::SaveFileIcon(CHAR *pFilePath)
{
	std::string		sRealPath;
	HICON			hIcon;

	ConvertPath(pFilePath, sRealPath);

	hIcon = NFile::GetFileSmallIcon((CHAR *)sRealPath.c_str());
	if(hIcon == NULL)
	{
		return NULL;
	}

	m_IconMap[sRealPath] = hIcon;
	return hIcon;
}


HICON			CFileSmallIconMgr::FindFileIcon(CHAR *pFilePath)
{
	std::string		sRealPath;
	std::map<std::string, HICON>::iterator		Iter;

	ConvertPath(pFilePath, sRealPath);

	Iter = m_IconMap.find(sRealPath);
	if(Iter == m_IconMap.end())
		return NULL;

	return Iter->second;
}
//		CFileSmallIconMgr
/////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////
//		CServiceBase

CServiceBase *	s_pServiceBase = NULL;

CServiceBase::CServiceBase()
{
	s_pServiceBase = this;
	m_hServiceStatus = NULL;
	memset(&m_ServiceStatus, 0, sizeof(m_ServiceStatus));
}


CServiceBase::~CServiceBase()
{

}


VOID	CServiceBase::OnError(CHAR *pText, int nError)
{
	OnEventLog(pText, nError);

	return;
}

VOID	CServiceBase::OnEventLog(CHAR *pText, int nError)
{
	if(nError != 0)
		NLog::DbgLog("%s Error: %d", pText, nError);
	else
		NLog::DbgLog("%s", pText);

	return;
}

VOID	CServiceBase::OnServiceMainAction()
{
	OnEventLog("OnServiceMainAction");

	for(;m_bStop == FALSE;)
		Sleep(1);

	return;
}


BOOL	CServiceBase::ReportEventLog(int nEventId, CHAR * pFormat, ...)
{
	HANDLE		hEventSource;
	CHAR *		lpszStrings;
	va_list		pArg;
	CHAR		szEventString[1024];
	BOOL		bRetVal;

	va_start(pArg, pFormat);
	vsprintf(szEventString, pFormat, pArg);
	va_end(pArg);

	lpszStrings = szEventString;

	hEventSource = RegisterEventSourceA(NULL, m_szServiceName);
	if(hEventSource == NULL)
		return FALSE;

	bRetVal = ReportEventA(hEventSource, EVENTLOG_INFORMATION_TYPE, 0, nEventId, NULL, 1, 0, 
			(CONST CHAR **)&lpszStrings, NULL);
	DeregisterEventSource(hEventSource);

	return bRetVal;
}



VOID	CServiceBase::OnActionWithStation()
{
	OnEventLog("OnActionWithStation");

	for(;m_bStop == FALSE;)
		Sleep(1);

	return;
}


VOID	CServiceBase::OnCtrlServer(DWORD fdwControl)
{
	SetServerError(ERROR_SUCCESS);

	switch(fdwControl)
	{
	case SERVICE_CONTROL_PAUSE:
		OnPauseServer();
		break;

	case SERVICE_CONTROL_CONTINUE:
		OnPauseServer(TRUE);
		break;

	case SERVICE_CONTROL_STOP:
	case SERVICE_CONTROL_SHUTDOWN:
		OnStopServer();
		break;

	case SERVICE_CONTROL_INTERROGATE:
	default:
		;
	}

	return;
}


VOID	CServiceBase::OnStopServer()
{

}

VOID	CServiceBase::OnPauseServer(BOOL bResume)
{

}

BOOL	CServiceBase::InitConfig(CHAR *pServerName, CHAR *pDisplayName, DWORD dwStartType, 
								 DWORD dwServerType)
{
	if(pDisplayName == NULL)
		pDisplayName = pServerName;

	strcpy(m_szServiceName, pServerName);
	strcpy(m_szDisplayName, pDisplayName);

	m_dwServerType = dwServerType;
	m_dwStartType = dwStartType;

	return TRUE;
}

BOOL	CServiceBase::CreateServer()
{
	CHAR			szModFileName[MAX_PATH];
	DWORD			dwRetVal;
	SC_HANDLE		hSC, hService;
	CHAR			szImagePath[MAXPATH];
	BOOL			bResult;


	hSC = NULL;
	hService = NULL;
	bResult = FALSE;

	dwRetVal = GetModuleFileNameA(NULL,	szModFileName, sizeof(szModFileName));
	if(dwRetVal == 0)
	{
		OnError("GetModuleFileNameA", GetLastError());
		goto CreateServerEnd;
	}

	if(strchr(szModFileName, ' ') && szModFileName[0] != '"')	//路径包含空格
		sprintf(szImagePath, "\"%s\"", szModFileName);
	else
		strcpy(szImagePath, szModFileName);

	hSC = OpenSCManagerA(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(hSC == NULL)
	{
		OnError("OpenSCManagerA", GetLastError());
		goto CreateServerEnd;
	}

	hService = CreateServiceA(hSC, m_szServiceName, m_szDisplayName, SERVICE_ALL_ACCESS, 
		m_dwServerType, m_dwStartType, SERVICE_ERROR_NORMAL, szImagePath, 
		NULL, NULL, NULL, NULL, NULL);
	if(hService == NULL)
	{
		//	1073	ERROR_SERVICE_EXISTS
		OnError("CreateServiceA", GetLastError());
		goto CreateServerEnd;
	}

	bResult = TRUE;

CreateServerEnd:

	if(hService)
		CloseServiceHandle(hService);
	if(hSC)
		CloseServiceHandle(hSC);

	return bResult;
}


BOOL	CServiceBase::StartServer()
{
	BOOL	bRetVal;
	int		nError;

	CreateServer();

	bRetVal = NSys::StartUpService(m_szServiceName);
	if(bRetVal == FALSE)
	{
		nError = GetLastError();
		OnError("StartUpService", nError);
		SetLastError(nError);
	}

	return bRetVal;
}


BOOL	CServiceBase::PauseServer(BOOL bResume)
{
	BOOL	bRetVal;

	bRetVal = NSys::PauseService(m_szServiceName, bResume);

	return bRetVal;
}


BOOL	CServiceBase::GetServerStat(DWORD &dwStat)
{
	dwStat = m_ServiceStatus.dwCurrentState;

	return TRUE;
}


BOOL	CServiceBase::StopServer()
{
	m_bStop = TRUE;
	return TRUE;
}

BOOL	CServiceBase::ChangeServiceStatus(DWORD dwState, DWORD dwError, DWORD dwWaitHint)
{
	BOOL		bRetVal;

	m_ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	m_ServiceStatus.dwCurrentState = dwState;
	m_ServiceStatus.dwWaitHint = dwWaitHint;
	if(dwError != ERROR_SUCCESS)
	{
		m_ServiceStatus.dwWin32ExitCode = ERROR_SERVICE_SPECIFIC_ERROR;
		m_ServiceStatus.dwServiceSpecificExitCode = dwError;
	}
	else
	{
		m_ServiceStatus.dwWin32ExitCode = ERROR_SUCCESS;
		m_ServiceStatus.dwServiceSpecificExitCode = ERROR_SUCCESS;
	}

	if(dwState == SERVICE_START_PENDING)
		m_ServiceStatus.dwControlsAccepted = 0;
	else
		m_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_PAUSE_CONTINUE;

	if(dwState == SERVICE_RUNNING || dwState == SERVICE_STOPPED)
	{
		m_ServiceStatus.dwCheckPoint = 0;
	}
	else
	{
		m_ServiceStatus.dwCheckPoint = m_dwCheckPoint;
		m_dwCheckPoint ++;
	}

	bRetVal = SetServiceStatus(m_hServiceStatus, &m_ServiceStatus);
	if(bRetVal == FALSE)
	{
		s_pServiceBase->OnError("SetServiceStatus", GetLastError());
	}
	return bRetVal;
}


BOOL	CServiceBase::ChangeServiceConfigs()
{
	SERVICE_FAILURE_ACTIONSA	ServiceInfo;
	SC_ACTION					ScAction[3];
	int							i;
	SC_HANDLE					hSC, hService;
	SC_LOCK						hLockSC;
	BOOL						bResult;


	bResult = FALSE;
	hSC = NULL;
	hService = NULL;
	hLockSC = NULL;

	for(i=0; i<3; i++)
	{
		ScAction[i].Type = SC_ACTION_NONE;
		ScAction[i].Delay = 6000;
	}

	ServiceInfo.dwResetPeriod = 0;
	ServiceInfo.lpCommand = NULL;
	ServiceInfo.lpRebootMsg = 0;
	ServiceInfo.cActions = 3;
	ServiceInfo.lpsaActions = ScAction;

	hSC = OpenSCManagerA(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(hSC == NULL)
	{
		goto ChangeServiceConfigEnd;
	}

	hLockSC = LockServiceDatabase(hSC);
	if(hLockSC == NULL)
	{
		goto ChangeServiceConfigEnd;
	}

	hService = OpenServiceA(hSC, m_szServiceName, SERVICE_ALL_ACCESS);
	if(hService == NULL)
	{
		goto ChangeServiceConfigEnd;
	}

	ChangeServiceConfig2A(hService, SERVICE_CONFIG_FAILURE_ACTIONS, &ServiceInfo);
	bResult = TRUE;

ChangeServiceConfigEnd:

	if(hService)
		CloseServiceHandle(hService);
	if(hLockSC)
		UnlockServiceDatabase(hLockSC);
	if(hSC)
		CloseServiceHandle(hSC);

	return bResult;
}

VOID WINAPI	CServiceBase::ServerHandlerProc(DWORD fdwControl)
{
	CHAR		szText[1024];

	sprintf(szText, "HandlerProc fdwControl: %08X", fdwControl);
	s_pServiceBase->OnEventLog(szText);

	s_pServiceBase->OnCtrlServer(fdwControl);

	switch(fdwControl)
	{
	case SERVICE_CONTROL_PAUSE:
		s_pServiceBase->ChangeServiceStatus(SERVICE_PAUSED, 0, 0);
		break;

	case SERVICE_CONTROL_CONTINUE:
		s_pServiceBase->ChangeServiceStatus(SERVICE_RUNNING, 0, 0);
		break;

	case SERVICE_CONTROL_STOP:
	case SERVICE_CONTROL_SHUTDOWN:
		s_pServiceBase->ChangeServiceStatus(SERVICE_STOP_PENDING, 0, 3000);
		s_pServiceBase->m_bStop = TRUE;
		break;

	case SERVICE_CONTROL_INTERROGATE:
	default:
		s_pServiceBase->ChangeServiceStatus(s_pServiceBase->m_ServiceStatus.dwCurrentState, 0, 3000);
	}
	return;
}


VOID WINAPI CServiceBase::ServiceMain(DWORD dwArgc, LPSTR *lpszArgv)
{
	BOOL						bRetVal;

	s_pServiceBase->OnEventLog("ServiceMain");

	s_pServiceBase->m_hServiceStatus = RegisterServiceCtrlHandlerA(s_pServiceBase->m_szServiceName, ServerHandlerProc);
	if(s_pServiceBase->m_hServiceStatus == NULL)
	{
		s_pServiceBase->OnError("RegisterServiceCtrlHandlerA", GetLastError());
		return;
	}

	bRetVal = s_pServiceBase->ChangeServiceStatus(SERVICE_START_PENDING, 0, 3000);
	if(bRetVal == FALSE)
	{
		s_pServiceBase->OnError("ChangeServiceStatus", GetLastError());
		return;
	}

	bRetVal = s_pServiceBase->ChangeServiceStatus(SERVICE_RUNNING, 0, 0);
	if(bRetVal == FALSE)
	{
		s_pServiceBase->OnError("ChangeServiceStatus", GetLastError());
		return;
	}

	s_pServiceBase->ChangeServiceConfigs();

	s_pServiceBase->m_bStop = FALSE;
	s_pServiceBase->m_nLastError = ERROR_SUCCESS;

	s_pServiceBase->OnServiceMainAction();
	s_pServiceBase->RunActionWithStation();

	if(s_pServiceBase->m_nLastError == ERROR_SUCCESS)
	{
		s_pServiceBase->ChangeServiceStatus(SERVICE_STOP_PENDING, 0, 3000);
		s_pServiceBase->ChangeServiceStatus(SERVICE_STOPPED);
	}
	else
	{
		s_pServiceBase->ChangeServiceStatus(SERVICE_STOP_PENDING, s_pServiceBase->m_nLastError, 3000);
		s_pServiceBase->ChangeServiceStatus(SERVICE_STOPPED, s_pServiceBase->m_nLastError);
	}

	return;
}

BOOL	CServiceBase::CtrlServiceStart()
{
	BOOL						bRetVal;
	SERVICE_TABLE_ENTRYA		ServiceStartTable[2];

	memset(&ServiceStartTable, 0, sizeof(ServiceStartTable));
	ServiceStartTable[0].lpServiceName = m_szServiceName;
	ServiceStartTable[0].lpServiceProc = ServiceMain;

	bRetVal = StartServiceCtrlDispatcherA(&ServiceStartTable[0]);
	if(bRetVal == FALSE)
	{
		//	1063	ERROR_FAILED_SERVICE_CONTROLLER_CONNECT
		//	1056	ERROR_SERVICE_ALREADY_RUNNING
		OnError("StartServiceCtrlDispatcherA", GetLastError());

		return FALSE;
	}

	return TRUE;
}


BOOL	CServiceBase::SetServerError(int nError)
{
	m_nLastError = nError;
	return TRUE;
}

BOOL	CServiceBase::RunActionWithStation()
{
	HDESK   hDesk;
	HWINSTA hWinsta;
	BOOL	bResult;
	ACCESS_MASK dwDesiredAccess;

	hWinsta = NULL;
	hDesk = NULL;
	bResult = FALSE;

	dwDesiredAccess = 
		WINSTA_ACCESSCLIPBOARD   |
		WINSTA_ACCESSGLOBALATOMS |
		WINSTA_CREATEDESKTOP     |
		WINSTA_ENUMDESKTOPS      |
		WINSTA_ENUMERATE         |
		WINSTA_EXITWINDOWS       |
		WINSTA_READATTRIBUTES    |
		WINSTA_READSCREEN        |
		WINSTA_WRITEATTRIBUTES   |
		0;
	hWinsta = OpenWindowStationA("winsta0", FALSE, dwDesiredAccess);
	if(hWinsta == NULL)
	{
		OnError("OpenWindowStation", GetLastError());
	}
	if(!SetProcessWindowStation(hWinsta))
	{
		assert(0);
		OnError("SetProcessWindowStation", GetLastError());
	}


	dwDesiredAccess = 
		DESKTOP_CREATEMENU |
		DESKTOP_CREATEWINDOW |
		DESKTOP_ENUMERATE    |
		DESKTOP_HOOKCONTROL  |
		DESKTOP_JOURNALPLAYBACK |
		DESKTOP_JOURNALRECORD |
		DESKTOP_READOBJECTS |
		DESKTOP_SWITCHDESKTOP |
		DESKTOP_WRITEOBJECTS  |
		0;
	hDesk = OpenDesktopA("default", 0, FALSE, dwDesiredAccess);
	if (hDesk == NULL)
	{
		assert(0);
		OnError("OpenDesktop", GetLastError());
	}
	if (!SetThreadDesktop(hDesk))
	{
		assert(0);
		OnError("SetThreadDesktop", GetLastError());
	}

	OnActionWithStation();

	bResult = TRUE;

	if(hWinsta)
		CloseWindowStation(hWinsta);
	if(hDesk)
		CloseDesktop(hDesk);

	return bResult;
}




//		CServiceBase
////////////////////////////////////////////////////////////////////////////////////////////////////





////////////////////////////////////////////////////////////////////////////////////////////////////
//		CDiskFile


CDiskFile::CDiskFile()
{
	m_hFile = INVALID_HANDLE_VALUE;
	m_hVolume = INVALID_HANDLE_VALUE;
	m_hDisk = INVALID_HANDLE_VALUE;
	m_bOpen = FALSE;
	m_bInit = FALSE;
	m_dwFileSize = -1;
}

CDiskFile::~CDiskFile()
{
	Close();
}

BOOL	CDiskFile::Close()
{
	if(m_hFile != INVALID_HANDLE_VALUE)
		CloseHandle(m_hFile);

	if(m_hVolume != INVALID_HANDLE_VALUE)
		CloseHandle(m_hVolume);

	if(m_hDisk != INVALID_HANDLE_VALUE)
		CloseHandle(m_hDisk);

	m_hFile = INVALID_HANDLE_VALUE;
	m_hVolume = INVALID_HANDLE_VALUE;
	m_hDisk = INVALID_HANDLE_VALUE;
	m_bOpen = FALSE;

	return TRUE;
}

BOOL	CDiskFile::InitInfo(CHAR *pFilePath)
{
	BOOL		bRetVal;
	CHAR		szVolume[1024];

	if(m_bInit)
		return TRUE;

	bRetVal = GetVolumePathNameA(pFilePath, m_szVolume, sizeof(m_szVolume));
	if(bRetVal == FALSE)
		return FALSE;

	bRetVal = GetDiskFreeSpaceA(m_szVolume, &m_dwSectorsPerCluster, &m_dwBytesPerSector, 
		&m_dwNumberOfFreeClusters, &m_dwTotalNumberOfClusters);
	if(bRetVal == FALSE)
		return FALSE;

	strcpy(szVolume, m_szVolume);
	if(szVolume[2] == '\\')
		szVolume[2] = 0;

	_snprintf(m_szFullVolume, sizeof(m_szFullVolume)-1, "\\\\.\\%s", szVolume);
	m_hVolume = CreateFileA(m_szFullVolume, GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,	NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(m_hVolume == INVALID_HANDLE_VALUE)
		return FALSE;


	VOLUME_DISK_EXTENTS		DiskEx;
	DWORD					dwBytesReturned;
	int						nDiskNub;

	bRetVal = DeviceIoControl(m_hVolume, IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS, NULL, 0, 
		&DiskEx, sizeof(DiskEx), &dwBytesReturned, NULL);
	if(bRetVal == FALSE)
		return FALSE;

	nDiskNub = DiskEx.Extents[0].DiskNumber;
	sprintf(m_szDiskPath, "\\\\.\\PhysicalDrive%d", nDiskNub);
	m_hDisk = CreateFileA(m_szDiskPath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(m_hDisk == INVALID_HANDLE_VALUE)
		return FALSE;

	bRetVal = GetDiskOtherInfo();
	if(bRetVal == FALSE)
		return FALSE;

	bRetVal = CheckSuppert();
	if(bRetVal == FALSE)
		return FALSE;

	m_bInit = TRUE;

	return TRUE;
}


BOOL	CDiskFile::OpenFile(CHAR *pFilePath)
{
	BOOL		bRetVal;
	DWORD		dwHSize;

	bRetVal = CheckOpen();
	if(bRetVal)
		return FALSE;

	Close();

	if(pFilePath)
		strcpy(m_szFilePath, pFilePath);

	pFilePath = m_szFilePath;
	if(pFilePath[0] == 0)
		return FALSE;

	m_hFile = CreateFileA(pFilePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(m_hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	m_dwFileSize = ::GetFileSize(m_hFile, &dwHSize);
	if(dwHSize != 0)
		return FALSE;

	m_bOpen = TRUE;

	InitInfo(pFilePath);

	return TRUE;
}


BOOL	CDiskFile::CheckOpen()
{
	return m_bOpen;
}

BOOL	CDiskFile::GetDiskOtherInfo()
{
	BOOL				bRetVal;
	DWORD				dwBytesRead;
	LARGE_INTEGER		llOffset;
	Fat32BootData		Boot;

	bRetVal = GetVolumeInformationA(m_szVolume, NULL, NULL, NULL, NULL, 
		&m_dwVolumeFileSystemFlags, m_szVolumeFileSystemName, sizeof(m_szVolumeFileSystemName));
	if(bRetVal == FALSE)
		return FALSE;

	if(stricmp(m_szVolumeFileSystemName, "FAT32") == 0)
	{
		m_nVolumeType = PARTITION_FAT32;
	}
	else if(stricmp(m_szVolumeFileSystemName, "NTFS") == 0)
	{
		m_nVolumeType = PARTITION_NTFT;
	}
	else
	{
		m_nVolumeType = -1;
	}


	if(m_nVolumeType == PARTITION_NTFT)
	{
		m_dwFat32FirstDataSector = 0;
		return TRUE;
	}


	llOffset.QuadPart = 0;
	bRetVal = SetFilePointerEx(m_hVolume, llOffset, NULL, FILE_BEGIN);
	if(bRetVal == FALSE)
		return FALSE;

	bRetVal = ReadFile(m_hVolume, &Boot, sizeof(Boot), &dwBytesRead, NULL);
	if(bRetVal == FALSE || dwBytesRead != sizeof(Boot))
		return FALSE;


	DWORD		dwReservedSectors;
	DWORD		dwFatSectors;
	DWORD		dwDirtoryTableSectors;

	dwReservedSectors = Boot.ReservedSectors;
	dwFatSectors = Boot.BigSectorsPerFat * Boot.NumberOfFATs;	//FAT32
	dwDirtoryTableSectors = ((Boot.RootEntries * 32 - 1) / m_dwBytesPerSector + 1) * m_dwBytesPerSector;

	m_dwFat32FirstDataSector = dwReservedSectors + dwFatSectors + dwDirtoryTableSectors;
	return TRUE;
}

BOOL	CDiskFile::CheckSuppert()
{
	if(m_nVolumeType != PARTITION_FAT32 && m_nVolumeType != PARTITION_NTFT)
		return FALSE;

	return TRUE;
}


BOOL	CDiskFile::GetFileCluster()
{
	Nntdll::IO_STATUS_BLOCK			IoStatus;
	STARTING_VCN_INPUT_BUFFER		InVcn;
	RETRIEVAL_POINTERS_BUFFER *		pBuf;
	DWORD							dwSize;
	Nntdll::NTSTATUS				Status;
	DWORD							i;
	BOOL							bResult;
	BOOL							bRetVal;

	bResult = FALSE;
	InVcn.StartingVcn.QuadPart = 0;

	pBuf = NULL;
	dwSize = 1024;

	for(; ; dwSize*=2)
	{
		if(pBuf)
			free(pBuf);

		pBuf = (RETRIEVAL_POINTERS_BUFFER *)malloc(dwSize);
		memset(pBuf, 0, dwSize);

		Status = Nntdll::NtFsControlFile(m_hFile, NULL, NULL, NULL, &IoStatus, FSCTL_GET_RETRIEVAL_POINTERS,
			&InVcn, sizeof(InVcn), pBuf, dwSize);

		if(Status == Nntdll::STATUS_BUFFER_OVERFLOW)
			continue;

		if(Status == Nntdll::STATUS_SUCCESS)
			break;

		//	FSCTL_GET_RETRIEVAL_POINTERS	返回	STATUS_END_OF_FILE
		//	The stream is resident in the MFT and has no clusters allocated
		//	NTFS文件当文件很小时，文件会保存在FILE_RECORD结构里，而不会给它分配簇来保存数据
		if(Status == Nntdll::STATUS_END_OF_FILE)
			goto GetFileLocationDone;

		if(dwSize > 10*1024)
			goto GetFileLocationDone;
	}


	FileCluster		Item;
	LARGE_INTEGER	llStart;
	LARGE_INTEGER	llFirstSector;


	llStart.QuadPart = 0;
	m_FileCluster.clear();

	for(i=0; i<pBuf->ExtentCount; i++)
	{
		Item.llOffsetVcn = llStart;
		Item.llOffsetLcn = pBuf->Extents[i].Lcn;
		Item.llLength.QuadPart = pBuf->Extents[i].NextVcn.QuadPart - llStart.QuadPart;

		llFirstSector.QuadPart = 0;
		bRetVal = GetSectorByCluster(Item.llOffsetLcn, llFirstSector);
		if(bRetVal == FALSE)
		{
			llFirstSector.QuadPart = -1;
		}
		Item.llFirstSector = llFirstSector;

		m_FileCluster.push_back(Item);
		llStart = pBuf->Extents[i].NextVcn;
	}

	bResult = TRUE;


GetFileLocationDone:
	if(pBuf)
		free(pBuf);

	return bResult;
}


BOOL	CDiskFile::GetSectorByCluster(LARGE_INTEGER llLcn, LARGE_INTEGER &llFirstSector)
{
	VOLUME_LOGICAL_OFFSET		Lgc;
	VOLUME_PHYSICAL_OFFSETS		Phy;
	BOOL			bRetVal;
	DWORD			dwBytesReturned;

	Lgc.LogicalOffset = llLcn.QuadPart * m_dwSectorsPerCluster * m_dwBytesPerSector;
	memset(&Phy, 0, sizeof(Phy));

	bRetVal = DeviceIoControl(m_hVolume, IOCTL_VOLUME_LOGICAL_TO_PHYSICAL,
		&Lgc, sizeof(Lgc), &Phy, sizeof(Phy), &dwBytesReturned,	NULL);
	if(bRetVal == FALSE)
	{
		m_dwError = GetLastError();
		return FALSE;
	}

	llFirstSector.QuadPart = Phy.PhysicalOffset[0].Offset / m_dwBytesPerSector;
	llFirstSector.QuadPart += m_dwFat32FirstDataSector;

	return TRUE;
}


BOOL	CDiskFile::ReadDiskSector(LARGE_INTEGER llFirstSector, LARGE_INTEGER llCount, BYTE *pBuf)
{
	LARGE_INTEGER		llOffset;
	LARGE_INTEGER		llReadCount;
	DWORD				dwRetVal;
	BOOL				bResult;
	DWORD				dwReadSize;

	bResult = FALSE;

	llOffset.QuadPart = llFirstSector.QuadPart * m_dwBytesPerSector;
	dwRetVal = SetFilePointer(m_hDisk, llOffset.LowPart, &llOffset.HighPart, FILE_BEGIN);
	if(dwRetVal == INVALID_SET_FILE_POINTER)
		goto ReadDiskSectorDone;

	llReadCount.QuadPart = llCount.QuadPart * m_dwBytesPerSector;
	if(llReadCount.HighPart != 0)
		assert(0);

	bResult = ReadFile(m_hDisk, pBuf, llReadCount.LowPart, &dwReadSize, NULL);
	if(bResult && llReadCount.QuadPart != dwReadSize)
		bResult = FALSE;

	bResult = TRUE;

ReadDiskSectorDone:

	return bResult;
}


BOOL	CDiskFile::WriteDiskSector(LARGE_INTEGER llFirstSector, LARGE_INTEGER llCount, BYTE *pBuf)
{
	LARGE_INTEGER		llOffset;
	LARGE_INTEGER		llWriteCount;
	DWORD				dwRetVal;
	BOOL				bResult;
	DWORD				dwWriteSize;

	bResult = FALSE;

	llOffset.QuadPart = llFirstSector.QuadPart * m_dwBytesPerSector;
	dwRetVal = SetFilePointer(m_hDisk, llOffset.LowPart, &llOffset.HighPart, FILE_BEGIN);
	if(dwRetVal == INVALID_SET_FILE_POINTER)
		goto WriteDiskSectorDone;

	llWriteCount.QuadPart = llCount.QuadPart * m_dwBytesPerSector;
	if(llWriteCount.HighPart != 0)
		assert(0);

	bResult = WriteFile(m_hDisk, pBuf, llWriteCount.LowPart, &dwWriteSize, NULL);
	if(bResult && llWriteCount.QuadPart != dwWriteSize)
		bResult = FALSE;

	bResult = TRUE;

WriteDiskSectorDone:

	return bResult;
}


BOOL	CDiskFile::ReadDiskCluster(LARGE_INTEGER llFirstCluster, LARGE_INTEGER llCount, BYTE *pBuf)
{
	LARGE_INTEGER		llOffset;
	LARGE_INTEGER		llReadCount;
	DWORD				dwRetVal;
	BOOL				bResult;
	DWORD				dwReadSize;

	bResult = FALSE;

	llOffset.QuadPart = llFirstCluster.QuadPart * m_dwBytesPerSector * m_dwSectorsPerCluster;
	llOffset.QuadPart += m_dwFat32FirstDataSector * m_dwBytesPerSector;

	dwRetVal = SetFilePointer(m_hVolume, llOffset.LowPart, &llOffset.HighPart, FILE_BEGIN);
	if(dwRetVal == INVALID_SET_FILE_POINTER)
		goto ReadDiskClusterDone;

	llReadCount.QuadPart = llCount.QuadPart * m_dwBytesPerSector * m_dwSectorsPerCluster;
	if(llReadCount.HighPart != 0)
		assert(0);

	bResult = ReadFile(m_hVolume, pBuf, llReadCount.LowPart, &dwReadSize, NULL);
	if(bResult && llReadCount.QuadPart != dwReadSize)
		bResult = FALSE;

	bResult = TRUE;

ReadDiskClusterDone:

	return bResult;
}


BYTE *	CDiskFile::GetFileDiskData(CHAR *pFileName, DWORD * pdwSize, int nType)
{
	BOOL			bRetVal;
	BYTE *			pData;
	DWORD			dwSize;

	bRetVal = OpenFile(pFileName);
	if(bRetVal == FALSE)
		return FALSE;

	bRetVal = GetFileCluster();
	if(bRetVal == FALSE)
		return FALSE;

	if(pdwSize)
		*pdwSize = m_dwFileSize;

	pData = GetFileDiskData(&dwSize, nType);

	return pData;
}


BOOL	CDiskFile::OpenDirectory(CHAR *pFilePath)
{
	m_hFile = CreateFileA(pFilePath, 0, 0, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS , NULL);
	if(m_hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	InitInfo(pFilePath);

	BYTE *		pDat1;
	BYTE *		pDat2;
	DWORD		dwSize;

	GetFileCluster();

	pDat1 = GetFileDiskData(&dwSize, 0);
	pDat2 = GetFileDiskData(&dwSize, 1);

	NFile::DumpToFile("D:\\1.dat", pDat1, dwSize);
	NFile::DumpToFile("D:\\2.dat", pDat2, dwSize);

	return TRUE;
}


BOOL	CDiskFile::GetFileDiskData(std::vector<BYTE *> &vData, int nType)
{
	BYTE *			pData;
	BYTE *			pRead;
	DWORD			dwAllocSize;
	FileCluster *	pItem;
	LARGE_INTEGER	llSectorCount;
	LARGE_INTEGER	llClusterCount;
	DWORD			dwRead;

	std::vector<FileCluster>::iterator	Iter;

	vData.clear();

	if(m_FileCluster.size() == 0)
		return FALSE;

	for(Iter = m_FileCluster.begin();
		Iter != m_FileCluster.end();
		Iter ++)
	{
		pItem = &(*Iter);

		dwAllocSize = (m_dwBytesPerSector * m_dwSectorsPerCluster);
		pData = (BYTE *)malloc(dwAllocSize);
		pRead = pData;

		if(nType == 0)
		{
			llClusterCount.QuadPart = pItem->llLength.QuadPart;
			ReadDiskCluster(pItem->llOffsetLcn, llClusterCount, pRead);
			dwRead = (DWORD)(llClusterCount.QuadPart * m_dwBytesPerSector * m_dwSectorsPerCluster);
		}
		else if(nType == 1)
		{
			llSectorCount.QuadPart = pItem->llLength.QuadPart * m_dwSectorsPerCluster;
			ReadDiskSector(pItem->llFirstSector, llSectorCount, pRead);
			dwRead = (DWORD)(llSectorCount.QuadPart * m_dwBytesPerSector);
		}

		vData.push_back(pData);
	}

	return TRUE;
}


BYTE *	CDiskFile::GetFileDiskData(DWORD *pdwReadSize, int nType)
{
	BYTE *			pData;
	BYTE *			pRead;
	DWORD			dwAllocSize;
	FileCluster *	pItem;
	LARGE_INTEGER	llSectorCount;
	LARGE_INTEGER	llClusterCount;
	DWORD			dwRead;

	std::vector<FileCluster>::iterator	Iter;


	if(m_FileCluster.size() == 0)
		return NULL;

	dwAllocSize = (DWORD)m_FileCluster.size();
	dwAllocSize *= (m_dwBytesPerSector * m_dwSectorsPerCluster);
	if(pdwReadSize)
		*pdwReadSize = dwAllocSize;

	pData = (BYTE *)malloc(dwAllocSize);

	pRead = pData;
	for(Iter = m_FileCluster.begin();
		Iter != m_FileCluster.end();
		Iter ++)
	{
		pItem = &(*Iter);

		if(nType == 0)
		{
			llClusterCount.QuadPart = pItem->llLength.QuadPart;
			ReadDiskCluster(pItem->llOffsetLcn, llClusterCount, pRead);
			dwRead = (DWORD)(llClusterCount.QuadPart * m_dwBytesPerSector * m_dwSectorsPerCluster);
		}
		else if(nType == 1)
		{
			llSectorCount.QuadPart = pItem->llLength.QuadPart * m_dwSectorsPerCluster;
			ReadDiskSector(pItem->llFirstSector, llSectorCount, pRead);
			dwRead = (DWORD)(llSectorCount.QuadPart * m_dwBytesPerSector);
		}

		pRead += dwRead;
	}

	return pData;
}



BOOL	CDiskFile::TestFileData(CHAR *pFileName, int nType)
{
	BYTE *		pFileData;
	BYTE *		pDiskData;
	DWORD		dwFileSize;
	DWORD		dwDFileSize;
	BOOL		bResult;
	int			nRetVal;

	bResult = FALSE;
	pFileData = NULL;
	pDiskData = NULL;

	pFileData = NFile::GetFileData(pFileName, &dwFileSize);
	if(pFileData == NULL)
		goto TestFileDataDone;

	pDiskData = GetFileDiskData(pFileName, &dwDFileSize, nType);
	if(pDiskData == NULL)
		goto TestFileDataDone;

	if(dwFileSize != dwDFileSize)
		goto TestFileDataDone;

	nRetVal = memcmp(pFileData, pDiskData, dwFileSize);
	if(nRetVal == 0)
		bResult = TRUE;

TestFileDataDone:

	if(pFileData)
		free(pFileData);
	if(pDiskData)
		free(pDiskData);

	return bResult;
}

//	CDiskFile
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
//	CTcpBase



CTcpBase::CTcpBase()
{
	m_bTempAttach = FALSE;

	m_nConnectTime = 3;
	m_nSendTime = 3;
	m_nRecvTime = 3;

	m_sock = INVALID_SOCKET;

	WSADATA              wsa;

	WSAStartup(MAKEWORD(2,2), &wsa);
	Init();
}

CTcpBase::~CTcpBase()
{
	Clear();
}

BOOL	CTcpBase::Init()
{
	if(m_sock == INVALID_SOCKET || m_sock == NULL)
	{
		m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(m_sock == INVALID_SOCKET)
			return FALSE;
	}

	return TRUE;
}

BOOL	CTcpBase::Clear()
{
	if(m_bTempAttach)
		return TRUE;

	if(m_sock == NULL || m_sock == INVALID_SOCKET)
		return TRUE;
	shutdown(m_sock, 2);
	closesocket(m_sock);
	m_sock = NULL;
	return TRUE;
}

BOOL	CTcpBase::Bind(int nPort, BOOL bReUser)
{
	int		nRetVal;
	sockaddr_in		sin;

	sin.sin_family           = AF_INET;
	sin.sin_port             = htons(nPort);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;

	if(bReUser)
		setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&bReUser, sizeof(bReUser));

	nRetVal = bind(m_sock,(const struct sockaddr *)&sin,sizeof(sin));
	if(nRetVal != 0)
		return FALSE;

	return TRUE;
}



BOOL	CTcpBase::Connect(DWORD dwIp, int nPort)
{
	sockaddr_in		addr;
	int				nRetVal;

	Init();

	addr.sin_family = AF_INET;
	addr.sin_port = htons(nPort);
	addr.sin_addr.S_un.S_addr = dwIp;
	nRetVal = connect(m_sock, (sockaddr *)&addr, sizeof(addr));
	if(nRetVal == SOCKET_ERROR)
		return FALSE;

	return TRUE;
}


BOOL	CTcpBase::Connect(CHAR *pHost, int nPort)
{
	DWORD			dwIp;
	BOOL			bRetVal;

	dwIp = NNet::GetHostIp(pHost);
	bRetVal = Connect(dwIp, nPort);

	return bRetVal;
}


BOOL	CTcpBase::SetTimeOut(int nConnect, int nSend, int nRecv)
{
	if(nConnect != -1)
	{
		m_nConnectTime = nConnect;
	}

	if(nSend != -1)
	{
		m_nSendTime = nSend;
		m_nSendTime *= 1000;
		setsockopt(m_sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&m_nSendTime, sizeof(m_nSendTime)); 
	}
	if(nRecv != -1)
	{
		m_nRecvTime = nRecv;
		m_nRecvTime *= 1000;
		setsockopt(m_sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&m_nRecvTime, sizeof(m_nRecvTime));
	}

	return TRUE;
}


BOOL	CTcpBase::Attach(SOCKET sock, BOOL bTemp)
{
	m_bTempAttach = bTemp;
	m_sock = sock;

	return TRUE;
}


BOOL	CTcpBase::DisConnect()
{
	if(m_sock == NULL || m_sock == INVALID_SOCKET)
		return TRUE;

	BOOL	bLinger = FALSE;
	BOOL	bReUse = TRUE;

	setsockopt(m_sock, SOL_SOCKET, SO_DONTLINGER, (const char*)&bLinger, sizeof(bLinger));

	shutdown(m_sock, 2);

	setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&bReUse, sizeof(bReUse));

	return TRUE;
}

BOOL	CTcpBase::ConnectTimed(CHAR *pHost, int nPort)
{
	int				nRetVal;
	u_long			uFlage;
	struct timeval	LmtTime; 
	fd_set			SockList;
	BOOL			bRetVal;
	int				nError;

	Init();

	uFlage = 1;
	nRetVal = ioctlsocket(m_sock, FIONBIO, &uFlage); 
	if(nRetVal == SOCKET_ERROR)
	{
		nError = WSAGetLastError();
		assert(0);
	}

	bRetVal = Connect(pHost, nPort);
	if(bRetVal == FALSE)
	{
		nError = WSAGetLastError();
		if(WSAEWOULDBLOCK != nError)
            assert(0);
	}

	FD_ZERO(&SockList);
	FD_SET(m_sock, &SockList);

	LmtTime.tv_sec = m_nConnectTime; //连接超时: 秒 
	LmtTime.tv_usec = 0; 
	nRetVal = select(0, 0, &SockList, 0, &LmtTime); 
	if(nRetVal == 1)
		bRetVal = TRUE;
	else
        bRetVal = FALSE;

	uFlage = 0;
	nRetVal = ioctlsocket(m_sock, FIONBIO, &uFlage); 
	assert(nRetVal != SOCKET_ERROR);

	return bRetVal;
}


BOOL	CTcpBase::Send(VOID *pData, int nSize)
{
	return Send(m_sock,	pData, nSize);
}

int		CTcpBase::RecvOne(VOID *pData, int nBufSize)
{
	return RecvOne(m_sock, pData, nBufSize);
}

BOOL	CTcpBase::RecvOneLine(VOID *pData, int nBufSize)
{
	return RecvOneLine(m_sock, pData, nBufSize);
}

BOOL	CTcpBase::RecvSizedData(VOID *pData, int nDataSize)
{
	return RecvSizedData(m_sock, pData, nDataSize);
}


int		CTcpBase::RecvOne(SOCKET sock, VOID *pData, int nBufSize)
{
	int			nRetVal;

	memset(pData, 0, nBufSize);
	nRetVal = recv(sock, (char *)pData, nBufSize, 0);
	if(nRetVal == -1 || nRetVal == 0)
	{
		nRetVal = WSAGetLastError();
		return -1;
	}

	return nRetVal;
}


BOOL	CTcpBase::RecvOneLine(SOCKET sock, VOID *pData, int nBufSize)
{
	int			nRetVal;
	CHAR		c;
	int			nRecved;
	CHAR *		pRcvBuf;

	pRcvBuf = (CHAR *)pData;
	nRecved = 0;
	memset(pData, 0, nBufSize);
	nBufSize --;

	for(;;)
	{
		nRetVal = recv(sock, &c, 1, 0);
		if(nRetVal == -1 || nRetVal == 0)
		{
			nRetVal = WSAGetLastError();
			return FALSE;
		}

		pRcvBuf[nRecved] = c;
		nRecved ++;

		if(c == '\n')
		{
			pRcvBuf[nRecved] = 0;
			return TRUE;
		}

		if(nRecved >= nBufSize)
			return FALSE;
	}

	return TRUE;
}


void	CTcpBase::OnRecvSizedDataPrgs(SOCKET sock, int nRecvSize, int nTotalSize)
{

}

void	CTcpBase::OnSendDataPrgs(SOCKET sock, int nSendSize, int nTotalSize)
{

}


BOOL	CTcpBase::Send(SOCKET sock, VOID *pData, int nSize)
{
	int			nRetVal;
	int			nSends, nRests;

	if(sock == INVALID_SOCKET)
		return FALSE;

	if(nSize == -1)
		nSize = (int)strlen((CONST CHAR *)pData);

	nRests = nSize;
	for(nSends = 0;
		nRests > 0;
		nRests = nSize - nSends)
	{
		nRetVal = send(sock, ((char *)pData)+nSends, nRests, 0);
		if(nRetVal == SOCKET_ERROR)
		{
			return FALSE;
		}

		nSends += nRetVal;
		OnSendDataPrgs(sock, nSends, nSize);
	}

	assert(nRests == 0);

	return TRUE;
}


BOOL	CTcpBase::RecvSizedData(SOCKET sock, VOID *pData, int nDataSize)
{
	int		nRetVal;
	int		nOneSize;
	int		nRecvSize;
	int		nRestSize;
	int		nError;

	nRestSize = nDataSize;
	for(nRecvSize = 0;
		nRestSize > 0;
		nRestSize = nDataSize - nRecvSize)
	{
		nOneSize = min(32*1024, nRestSize);
		nRetVal = recv(sock, (CHAR *)pData+nRecvSize, nOneSize, 0);
		if(nRetVal <= 0)
		{
			nError = WSAGetLastError();
			if(WSAENOBUFS == nError)
			{
				Sleep(1);
				continue;
			}
			return FALSE;
		}

		nRecvSize += nRetVal;
		OnRecvSizedDataPrgs(sock, nRecvSize, nDataSize);
	}

	assert(nRestSize == 0);

	return TRUE;
}


//	CAsyncTcp
////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////
//	CBlockTcp
CBlockTcp::CBlockTcp()
{

}


CBlockTcp::~CBlockTcp()
{

}


BOOL	CBlockTcp::Query(CHAR *pQuery, CHAR *pRecv, int nBufSize)
{
	BOOL	bRetVal;

	bRetVal = Send(pQuery);
	if(bRetVal == FALSE)
		return FALSE;

	bRetVal = RecvOne(pRecv, nBufSize);
	if(bRetVal == FALSE)
		return FALSE;

	return TRUE;
}



//	CBlockTcp
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
//	CPckTcp


CPckTcp::CPckTcp()
{

}

CPckTcp::~CPckTcp()
{

}



struct Pck_Header
{
	CHAR	cFlag;
	BYTE	bHash;
	WORD	wCount;
};


BOOL	CPckTcp::Pck_Send(VOID *pData, int nSize)
{
	BOOL		bRetVal;
	Pck_Header	Header;

	Header.cFlag = '#';
	Header.wCount = (WORD)nSize;
	Header.bHash = (Header.wCount << 8) ^ (Header.wCount & 0xFF);

	bRetVal = Send(&Header, sizeof(Header));
	if(bRetVal == FALSE)
		return FALSE;

	bRetVal = Send(pData, nSize);
	if(bRetVal == FALSE)
		return FALSE;

	return TRUE;
}


BOOL	CPckTcp::Pck_Recv(VOID *pBuffer, int nBufSize)
{
	Pck_Header		Header;
	BOOL			bRetVal;
	BYTE			bHash;
	int				nReadSize;

	for(;;)
	{
		bRetVal = RecvSizedData(&Header, 1);
		if(bRetVal == FALSE)
			return FALSE;

		if(Header.cFlag != '#')
			continue;

		bRetVal = RecvSizedData(&(Header.bHash), 3);
		if(bRetVal == FALSE)
			return FALSE;

		bHash = (Header.wCount << 8) ^ (Header.wCount & 0xFF);
		if(Header.bHash != bHash)
			continue;
	}

	nReadSize = Header.wCount;
	if(nBufSize < nReadSize)
	{
		assert(FALSE);
		nReadSize = nBufSize;
	}

	bRetVal = RecvSizedData(pBuffer, nReadSize);

	return bRetVal;
}


//	CPckTcp
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
//	CAsyncTcp

CAsyncTcp::CAsyncTcp()
{
	m_hListenThread = NULL;
	m_hClientRecvThread = NULL;

	m_sock = NULL;

}


CAsyncTcp::~CAsyncTcp()
{
	StopListen();

	if(m_hListenThread)
	{
		SuspendThread(m_hListenThread);
		CloseHandle(m_hListenThread);
	}
}


void	CAsyncTcp::OnSvrAccept(SOCKET sConSock, sockaddr_in *pClientAddr)
{
	CHAR		szOneBuf[4096];
	int			nRetVal;

	for(;;)
	{
		memset(szOneBuf, 0, sizeof(szOneBuf));
		nRetVal = recv(sConSock, szOneBuf, sizeof(szOneBuf)-1, 0);
		if(nRetVal == -1 || nRetVal == 0)
		{
			nRetVal = WSAGetLastError();
			OnSvrRecvError(sConSock, nRetVal);
			break;
		}

		OnSvrRecvData(sConSock, szOneBuf, nRetVal);
		Sleep(1);
	}

	return;
}


void	CAsyncTcp::OnSvrRecvData(SOCKET sConSock, CHAR *pBuffer, int nSize)
{
	send(sConSock, pBuffer, nSize, 0);
	return;
}

void	CAsyncTcp::OnSvrRecvError(SOCKET sConSock, int nError)
{

}

void	CAsyncTcp::OnSvrDisconnect()
{
	NSys::Trace("OnDisconnect\r\n");
}


void	CAsyncTcp::OnCltRecvData(CHAR *pBuffer, int nSize)
{

}

void	CAsyncTcp::OnRecvSizedDataPrgs(SOCKET sock, int nRecvSize, int nTotalSize)
{

}



BOOL	CAsyncTcp::SetPort(int nPort)
{
	if(nPort != -1)
        m_nPort = nPort;

	return TRUE;
}


BOOL	CAsyncTcp::Listen(int nPort)
{
	sockaddr_in			sin;
	int					nRetVal;
	BOOL				bReusePort;

	SetPort(nPort);

	sin.sin_family           = AF_INET;
	sin.sin_port             = htons(m_nPort);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;

	bReusePort = TRUE;
	setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&bReusePort, sizeof(bReusePort));
	nRetVal = bind(m_sock,(const struct sockaddr *)&sin,sizeof(sin));
	if(nRetVal == SOCKET_ERROR)
		return FALSE;

	nRetVal = listen(m_sock, 5);
	if(nRetVal == SOCKET_ERROR)
		return FALSE;

	return TRUE;
}


//	PTHREAD_START_ROUTINE
DWORD	CALLBACK	CAsyncTcp::QuickListenProc(VOID *pParam)
{
	SOCKET		sClient;
	BOOL		bRetVal;
	CAsyncTcp *	pThis;

	pThis = (CAsyncTcp *)pParam;
	bRetVal = pThis->Init();
	if(bRetVal == FALSE)
		return 1;

	bRetVal = pThis->Listen();
	if(bRetVal == FALSE)
	{
		assert(bRetVal);
		return FALSE;
	}

//	u_long			uFlage;
//	int				nRetVal;
//	timeval			LmtTime;
//	fd_set			SockList;

// 	uFlage = 1;
// 	nRetVal = ioctlsocket(pThis->m_sock, FIONBIO, &uFlage); 
// 	assert(nRetVal != SOCKET_ERROR);

	for(;
		pThis->m_bListenStop == FALSE;
		)
	{
		sockaddr_in			Addr;
		int					nAdrSize;

		nAdrSize = sizeof(Addr);
		sClient = accept(pThis->m_sock, (sockaddr *)&Addr, &nAdrSize);
		if(pThis->m_bListenStop || sClient == SOCKET_ERROR)
		{
			NSys::DebugOut("accept Failed: %d", GetLastError());
			break;
		}

		pThis->m_CurClient = sClient;
		pThis->m_CurClinetAddr = Addr;
		pThis->OnSvrAccept(sClient, &Addr);
// 		FD_ZERO(&SockList);
// 		FD_SET(pThis->m_sock, &SockList);
// 		LmtTime.tv_sec = 1;		//超时: 秒 
// 		LmtTime.tv_usec = 0; 
// 
// 		nRetVal = select(0, 0, &SockList, 0, &LmtTime); 
// 		if(nRetVal == 1)
// 			pThis->OnSvrAccept(sClient);
// 
// 		if(pThis->m_bListenStop)
// 			break;
// 		
// 		Sleep(1);
	}

	return 0;
}


BOOL	CAsyncTcp::QuickListenRun(int nPort)
{
	SetPort(nPort);

	if(m_hListenThread)
		CloseHandle(m_hListenThread);

	m_bListenStop = FALSE;
	m_hListenThread = NSys::QucikRunThread(QuickListenProc, this);

	return TRUE;
}


BOOL	CAsyncTcp::StopListen(BOOL bWait)
{
 	BOOL  bDontLinger = FALSE;

	OnSvrDisconnect();
	m_bListenStop = TRUE;

	setsockopt(m_CurClient, SOL_SOCKET, SO_DONTLINGER, (const char*)&bDontLinger, sizeof(BOOL));
	shutdown(m_CurClient, SD_BOTH);
	closesocket(m_CurClient);
	m_CurClient = NULL;

 	setsockopt(m_sock, SOL_SOCKET, SO_DONTLINGER, (const char*)&bDontLinger, sizeof(BOOL));
	shutdown(m_sock, SD_BOTH);
	closesocket(m_sock);
	m_sock = NULL;

	if(bWait)
        WaitForSingleObject(m_hListenThread, INFINITE);

	return TRUE;
}


DWORD	CALLBACK	CAsyncTcp::CltRecvThread(VOID *pParam)
{
	CHAR		szOneBuf[4096];
	CAsyncTcp *	pThis;
	int			nRetVal;

	pThis = (CAsyncTcp *)pParam;

	for(;;)
	{
		memset(szOneBuf, 0, sizeof(szOneBuf));
		nRetVal = recv(pThis->m_sock, szOneBuf, sizeof(szOneBuf)-1, 0);
		if(nRetVal == -1 || nRetVal == 0)
		{
			nRetVal = WSAGetLastError();
			break;
		}

		pThis->OnCltRecvData(szOneBuf, nRetVal);
		Sleep(1);
	}

	return 0;

}

BOOL	CAsyncTcp::CreateCltRecvThread()
{
	if(m_hClientRecvThread)
		CloseHandle(m_hClientRecvThread);

	m_hClientRecvThread = NSys::QucikRunThread(CltRecvThread, this);

	return TRUE;
}

BOOL	CAsyncTcp::QuickClientRun(CHAR *pHost, int nPort)
{
	BOOL		bRetVal;

	bRetVal = Init();
	bRetVal = ConnectTimed(pHost, nPort);
	if(bRetVal == FALSE)
		return FALSE;

	bRetVal = CreateCltRecvThread();

	return bRetVal;
}


BOOL	CAsyncTcp::ClientDisconnect()
{
	NSys::Trace("ClientDisconnect\r\n");

	shutdown(m_sock, 2);
	closesocket(m_sock);
	m_sock = NULL;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//		CAppIoHandle


CAppIoHandle::CAppIoHandle()
{
	m_hCmdProc = NULL;
	m_hInput = NULL;
	m_hOutput = NULL;
	m_hConIn = NULL;
	m_hConOut = NULL;
	m_bHide = TRUE;

	m_bStop = FALSE;
	m_hGetProc = NULL;

	NSys::EnableProcPrivilege();
}

CAppIoHandle::~CAppIoHandle()
{
	if(m_hGetProc)
	{
		DWORD		dwRetVal;

		m_bStop = TRUE;
		dwRetVal = WaitForSingleObject(m_hGetProc, 10000);
		if(WAIT_TIMEOUT == dwRetVal)
			TerminateThread(m_hGetProc, 0);

        CloseHandle(m_hGetProc);
		m_hGetProc = NULL;
	}

	Exit();
}


BOOL	CAppIoHandle::StartShell(CHAR *pCmdLine)
{
	SECURITY_ATTRIBUTES	sa;
	HANDLE				hWritePipe, hWriteShell;
	HANDLE				hReadPipe, hReadShell;
	STARTUPINFOA		si;
	PROCESS_INFORMATION	pi;
	BOOL				bRetVal;
	BOOL				bResult;

	Exit();
	CrateConsole();
	m_bStop = FALSE;

	bResult = FALSE;

	sa.nLength              = sizeof(sa);
	sa.bInheritHandle       = TRUE;
	sa.lpSecurityDescriptor = NULL;

	if(CreatePipe(&hReadPipe, &hReadShell, &sa, 0)==0)                
	{
		assert(0);
		goto StartShell_Done;
	}

	if(CreatePipe(&hWriteShell, &hWritePipe, &sa, 0)==0)              
	{
		assert(0);
		goto StartShell_Done;
	}


	memset(&si, 0, sizeof(si));
	si.cb           = sizeof(si);
	si.dwFlags      = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.hStdInput    = hWriteShell;
	si.hStdOutput   = hReadShell;
	si.hStdError    = hReadShell;
	si.wShowWindow  = SW_HIDE;
	if(m_bHide)
		si.dwFlags  = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	else
		si.dwFlags  = STARTF_USESTDHANDLES;

	bRetVal = CreateProcessA(NULL, pCmdLine, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
	if(bRetVal == FALSE)
	{
		assert(0);
		goto StartShell_Done;
	}

	m_dwCmdPid = pi.dwProcessId;
/*
	HANDLE		hTmp;
	HANDLE		hWriteShellDump;
	HANDLE		hReadShellDump;
	int			nRetVal;
	DWORD		dwParam[2];

	//复制
	bRetVal = DuplicateHandle(GetCurrentProcess(), hWritePipe, pi.hProcess, &hWriteShellDump, DUPLICATE_SAME_ACCESS, TRUE, 0);
	bRetVal = DuplicateHandle(GetCurrentProcess(), hReadShell, pi.hProcess, &hReadShellDump, DUPLICATE_SAME_ACCESS, TRUE, 0);

//	CloseHandle(hWriteShell);
//	CloseHandle(hReadShell);


	dwParam[1] = STD_INPUT_HANDLE;
	dwParam[0] = (DWORD)hWriteShellDump;
	NSys::RometProcRun(m_dwCmdPid, SetStdHandle, dwParam, 2, &nRetVal);
	dwParam[1] = STD_OUTPUT_HANDLE;
	dwParam[0] = (DWORD)hReadShellDump;
	NSys::RometProcRun(m_dwCmdPid, SetStdHandle, dwParam, 2, &nRetVal);
	dwParam[1] = STD_ERROR_HANDLE;
	dwParam[0] = (DWORD)hReadShellDump;
	NSys::RometProcRun(m_dwCmdPid, SetStdHandle, dwParam, 2, &nRetVal);

	Nntdll::PEB 						PEB;
	Nntdll::RTL_USER_PROCESS_PARAMETERS	ProcParam;
	DWORD				dwSize;
	VOID *				pAddr;

	pAddr = Nntdll::GetPEB(pi.hProcess);
	::ReadProcessMemory(pi.hProcess, pAddr, &PEB, sizeof(PEB), &dwSize);

	pAddr = PEB.ProcessParameters;
	::ReadProcessMemory(pi.hProcess, pAddr, &ProcParam, sizeof(ProcParam), &dwSize);

	ProcParam.WindowFlags = 0;
	ProcParam.WindowFlags = STARTF_USESTDHANDLES;
	::WriteProcessMemory(pi.hProcess, pAddr, &ProcParam, sizeof(ProcParam), &dwSize);
	::ResumeThread(pi.hThread);
*/
	CloseHandle(pi.hThread);

	
// 	bRetVal = AttachConsole(m_dwCmdPid);
// 	assert(bRetVal);

	m_hConIn = CreateFileA("CONIN$", GENERIC_READ | GENERIC_WRITE, 0, &sa, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	assert(m_hConIn != INVALID_HANDLE_VALUE);

	m_hConOut = CreateFileA("CONOUT$", GENERIC_READ | GENERIC_WRITE, 0, &sa, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	assert(m_hConOut != INVALID_HANDLE_VALUE);

	m_hCmdProc = pi.hProcess;
	m_hInput = hWritePipe;
	m_hOutput = hReadPipe;

	m_hGetProc = NSys::QucikRunThread(GetOutputProc, this);

StartShell_Done:

	return TRUE;
}


DWORD CALLBACK	CAppIoHandle::GetOutputProc(VOID *pParam)
{
	CAppIoHandle *		pThis;
	CHAR				szBuffer[4096];
	DWORD				dwRead;
//	BOOL				bRetVal;

	pThis = (CAppIoHandle *)pParam;

	for(;	pThis->m_bStop==FALSE; )
	{
		dwRead = pThis->ReadOutput(szBuffer, sizeof(szBuffer)-1);
		if(dwRead == -1)
			break;

		if(dwRead == 0)
		{
			Sleep(1);
			continue;
		}

		NSys::Trace("%s", szBuffer);
		pThis->OnAppOutput(szBuffer, dwRead);
	}

	pThis->OnAppExit();
	return 0;
}


DWORD	CAppIoHandle::ReadOutput(CHAR *pBuffer, DWORD dwSize)
{
	BOOL	bRetVal;
	DWORD	dwRead;

	bRetVal = PeekNamedPipe(m_hOutput, pBuffer, dwSize, &dwRead, NULL, NULL);
	if(bRetVal == FALSE)
		return -1;

	if(dwRead == 0)
		return 0;

	memset(pBuffer, 0, dwSize);
	ReadFile(m_hOutput, pBuffer, dwSize, &dwRead, NULL);
	return dwRead;
}


BOOL	CAppIoHandle::CrateConsole()
{
	HWND			hWnd;
	BOOL			bRetVal;
	HWINSTA			hWinSta;
	HWINSTA			hOldWinSta;

	hWinSta = CreateWindowStationA(NULL, 0, NULL, NULL);
	hOldWinSta = GetProcessWindowStation();

	bRetVal = SetProcessWindowStation(hWinSta);

	bRetVal = AllocConsole();
	hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);

	bRetVal = SetProcessWindowStation(hOldWinSta);

	CloseHandle(hWinSta);
	CloseHandle(hOldWinSta);

	return TRUE;
}


void	CAppIoHandle::OnAppOutput(VOID *pBuffer, int nSize)
{
	CHAR				szBuffer[4096];

	memcpy(szBuffer, pBuffer, nSize);
	szBuffer[nSize] = 0;

	OnAppOutput(szBuffer);
}


void	CAppIoHandle::OnAppExit()
{
	NSys::Trace("OnAppExit\r\n");

	Exit();
}

void	CAppIoHandle::OnAppOutput(CHAR *pString)
{

}


BOOL	ConsoleInputChar(HANDLE hCon, CHAR c, BOOL bDown = TRUE)
{
	INPUT_RECORD		InRcd;
	SHORT				vk;
	WORD				scan;
	DWORD				dwWrite;

	vk = VkKeyScanA(c);
	scan = MapVirtualKey(vk, 0);

	InRcd.EventType = KEY_EVENT;
	InRcd.Event.KeyEvent.bKeyDown = bDown;
	InRcd.Event.KeyEvent.uChar.AsciiChar = c;
	InRcd.Event.KeyEvent.uChar.UnicodeChar = c;
	InRcd.Event.KeyEvent.wRepeatCount = 1;
	InRcd.Event.KeyEvent.wVirtualKeyCode = vk;
	InRcd.Event.KeyEvent.wVirtualScanCode = scan;
	InRcd.Event.KeyEvent.dwControlKeyState = 0;

	WriteConsoleInput(hCon, &InRcd, 1, &dwWrite);

	return TRUE;
}

BOOL	ConsoleInputString(HANDLE hCon, CHAR *pString)
{
	int			i;

	for(i=0; pString[i]; i++)
	{
		ConsoleInputChar(hCon, pString[i]);
		Sleep(1);
		ConsoleInputChar(hCon, pString[i], FALSE);
	}

	return TRUE;
}


BOOL	CAppIoHandle::InputString(CHAR *pCmdString)
{
	DWORD		dwWrite;
	BOOL		bRetVal;

	if(m_hInput == NULL)
		return FALSE;


	if(pCmdString[0] == '#')
	{
		std::string		sCmdString;

		sCmdString = pCmdString + 1;
		ConsoleInputString(m_hConIn, (CHAR *)sCmdString.c_str());

		return TRUE;
	}

	NSys::Trace("%s", pCmdString);

	bRetVal = WriteFile(m_hInput, pCmdString, (DWORD)strlen(pCmdString), &dwWrite, NULL);

	if(stricmp(pCmdString, "\r\n") != 0)
		return TRUE;

	INPUT_RECORD		InRcd;

	InRcd.EventType = KEY_EVENT;
	InRcd.Event.KeyEvent.bKeyDown = TRUE;
	InRcd.Event.KeyEvent.uChar.AsciiChar = ' ';
	InRcd.Event.KeyEvent.uChar.UnicodeChar = ' ';
	InRcd.Event.KeyEvent.wRepeatCount = 1;
	InRcd.Event.KeyEvent.wVirtualKeyCode = ' ';
	InRcd.Event.KeyEvent.wVirtualScanCode = ' ';

	WriteConsoleInput(m_hConIn, &InRcd, 1, &dwWrite);

	return bRetVal;
}

BOOL	CAppIoHandle::Exit()
{
	NSys::Trace("OnAppExit\r\n");

	if(m_dwCmdPid)
	{
		NSys::TerminateProcTree(m_dwCmdPid);
	}

	NSys::CloseHandleSafe(m_hCmdProc);
	NSys::CloseHandleSafe(m_hInput);
	NSys::CloseHandleSafe(m_hOutput);
	NSys::CloseHandleSafe(m_hConIn);
	NSys::CloseHandleSafe(m_hConOut);

	return TRUE;
}
//	CAppIoHandle
//////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//	CHttpUpdate
CHttpUpdate::CHttpUpdate()
{
	m_UpdateThread = NULL;
	m_sListFile = "Filelist.txt";
	m_nRetryCount = 3;
	m_dwMaxFileTimeDlt = 10;
	m_nLastError = ERROR_SUCCESS;
	m_bStop = FALSE;
}

CHttpUpdate::~CHttpUpdate()
{
	StopUpdate();
	NSys::CloseHandleSafe(m_UpdateThread);
}

BOOL	CHttpUpdate::OnFixFileName(UpdateItem *pItem, std::string &sFileName)
{
	sFileName = pItem->sFileName;
	assert(0);
	return FALSE;
}

BOOL	CHttpUpdate::OnUpdateFileOK(UpdateItem *pItem, int nCurItem, int nTotalCount, BOOL bDownload)
{
	return TRUE;
}

BOOL	CHttpUpdate::OnDownloadFailed(CHAR *pUrlPath, CHAR *SavePath, int nError)
{
	return TRUE;
}

VOID	CHttpUpdate::OnUpdateEnd(int nError)
{
	std::vector<UpdateItem>::iterator		Iter;
	CHAR			szFullPath[MAXPATH];
	UpdateItem *	pOenItem;

	for(Iter = m_UpdateList.begin();
		Iter != m_UpdateList.end();
		Iter ++)
	{
		pOenItem = &*Iter;

		if(pOenItem->bDownLoad == FALSE)
			continue;

		if(NStr::StrTailCmp((CHAR *)pOenItem->sFileName.c_str(), ".dll") != 0 &&
			NStr::StrTailCmp((CHAR *)pOenItem->sFileName.c_str(), ".ocx") != 0
			)
			continue;

		sprintf(szFullPath, "%s\\%s", m_sCheckPath.c_str(), pOenItem->sFileName.c_str());
		NSys::RegisterFile(szFullPath, 10000);
	}

	return;
}

BOOL	CHttpUpdate::CopyOneFile(CHAR *pSorDir, CHAR *pDestDir, CHAR *pName)
{
	CHAR		szSorPath[MAXPATH];
	CHAR		szDestPath[MAXPATH];
	BOOL		bRetVal;

	sprintf(szSorPath, "%s\\%s", pSorDir, pName);
	sprintf(szDestPath, "%s\\%s", pDestDir, pName);
    if (PathFileExistsA(szDestPath))
    {
        SetFileAttributesA(szDestPath,FILE_ATTRIBUTE_NORMAL);
    }
	bRetVal = CopyFileA(szSorPath, szDestPath, FALSE);
	return bRetVal;
}


BOOL	CHttpUpdate::SetUpdateUrl(CHAR *pUpdateUrl, CHAR *pSavePath, CHAR *pCheckPath, CHAR *pListFile)
{
	CHAR			szUpdateUrl[MAXPATH];
	CHAR			szSavePath[MAXPATH];

	strcpy(szUpdateUrl, pUpdateUrl);
	NStr::StrTrimRight(szUpdateUrl, "/ ");

	if(pSavePath)
		strcpy(szSavePath, pSavePath);
	else
		NFile::GetFileInModulePath(szSavePath);
	NStr::StrTrimRight(szSavePath, "/ ");

	m_sUpdateUrl = szUpdateUrl;
	m_sSavePath = szSavePath;

	if(pCheckPath)
		m_sCheckPath = pCheckPath;
	else
		m_sCheckPath = m_sSavePath;
	if(pListFile)
        m_sListFile = pListFile;

	return TRUE;
}


DWORD WINAPI HttpUpdateProc(LPVOID lpThreadParameter)
{
	CHttpUpdate *		pThis;
	BOOL				bRetVal;
	CHAR				szFullUrl[MAXPATH];
	CHAR				szFullSave[MAXPATH];
	int					i;
	int					nTotalCount;
	std::string			sFixName;

	CHttpUpdate::UpdateItem *		pItem;
	std::vector<CHttpUpdate::UpdateItem>::iterator		Iter;

	pThis = (CHttpUpdate *)lpThreadParameter;

	pThis->m_bStop = FALSE;
	bRetVal = pThis->CheckRealUrl();
	if(bRetVal == FALSE)
		goto HttpUpdateProc_End;

	bRetVal = pThis->ParseListFile();
	if(bRetVal == FALSE)
		goto HttpUpdateProc_End;

	nTotalCount = (int)pThis->m_UpdateList.size();
	i = 0;

	for(Iter = pThis->m_UpdateList.begin();
		Iter != pThis->m_UpdateList.end();
		Iter ++)
	{
		if(pThis->m_bStop)
		{
			pThis->m_nLastError = ERROR_CANCELLED;
			goto HttpUpdateProc_End;
		}

		pItem = &*Iter;
		bRetVal = pThis->CheckUpdateNeed(pItem);
		if(bRetVal == FALSE)
		{
			i ++;
			pThis->OnUpdateFileOK(pItem, i, nTotalCount, FALSE);
			pItem->bDownLoad = FALSE;
			continue;
		}

		pThis->OnFixFileName(pItem, sFixName);

		sprintf(szFullUrl, "%s/%s", pThis->m_sRealUrl.c_str(), sFixName.c_str());
		sprintf(szFullSave, "%s\\%s", pThis->m_sSavePath.c_str(), sFixName.c_str());

		bRetVal = pThis->DownloadOneFile(szFullUrl, szFullSave);
		if(bRetVal == FALSE)
		{
			pThis->OnDownloadFailed(szFullUrl, szFullSave, pThis->m_nLastError);
			goto HttpUpdateProc_End;
		}

		i ++;
		pThis->OnUpdateFileOK(pItem, i, nTotalCount, TRUE);
		pItem->bDownLoad = TRUE;
	}

	sprintf(szFullSave, "%s\\%s", pThis->m_sSavePath.c_str(), pThis->m_sListFile.c_str());
	DeleteFileA(szFullSave);

	pThis->CopyUpdateFiles();

HttpUpdateProc_End:
	pThis->OnUpdateEnd(pThis->m_nLastError);
	return 0;
}


BOOL	CHttpUpdate::StartUpdate(BOOL bNewThread)
{
	BOOL		bRetVal;

	if(bNewThread)
	{
		bRetVal = NSys::ThreadOneceRun(HttpUpdateProc, m_UpdateThread, this);
	}
	else
	{
		HttpUpdateProc(this);
	}

	return bRetVal;
}

BOOL	CHttpUpdate::StopUpdate(BOOL bWait)
{
	m_bStop = TRUE;

	m_http.StopAcion();

	if(bWait)
		WaitForSingleObject(m_UpdateThread, INFINITE);
    
	return TRUE;
}


BOOL	CHttpUpdate::CheckRealUrl()
{
	CHAR		szRealUrl[MAXPATH];
	CHAR		szFullUrl[MAXPATH];
	CHAR		szFullPath[MAXPATH];
	CHAR *		pUrl;
	BOOL		bRetVal;
	DWORD		dwSize;
	std::vector<std::string>			vUpdUrl;
	std::vector<std::string>::iterator	Iter;

	NStr::SplitString((CHAR *)m_sUpdateUrl.c_str(), vUpdUrl, ";,");
	szRealUrl[0] = 0;
	for(Iter = vUpdUrl.begin();
		Iter != vUpdUrl.end();
		Iter ++)
	{
		pUrl = (CHAR *)Iter->c_str();
		if(pUrl[0] == 0)
			continue;

		if(m_bStop)
		{
			m_nLastError = ERROR_CANCELLED;
			return FALSE;
		}

		sprintf(szFullUrl, "%s/%s", pUrl, m_sListFile.c_str());
		sprintf(szFullPath, "%s\\%s", m_sSavePath.c_str(), m_sListFile.c_str());
		bRetVal = DownloadOneFile(szFullUrl, szFullPath);
		if(bRetVal == FALSE)
			continue;

		dwSize = NFile::GetFileSize(szFullPath);
		if(dwSize == 0)
			continue;

		m_sRealUrl = pUrl;
		break;
	}

	if(m_sRealUrl.length() == 0)
		return FALSE;

	return TRUE;
}

BOOL	CHttpUpdate::ParseListFile()
{
	BOOL		bRetVal;
	CHAR		szListUrl[MAXPATH];
	CHAR		szSavePath[MAXPATH];
	CHAR *		pFileDat;
	CHAR *		pLastPos;
	CHAR		szOneLine[4096];
	CHAR		szFileInfo[3][1024];
	UpdateItem	Item;
	DWORD		dwSize;

	sprintf(szListUrl, "%s/%s", m_sRealUrl.c_str(), m_sListFile.c_str());
	sprintf(szSavePath, "%s\\%s", m_sSavePath.c_str(), m_sListFile.c_str());

	bRetVal = DownloadOneFile(szListUrl, szSavePath);
	if(bRetVal == FALSE)
		return FALSE;

	if(m_bStop)
	{
		m_nLastError = ERROR_CANCELLED;
		return FALSE;
	}

	pFileDat = (CHAR *)NFile::GetFileData(szSavePath, &dwSize);
	if(pFileDat == NULL)
	{
		m_nLastError = GetLastError();
		return FALSE;
	}

	if(dwSize == 0)
	{
		m_nLastError = ERROR_BAD_FORMAT;
		return FALSE;
	}

	m_UpdateList.clear();
	for(NStr::GetLine(&pLastPos, pFileDat);
		NStr::GetLine(&pLastPos, pFileDat, szOneLine, sizeof(szOneLine));
		)
	{
		if(m_bStop)
		{
			m_nLastError = ERROR_CANCELLED;
			free(pFileDat);
			return FALSE;
		}

		SYSTEMTIME			st;

		if(szOneLine[0] == 0 ||
			szOneLine[0] == ';' || 
			szOneLine[0] == '/')
			continue;

		szFileInfo[0][0] = 0;
		szFileInfo[1][0] = 0;
		szFileInfo[2][0] = 0;

		NStr::SplitString(szOneLine, szFileInfo[0], 3, sizeof(szFileInfo[0]), ";,");

		memset(&st, 0, sizeof(st));
		sscanf(szFileInfo[0], "%02hu%02hu%02hu%02hu%02hu%04hu", 
			&st.wSecond, &st.wMinute, &st.wHour, 
			&st.wDay, &st.wMonth, &st.wYear);
		bRetVal = SystemTimeToFileTime(&st, &Item.ft);
		if(bRetVal == FALSE)
			continue;
//		FileTimeToLocalFileTime(&ft, &Item.ft);		字符串已经是本地时间
        
		Item.sFileName = szFileInfo[1];
		Item.dwFileSize = strtoul(szFileInfo[2], NULL, 10);

		if(Item.sFileName.length() == 0)
			continue;
		if(Item.dwFileSize == 0)
			continue;

		m_UpdateList.push_back(Item);
	}
	free(pFileDat);

	if(m_UpdateList.size() == 0)
	{
		m_nLastError = ERROR_BAD_FORMAT;
		return FALSE;
	}

	return TRUE;
}


BOOL	CHttpUpdate::DownloadOneFile(CHAR *pUrl, CHAR *pSavePath)
{
	BOOL				bRetVal;
	int					i;

	for(i=0; i<m_nRetryCount; i++)
	{
		bRetVal = m_http.DownloadOneFile(pUrl, pSavePath);
		if(bRetVal)
			return TRUE;

		if(m_bStop)
		{
			m_nLastError = ERROR_CANCELLED;
			return FALSE;
		}

		m_nLastError = m_http.GetError();
	}
	return FALSE;
}


BOOL	CHttpUpdate::CheckUpdateNeed(UpdateItem *pItem)
{
	CHAR			szSavePath[MAXPATH];
	BOOL			bRetVal;
	LONGLONG		tmDlt;
	FILETIME		LocalFt;
	BY_HANDLE_FILE_INFORMATION	FileInfo;


	sprintf(szSavePath, "%s\\%s", m_sCheckPath.c_str(), pItem->sFileName.c_str());
	bRetVal = NFile::GetFileInfo(szSavePath, &FileInfo);
	if(bRetVal == FALSE)
		return TRUE;

	if(FileInfo.nFileSizeLow != pItem->dwFileSize)
		return TRUE;

	FileTimeToLocalFileTime(&FileInfo.ftLastWriteTime, &LocalFt);
	tmDlt = *(LONGLONG *)&LocalFt	- *(LONGLONG *)&pItem->ft;
	if(tmDlt < 0)
		tmDlt = -tmDlt;

	tmDlt /= 10000000;	//	FILETIME 转换成秒
	if(tmDlt > m_dwMaxFileTimeDlt)
		return TRUE;

	return FALSE;
}


BOOL WINAPI CopyUpdateProc(VOID *pParam, CHAR *pFolderPath, WIN32_FIND_DATAA *pFileData)
{
	CHttpUpdate *		pThis;
	CHAR *				pDestDir;
	BOOL				bRetVal;

	pThis = (CHttpUpdate *)pParam;
	pDestDir = (CHAR *)pThis->m_sCheckPath.c_str();

	bRetVal = pThis->CopyOneFile(pFolderPath, pDestDir, pFileData->cFileName);
	if(bRetVal == FALSE)
		assert(0);

	return FALSE;
}


BOOL	CHttpUpdate::CopyUpdateFiles()
{
	CHAR		szFind[MAXPATH];

	if(stricmp(m_sCheckPath.c_str(), m_sSavePath.c_str()) == 0)
		return TRUE;

	strcpy(szFind, m_sSavePath.c_str());
	strcat(szFind, "\\*.*");
	NFile::FileFindEnum(CopyUpdateProc, this, szFind);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//	CDynDriver
/////////////////////////////////////////////////////////////////////////////////////////////

CDynDriver::CDynDriver(CHAR *pFilePath)
{
	m_hDriver = NULL;
	m_hSycEvent = NULL;
	m_szDriverPath[0] = 0;
	m_szSvrName[0] = 0;
}


CDynDriver::~CDynDriver()
{
	StopRead();

	if(m_hSycEvent)
		NSys::CloseHandleSafe(m_hSycEvent);
}


BOOL	CDynDriver::OnReadData(VOID *pData, DWORD dwDatSize)
{
	return TRUE;
}


BOOL	CDynDriver::Init(CHAR *pSvrName)
{
	strcpy(m_szSvrName, pSvrName);

	return TRUE;
}

BOOL	CDynDriver::Install(CHAR *pFilePath, CHAR *pSvrName)
{
	BOOL		bRetVal;

	if(pFilePath == NULL)
		return FALSE;

	if(pFilePath[1] != ':')
		NFile::GetFileInModulePath(m_szDriverPath, pFilePath);
	else
		strcpy(m_szDriverPath, pFilePath);

	if(pSvrName)
		strcpy(m_szSvrName, pSvrName);

	if(m_szSvrName[0] == 0)
		NFile::SpliteFilePathEx(m_szDriverPath, NULL, NULL, NULL, NULL, m_szSvrName);

	sprintf(m_szDeviceName, "\\\\.\\%s", m_szSvrName);

	bRetVal = NSys::CreateAndStartService(m_szDriverPath, m_szSvrName, SERVICE_DEMAND_START);

	return bRetVal;
}


BOOL	CDynDriver::InstallByResId()
{
	return FALSE;
}


BOOL	CDynDriver::OpenDevice(CHAR *pDeviceName, DWORD dwDesiredAccess)
{
	DWORD		dwFlage;

	if(pDeviceName)
		strcpy(m_szDeviceName, pDeviceName);

	if(dwDesiredAccess & SYNCHRONIZE)
		dwFlage = FILE_FLAG_OVERLAPPED;
	else
		dwFlage = 0;

	m_hDriver = CreateFileA(m_szDeviceName, dwDesiredAccess, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	if(m_hDriver == INVALID_HANDLE_VALUE)
		return FALSE;

	return TRUE;
}


BOOL	CDynDriver::CloseDevice()
{
	NSys::CloseHandleSafe(m_hDriver);

	return TRUE;
}


BOOL	CDynDriver::DriverCtrl(DWORD dwIndex, VOID *pInBuffer, DWORD dwInSize, 
							   VOID *pOutBuffer, DWORD dwOutBuf, DWORD *pdwRead)
{
	BOOL	bRetVal;

	bRetVal = DeviceIoControl(m_hDriver, dwIndex, pInBuffer, dwInSize, pOutBuffer, dwOutBuf, pdwRead, NULL);

	return bRetVal;
}


BOOL	CDynDriver::StopDriver()
{
	BOOL	bRetVal;

	CloseDevice();

	if(m_szSvrName[0] == 0)
		return FALSE;

	bRetVal = NSys::StopService(m_szSvrName);

	return bRetVal;
}

BOOL	CDynDriver::StartDriver()
{
	BOOL	bRetVal;

	if(m_szSvrName[0] == 0)
		return FALSE;

	bRetVal = NSys::StartUpService(m_szSvrName);

	return bRetVal;
}

BOOL	CDynDriver::UnInstall()
{
	BOOL		bRetVal;

	StopDriver();
	bRetVal = NSys::DeleteService(m_szSvrName);

	return bRetVal;
}


DWORD	CDynDriver::SycRead(VOID *pBuffer, int nBufSize)
{
	DWORD		dwRead;
	OVERLAPPED	SycLap;
	BOOL		bRetVal;
	int			nError;

	if(m_hDriver == NULL ||
		m_hDriver == INVALID_HANDLE_VALUE)
		return 0;

	if(m_hSycEvent == NULL)
		m_hSycEvent = CreateEventA(NULL, FALSE, FALSE, NULL);

	memset(&SycLap, 0, sizeof(SycLap));
	SycLap.hEvent = m_hSycEvent;
	if(SycLap.hEvent == NULL)
		return 0;

	bRetVal = ReadFile(m_hDriver, pBuffer, nBufSize, &dwRead, &SycLap);
	if(bRetVal == FALSE)
	{
		nError = GetLastError();
		if(ERROR_IO_PENDING != nError)
			return 0;
	}

// 	bRetVal = GetOverlappedResult(m_hDriver, &SycLap, &dwRead, FALSE);
// 	{
// 		nError = GetLastError();
// 		if(ERROR_IO_INCOMPLETE != nError)
// 			return 0;
// 	}
// 
// 	WaitForSingleObject(m_hSycEvent, INFINITE);

	bRetVal = GetOverlappedResult(m_hDriver, &SycLap, &dwRead, TRUE);
	if(bRetVal == FALSE)
		return 0;

	OnReadData(pBuffer, (DWORD)SycLap.InternalHigh);

	return dwRead;
}


BOOL	CDynDriver::StopRead()
{
	if(m_hSycEvent)
        SetEvent(m_hSycEvent);

	return TRUE;
}














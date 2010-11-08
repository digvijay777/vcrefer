#pragma once

class CProcessMem
{
public:
	CProcessMem(DWORD dwPid)
	{
		m_pData = NULL;
		m_dwDataSize = 0;

		Open(dwPid);
	}

	CProcessMem(DWORD dwPid, DWORD dwSize)
	{
		Open(dwPid);
		Alloc(dwSize);
	}

	CProcessMem()
	{
		m_hProc = NULL;
		m_pData = NULL;
		m_dwDataSize = 0;
	}

	~CProcessMem()
	{
		Free();
		if(m_hProc)
			CloseHandle(m_hProc);
	}

	HANDLE GetHandle()
	{
		return m_hProc;
	}


	BOOL	Attach(HANDLE Proc)
	{
		m_hProc = Proc;
		m_pData = NULL;
		m_dwDataSize = 0;

		return TRUE;
	}

	BOOL	DeAttach()
	{
		m_hProc = NULL;
		m_pData = NULL;
		m_dwDataSize = 0;

		return TRUE;
	}

	BOOL Open(DWORD dwPid)
	{
		Free();

		if(m_hProc)
			CloseHandle(m_hProc);

		m_hProc = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
		if(m_hProc == NULL)
			return FALSE;

		return TRUE;
	}

	BOOL Open(HWND hWnd)
	{
		DWORD		dwProcId;

		dwProcId = 0;
		GetWindowThreadProcessId(hWnd, &dwProcId);
		if(dwProcId == 0)
			return FALSE;

		return Open(dwProcId);
	}

	BOOL Alloc(DWORD dwSize)
	{
		if(m_hProc == NULL)
			return FALSE;

		Free();

		m_pData = VirtualAllocEx(m_hProc, 0, dwSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if(m_pData == NULL)
			return FALSE;

		m_dwDataSize = dwSize;

		return TRUE;
	}

	BOOL Free()
	{
		if(m_hProc == NULL || m_pData == NULL)
			return FALSE;

		BOOL	bRetVal;
		bRetVal = VirtualFreeEx(m_hProc, m_pData, 0, MEM_RELEASE);
		if(bRetVal == FALSE)
			return FALSE;

		m_pData = NULL;
		return TRUE;
	}

	BOOL Wirte(VOID *pData, DWORD dwSize = 0)
	{
		if(m_hProc == NULL || m_pData == NULL)
			return FALSE;

		if(dwSize == 0)
			dwSize = (DWORD)strlen((CHAR *)pData);

		if(dwSize > m_dwDataSize)
			return FALSE;

		DWORD		dwWriteSize;
		BOOL		bRetVal;
		bRetVal = WriteProcessMemory(m_hProc, m_pData, pData, dwSize, &dwWriteSize);			//复制代码
		if(bRetVal == FALSE || dwSize != dwWriteSize )
			return FALSE;

		return TRUE;
	}

	BOOL Wirte(VOID * pWriteAddr, VOID *pData, DWORD dwSize = 0)
	{
		if(m_hProc == NULL)
			return FALSE;

		if(dwSize == 0)
			dwSize = (DWORD)strlen((CHAR *)pData) + 1;

		DWORD		dwWriteSize;
		BOOL		bRetVal;
		bRetVal = WriteProcessMemory(m_hProc, pWriteAddr, pData, dwSize, &dwWriteSize);
		if(bRetVal == FALSE || dwSize != dwWriteSize )
			return FALSE;

		return TRUE;
	}

	BOOL OpenAndAlloc(DWORD dwPid, DWORD dwSize, VOID *pData = NULL)
	{
		BOOL	bRetVal;

		bRetVal = Open(dwPid);
		if(bRetVal == FALSE)
			return FALSE;

		bRetVal = Alloc(dwSize);
		if(bRetVal == FALSE)
			return FALSE;

		if(pData)
			bRetVal = Wirte(pData, dwSize);

		return bRetVal;
	}

	BOOL Read(VOID *pData, DWORD dwSize)
	{
		if(m_hProc == NULL || m_pData == NULL)
			return FALSE;

		if(dwSize > m_dwDataSize)
			return FALSE;

		DWORD		dwReadSize;
		BOOL		bRetVal;
		bRetVal = ReadProcessMemory(m_hProc, m_pData, pData, dwSize, &dwReadSize);			//复制代码
		if(bRetVal == FALSE || dwSize != dwReadSize )
			return FALSE;

		return TRUE;
	}


	//	从指定地址读取
	DWORD Read(VOID *pAddr, VOID *pData, DWORD dwSize)
	{
		if(m_hProc == NULL || m_pData == NULL)
			return FALSE;

		if(dwSize > m_dwDataSize)
			return FALSE;

		DWORD		dwReadSize;
		BOOL		bRetVal;
		bRetVal = ReadProcessMemory(m_hProc, pAddr, pData, dwSize, &dwReadSize);			//复制代码
		if(bRetVal == FALSE || dwSize != dwReadSize )
			return 0;

		return dwReadSize;
	}



	VOID * GetData()
	{
		return m_pData;
	}

	BOOL KeepMem()
	{
		if(m_pData = NULL)
			return FALSE;

		m_pData = NULL;
		return TRUE;
	}




	HANDLE		m_hProc;
	VOID *		m_pData;
	DWORD		m_dwDataSize;
};
// PipeServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <windows.h>

HANDLE		hStopEvent		= NULL;
HANDLE		hCompletPort		= NULL;		;


#define BUFSIZE 4096

void __cdecl mywait( void )
{
	system("Pause");
}

VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	KillTimer(hwnd, idEvent);
	printf("Set stop event handler.\r\n");
	if(NULL != hStopEvent)
		SetEvent(hStopEvent);
	if(NULL != hCompletPort)
		CloseHandle(hCompletPort);
}

DWORD CALLBACK ThreadProc(LPVOID lpParameter);
DWORD CALLBACK ThreadProc2(LPVOID lpParameter);
void TestNamedPipeNormal();
void TestCompletPort();

int _tmain(int argc, _TCHAR* argv[])
{
	//SetTimer(NULL, 0, 1000 /** 10*/ /** 10*/, TimerProc); // 10分钟后断开
	atexit(mywait);

	// 普通模式
//	TestNamedPipeNormal();

	// 完成端口模型
	TestCompletPort();

	return 0;
}

DWORD CALLBACK ThreadProc(LPVOID lpParameter)
{
	HANDLE		hPipe			= (HANDLE)lpParameter;
	static int	nCount			= 0;
	CHAR		szBuff[512]		= {0};
	DWORD		dwSize			= sizeof(szBuff);
	int			nRef			= ++nCount;

	ReadFile(hPipe, szBuff, dwSize, &dwSize, NULL);
	printf("%d: recv %s\r\n", nRef, szBuff);
	Sleep(1000 * 30);
	printf("%d: send.\r\n", nRef);
	itoa(nRef, szBuff, 10);
	dwSize = strlen(szBuff)+1;
	WriteFile(hPipe, szBuff, dwSize, &dwSize, NULL);
	CloseHandle(hPipe);
	return 0;
}
// 测试普通模式
void TestNamedPipeNormal()
{
	hStopEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

// 	SetTimer(NULL, 1, 1000 * 60 * 10, TimerProc); // 10分钟后断开
// 	atexit(mywait);

	while(true)
	{
		HANDLE			hNamedPipe	= NULL;
		OVERLAPPED		over		= {0};
		BOOL			bRes		= FALSE;
		HANDLE			hThread		= NULL;
		DWORD			dwRead		= 0;

		hNamedPipe = CreateNamedPipe(_T("\\\\.\\pipe\\mytest")
			, PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED
			, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT
			, PIPE_UNLIMITED_INSTANCES 
			, BUFSIZE
			, BUFSIZE
			, NMPWAIT_USE_DEFAULT_WAIT
			, NULL);
		if(INVALID_HANDLE_VALUE == hNamedPipe)
		{
			printf("Create named pipe failed: %d\r\n", GetLastError());
			return;
		}
		printf("Create new named pipe instance.\r\n");
		// 完成端口操作
		over.hEvent = hStopEvent;
		// 等待连接
		bRes = ConnectNamedPipe(hNamedPipe, &over);
		printf("ConnectNamedPipe %d(%d).\r\n", bRes, GetLastError());
		if(FALSE == GetOverlappedResult(hNamedPipe, &over, &dwRead, TRUE))
		{
			printf("Wait connect failed: %d.\r\n", GetLastError());
			goto close;
		}
		// 创建线程处理操作
		hThread = CreateThread(NULL, 0, ThreadProc, (LPVOID)hNamedPipe, 0, NULL);
		CloseHandle(hThread);

		continue;
		// 关闭操作
close:
		if(NULL != hNamedPipe && INVALID_HANDLE_VALUE != hNamedPipe)
			CloseHandle(hNamedPipe);
	}
}

typedef struct _PipeOVERLAPPED{
	OVERLAPPED			over;
	HANDLE				hPipe;
}PIPEOVERLAPPED, *LPPIPEOVERLAPPED;
// 测试完成端口
void TestCompletPort()
{
	HANDLE				hThreads[5]			= {0};
	HANDLE				hNamePipes[5]		= {0};
	PIPEOVERLAPPED		over				= {0};
	PIPEOVERLAPPED		pipeovers[5]		= {0};

	hStopEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	// 创建完成端口
	hCompletPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	if(NULL == hCompletPort)
	{
		printf("CreateIoCompletionPort failed: %d\n", GetLastError());
		return;
	}
	// 创建线程
	for(int i = 0; i < 5; i++)
	{
		hThreads[i] = CreateThread(NULL, 0, ThreadProc2, (LPVOID)hCompletPort, 0, NULL);
	}
	// 创建NamePipe
	for(int i = 0; i < 5; i++)
	{
		hNamePipes[i] = CreateNamedPipe(_T("\\\\.\\pipe\\mytest")
			, PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED
			, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT
			, PIPE_UNLIMITED_INSTANCES 
			, BUFSIZE
			, BUFSIZE
			, NMPWAIT_USE_DEFAULT_WAIT
			, NULL);
		if(INVALID_HANDLE_VALUE == hNamePipes[i])
		{
			printf("Create NO.%d named pipe failed: %d\r\n", i, GetLastError());
			PostQueuedCompletionStatus(hCompletPort, 0, 0, (LPOVERLAPPED)&over);
			continue;
		}

		BOOL				bRes			= FALSE;
		
		// 关连端口
		if(NULL == CreateIoCompletionPort(hNamePipes[i], hCompletPort, i, 0))
		{
			printf("Create io NO.%d named pipe failed: %d\r\n", i, GetLastError());
			PostQueuedCompletionStatus(hCompletPort, 0, 0, (LPOVERLAPPED)&over);
			CloseHandle(hNamePipes[i]);
			continue;
		}

		pipeovers[i].hPipe = hNamePipes[i];
		//hThreads[i] = CreateThread(NULL, 0, ThreadProc2, (LPVOID)hCPNamedPipe, 0, NULL);
		bRes = ConnectNamedPipe(hNamePipes[i], (LPOVERLAPPED)&pipeovers[i]);
		if(FALSE == bRes && ERROR_IO_PENDING != GetLastError())
		{
			printf("Connect NO.%d named pipe failed: %d\r\n", i, GetLastError());
			PostQueuedCompletionStatus(hCompletPort, 0, 0, (LPOVERLAPPED)&over);
			CloseHandle(hNamePipes[i]);
			continue;
		}
	}
	// 等待完成
	WaitForMultipleObjects(5, hThreads, TRUE, INFINITE);
	for(int i = 0; i < 5; i++)
	{
		CloseHandle(hThreads[i]);
		CloseHandle(hNamePipes[i]);
	}
	CloseHandle(hCompletPort);
	printf("工作完成！\n");
}

DWORD CALLBACK ThreadProc2(LPVOID lpParameter)
{
	HANDLE				hComplet		= (HANDLE)lpParameter;
	DWORD				dwRead			= 0;
	ULONG_PTR			pKey			= NULL;
	LPPIPEOVERLAPPED	lpOver			= NULL;
	BOOL				bRes			= FALSE;
	char				szBuff[4096]	= {0};
	DWORD				dwSize			= 0;
	int					nRef			= 0;

	while(true)
	{
		pKey = NULL;
		bRes = GetQueuedCompletionStatus(hComplet, &dwRead, &pKey
			, (LPOVERLAPPED *)&lpOver, INFINITE);
		if(NULL == lpOver)
			continue;
		if(NULL == lpOver->hPipe)
			break;

		dwSize = sizeof(szBuff);
		memset(szBuff, 0, sizeof(szBuff));
		ReadFile(lpOver->hPipe, szBuff, dwSize, &dwSize, NULL);
		printf("(%d)%d: recv %s\r\n", GetCurrentThreadId(), nRef, szBuff);
		Sleep(1000 * 10);
		printf("(%d)%d: send.\r\n", GetCurrentThreadId(), nRef);
		itoa(nRef++, szBuff, 10);
		dwSize = strlen(szBuff)+1;
		WriteFile(lpOver->hPipe, szBuff, dwSize, &dwSize, NULL);
		// 下一个连接	
		DisconnectNamedPipe(lpOver->hPipe);
		ConnectNamedPipe(lpOver->hPipe, (LPOVERLAPPED)lpOver);
	}
	return 0;
}
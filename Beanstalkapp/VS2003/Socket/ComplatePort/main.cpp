#include <afxwin.h>
#include <afxsock.h>

typedef struct _OVERLAPPEDPLUS {
	OVERLAPPED        ol;
	SOCKET            s, sclient;
	int               OpCode;
	WSABUF            wbuf;
	DWORD             dwBytes, dwFlags;
	// 其它有用的信息
} OVERLAPPEDPLUS;

#define OP_READ     0
#define OP_WRITE    1
#define OP_ACCEPT   2


int AfxMessageBox2(LPCTSTR lpFormat, ...); 
DWORD WINAPI ThreadComplatePort(LPVOID lpParameter);
HANDLE		hIocp;

int WinMain( IN HINSTANCE hInstance, IN HINSTANCE hPrevInstance, IN LPSTR lpCmdLine, IN int nShowCmd )
{
	if(!AfxSocketInit())
	{
		AfxMessageBox2("初始化套接字错误");
		return -1;
	}

	// 创建完成端口
	hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, (ULONG_PTR)0, 0);
	if(NULL == hIocp)
	{
		AfxMessageBox2("CreateIoCompletionPort failed: %d", GetLastError());
		return -1;
	}

	// 创建SOKECT
	SOCKET			server;
	SOCKADDR_IN		addr		= {0};
	int				nRet;

	server = socket(AF_INET, SOCK_STREAM, 0);
	if(INVALID_SOCKET == server)
	{
		AfxMessageBox2("socket failed: %d", WSAGetLastError());
		return -1;
	}
	addr.sin_port = htons(8603);
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	//梆定
	nRet = bind(server, (SOCKADDR *)&addr, sizeof(SOCKADDR));
	if(0 != nRet)
	{
		AfxMessageBox2("bind error:%d\n", WSAGetLastError());
		return -1;
	}
	//监听
	nRet = listen(server, SOMAXCONN);
	if(0 != nRet)
	{
		AfxMessageBox2("listen error:%d\n", WSAGetLastError());
		return -1;
	}

	// 关联操作
	if(NULL == CreateIoCompletionPort((HANDLE)server, hIocp, (ULONG_PTR)0, 0))
	{
		AfxMessageBox2("关联操作失败: %d", GetLastError());
		return -1;
	}
	return 0;
}

// 完成端口事件
DWORD WINAPI ThreadComplatePort(LPVOID lpParameter)
{
	ULONG_PTR       *PerHandleKey;
	OVERLAPPED      *Overlap;
	OVERLAPPEDPLUS  *OverlapPlus,
					*newolp;
	DWORD           dwBytesXfered;
	BOOL			ret;

	while (1)
	{
		ret = GetQueuedCompletionStatus(
			hIocp,
			&dwBytesXfered,
			(PULONG_PTR)&PerHandleKey,
			&Overlap,
			INFINITE);
		if (ret == 0)
		{
			// Operation failed
			continue;
		}
		OverlapPlus = CONTAINING_RECORD(Overlap, OVERLAPPEDPLUS, ol);

		switch (OverlapPlus->OpCode)
		{
		case OP_ACCEPT:
			// Client socket is contained in OverlapPlus.sclient
			// Add client to completion port
			CreateIoCompletionPort(
				(HANDLE)OverlapPlus->sclient,
				hIocp,
				(ULONG_PTR)0,
				0);

			//  Need a new OVERLAPPEDPLUS structure
			//  for the newly accepted socket. Perhaps
			//  keep a look aside list of free structures.
	//		newolp = AllocateOverlappedPlus();
			if (!newolp)
			{
				// Error
			}
			newolp->s = OverlapPlus->sclient;
			newolp->OpCode = OP_READ;

			// This function prepares the data to be sent
	//		PrepareSendBuffer(&newolp->wbuf);

			ret = WSASend(
				newolp->s,
				&newolp->wbuf,
				1,
				&newolp->dwBytes,
				0,
				&newolp->ol,
				NULL);

			if (ret == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSA_IO_PENDING)
				{
					// Error
				}
			}

			// Put structure in look aside list for later use
//			FreeOverlappedPlus(OverlapPlus);

			// Signal accept thread to issue another AcceptEx
//			SetEvent(hAcceptThread);
			break;

		case OP_READ:
			// Process the data read    
			// ...

			// Repost the read if necessary, reusing the same
			// receive buffer as before
			memset(&OverlapPlus->ol, 0, sizeof(OVERLAPPED));
			ret = WSARecv(
				OverlapPlus->s,
				&OverlapPlus->wbuf,
				1,
				&OverlapPlus->dwBytes,
				&OverlapPlus->dwFlags,
				&OverlapPlus->ol,
				NULL);

			if (ret == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSA_IO_PENDING)
				{
					// Error
				}
			}
			break;

		case OP_WRITE:
			// Process the data sent, etc.
			break;
		} // switch
	} // while
	return 0;
}
// 格试化对话框
int AfxMessageBox2(LPCTSTR lpFormat, ...)
{
	TCHAR		szMsg[2048]		= {0};
	va_list		arglist;

	va_start(arglist, lpFormat);
	_vsnprintf(szMsg, 2048 * sizeof(TCHAR), lpFormat, arglist);
	va_end(arglist);
	return ::MessageBox(NULL, szMsg, "ComplatePort", MB_OK);
}
#include "stdafx.h"
#include "proxysocks4svr.h"
#include <cassert>
#include <algorithm>


#define THREAD_WORKNUM				10

#ifndef TRACELOG
#include <atltrace.h>
#define TRACELOG		TRACE
#endif

CProxySocks4Svr::CProxySocks4Svr(void)
{
}

CProxySocks4Svr::~CProxySocks4Svr(void)
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// CSocksServer

CSocksServer::CSocksServer()
{
	m_hCompetionPort = NULL;
	m_hThreadAccept = NULL;
	m_nPort = 1080;
	m_bRunning = FALSE;
	m_sockSvr = INVALID_SOCKET;
}

CSocksServer::~CSocksServer()
{
	if(NULL != m_hCompetionPort)
		CloseHandle(m_hCompetionPort);
	if(NULL != m_hThreadAccept)
		CloseHandle(m_hThreadAccept);
}

int CSocksServer::StartServer(int nPort /* = 1080 */)
{
	assert(NULL == m_hThreadAccept);

	m_nPort = nPort;
	m_bRunning = TRUE;
	m_hThreadAccept = CreateThread(NULL, 0, ThreadAccept, this, 0, NULL);
	return GetLastError();
}

/*
 *	�����߳�
 */
DWORD CSocksServer::ThreadAccept(LPVOID lpParameter)
{
	CSocksServer*	pThis		= (CSocksServer *)lpParameter;
	
	return pThis->OnAccept();
}

/*
 *	���չ���
 */
DWORD CSocksServer::OnAccept()
{
	// ������ɶ˿�
	m_hCompetionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, NULL, 0);
	if(NULL == m_hCompetionPort)
	{
		TRACELOG("Create Io Completion Port failed: %d\n", GetLastError());
		return -1;
	}

	// ���������߳�
// 	SYSTEM_INFO			sysInfo;
	HANDLE				hThreadWork[THREAD_WORKNUM];

	assert(THREAD_WORKNUM < 64);
// 	GetSystemInfo(&sysInfo);
	for(int i = 0; i < sizeof(hThreadWork); i++)
	{
		hThreadWork[i] = CreateThread(NULL, 0, ThreadWork, this, 0, NULL);
		assert(NULL != hThreadWork[i]);
	}

	// ����SOCKET
	SOCKADDR_IN		addr;
	int				iErr		= 0;

	m_sockSvr = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if(INVALID_SOCKET == m_sockSvr)
	{
		TRACELOG("WSASocket failed: %d\n", WSAGetLastError());
		return WSAGetLastError();
	}

	// ��SOCKET
	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(m_nPort);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	iErr = bind(m_sockSvr, (SOCKADDR*)&addr, sizeof(addr));
	if(SOCKET_ERROR == iErr)
	{
		TRACELOG("bind port %d failed: %d\n", m_nPort, WSAGetLastError());
		return WSAGetLastError();
	}
	
	// ����SOCKET
	iErr = listen(m_sockSvr, SOMAXCONN);
	if(SOCKET_ERROR == iErr)
	{
		TRACELOG("listen failed: %d\n", WSAGetLastError());
		return WSAGetLastError();
	}

	// ����
	SOCKET		sockAccept;
	while(m_bRunning)
	{
		sockAccept = WSAAccept(m_sockSvr, NULL, NULL, NULL, 0);
		
		// �߳��Ƿ��˳�
		if((FALSE == m_bRunning))
			break;
		if(INVALID_SOCKET == sockAccept)
		{
			TRACELOG("WSAAccept failed: %d\n", WSAGetLastError());
			continue;
		}

		// �����շ��͵������߳�
		WSAOVERLAPPEDPROXY*		pOverLapped		= (WSAOVERLAPPEDPROXY *)malloc(sizeof(WSAOVERLAPPEDPROXY));
		DWORD					dwRecv			= 0;
		DWORD					dwFlags			= 0;

		if(NULL == pOverLapped)
		{
			TRACELOG("malloc WSAOVERLAPPEDEX failed: %d\n", GetLastError());
			closesocket(sockAccept);
			continue;
		}
		m_vctWSAOL.push_back(pOverLapped);
		pOverLapped->WSARequest.WSABuf.buf = pOverLapped->WSARequest.szRawBuff;
		pOverLapped->WSARequest.WSABuf.len = sizeof(pOverLapped->WSARequest.szRawBuff);
		pOverLapped->WSAConnect.WSABuf.buf = pOverLapped->WSAConnect.szRawBuff;
		pOverLapped->WSAConnect.WSABuf.len = sizeof(pOverLapped->WSAConnect.szRawBuff);
		pOverLapped->WSARequest.sock = sockAccept;
		pOverLapped->WSAConnect.sock = INVALID_SOCKET;
		pOverLapped->nProxyVersion = 0x0;
		CreateIoCompletionPort((HANDLE)sockAccept, m_hCompetionPort, (DWORD)sockAccept, 0);
		if(SOCKET_ERROR == WSARecv(sockAccept, &pOverLapped->WSARequest.WSABuf, 1, &dwRecv, 
			&dwFlags, (LPWSAOVERLAPPED)pOverLapped, NULL))
		{
			if(WSA_IO_PENDING != WSAGetLastError())
			{
				TRACELOG("WSARecv failed: %d\n", WSAGetLastError());
			}
		}
	}

	// �˳�������
	closesocket(m_sockSvr);
	m_sockSvr = INVALID_SOCKET;

	// �ȴ��������߳̽���
	WaitForMultipleObjects(sizeof(hThreadWork), hThreadWork, TRUE, INFINITE);

	// ����ڴ�
	for(int i = 0; i < m_vctWSAOL.size(); i++)
	{
		free(m_vctWSAOL[i]);
	}
	m_vctWSAOL.clear();

	return 0;
}
/*
 *	�������߳�
 */
DWORD CSocksServer::ThreadWork(LPVOID lpParameter)
{
	CSocksServer*		pThis		= (CSocksServer *)lpParameter;

	return pThis->OnWork();
}

/*
 *	����
 */
DWORD CSocksServer::OnWork()
{	
	BOOL					bRes;
	DWORD					dwSize;
	SOCKET					sock;
	LPWSAOVERLAPPEDPROXY		lpOvlp;

	while(m_bRunning)
	{
		sock = INVALID_SOCKET;
		lpOvlp = NULL;
		dwSize = 0;
		bRes = FALSE;

		// ��ѯ����
		bRes = GetQueuedCompletionStatus(m_hCompetionPort, &dwSize, (LPDWORD)&sock, 
			(LPWSAOVERLAPPED *)&lpOvlp, INFINITE);
		if((FALSE == bRes) || 
			(INVALID_SOCKET == sock) || 
			(NULL == lpOvlp))
		{
			TRACELOG("GetQueuedCompletionStatus failed: %d\n", WSAGetLastError());
			continue;
		}
		
		// 
	}

	return 0;
}

/*
 *	�ر�SOCKET
 */
void CSocksServer::CloseSocket(LPWSAOVERLAPPEDPROXY lpOvlp, SOCKET sock)
{
	using namespace std;

	if(sock == lpOvlp->WSAConnect.sock)
	{
		closesocket(lpOvlp->WSAConnect.sock);
		lpOvlp->WSAConnect.sock = INVALID_SOCKET;
		if(INVALID_SOCKET != lpOvlp->WSARequest.sock)
			shutdown(lpOvlp->WSARequest.sock, SD_BOTH);
	}
	else if(sock == lpOvlp->WSARequest.sock)
	{
		closesocket(lpOvlp->WSARequest.sock);
		lpOvlp->WSARequest.sock = INVALID_SOCKET;
		if(INVALID_SOCKET != lpOvlp->WSAConnect.sock)
			shutdown(lpOvlp->WSAConnect.sock, SD_BOTH);
	}

	if((INVALID_SOCKET != lpOvlp->WSAConnect.sock) ||
		(INVALID_SOCKET != lpOvlp->WSARequest.sock))
		return;

	vector<LPWSAOVERLAPPEDPROXY>::iterator		item;
	item = find(m_vctWSAOL.begin(), m_vctWSAOL.end(), lpOvlp);
	if(item != m_vctWSAOL.end())
		m_vctWSAOL.erase(item);
}
/*
 *	ֹͣ����
 */
void CSocksServer::Stop()
{
	m_bRunning = FALSE;

	shutdown(m_sockSvr, SD_BOTH);
	WaitForSingleObject(m_hThreadAccept, INFINITE);
}
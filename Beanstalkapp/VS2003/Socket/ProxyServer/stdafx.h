#pragma once


#include <WinSock2.h>
#include <winerror.h>
#include <tchar.h>
#include <windows.h>
#include <strsafe.h>


#define str_ServiceName					"ProxyServer"						// ������
#define str_ServiceDisplay				"Socks5���������"					// ��ʾ��
#define str_ServiceDescription			"�򵥵�Socks5���������"			// ����


#ifndef TRACELOG
#include <Atlbase.h>
#define TRACELOG		ATLTRACE
#endif
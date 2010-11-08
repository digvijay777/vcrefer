#pragma once


#include <WinSock2.h>
#include <winerror.h>
#include <tchar.h>
#include <windows.h>
#include <strsafe.h>


#define str_ServiceName					"ProxyServer"						// 服务名
#define str_ServiceDisplay				"Socks5代理服务器"					// 显示名
#define str_ServiceDescription			"简单的Socks5代理服务器"			// 描述


#ifndef TRACELOG
#include <Atlbase.h>
#define TRACELOG		ATLTRACE
#endif
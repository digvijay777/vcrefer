#pragma once

#ifndef _NTDDK_
#include <WinIoCtl.h>
#endif

#define MAXPATHLEN 1024

/*
 *	开启监控
 *  无输入输出参数
 */
#define IOCTRL_REGMON_START			CTL_CODE(\
	FILE_DEVICE_UNKNOWN \
	, 0x800 \
	, METHOD_BUFFERED \
	, FILE_ANY_ACCESS)

/*
 *	关闭监控
 *  无输入输出参数
 */
#define IOCTRL_REGMON_STOP			CTL_CODE(\
	FILE_DEVICE_UNKNOWN \
	, 0x801 \
	, METHOD_BUFFERED \
	, FILE_ANY_ACCESS)

// 发现一个非法操作结构
struct FindBadOptItem
{
	WCHAR		szRegPath[MAXPATHLEN+4];
	WCHAR		szExePath[512];
	WCHAR		szName[MAXPATHLEN];
	WCHAR		szValue[MAXPATHLEN];
	ULONG		nPID;
	ULONG		Type;
};


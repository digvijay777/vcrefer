#pragma once

#ifndef _NTDDK_
#include <WinIoCtl.h>
#endif

#define MAXPATHLEN 1024

/*
 *	�������
 *  �������������
 */
#define IOCTRL_REGMON_START			CTL_CODE(\
	FILE_DEVICE_UNKNOWN \
	, 0x800 \
	, METHOD_BUFFERED \
	, FILE_ANY_ACCESS)

/*
 *	�رռ��
 *  �������������
 */
#define IOCTRL_REGMON_STOP			CTL_CODE(\
	FILE_DEVICE_UNKNOWN \
	, 0x801 \
	, METHOD_BUFFERED \
	, FILE_ANY_ACCESS)

// ����һ���Ƿ������ṹ
struct FindBadOptItem
{
	WCHAR		szRegPath[MAXPATHLEN+4];
	WCHAR		szExePath[512];
	WCHAR		szName[MAXPATHLEN];
	WCHAR		szValue[MAXPATHLEN];
	ULONG		nPID;
	ULONG		Type;
};


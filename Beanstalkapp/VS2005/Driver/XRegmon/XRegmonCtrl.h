#pragma once

#ifndef _NTDDK_
#include <WinIoCtl.h>
#endif

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
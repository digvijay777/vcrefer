#pragma once

#ifndef _NTDDK_
#include <WinIoCtl.h>
#endif

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
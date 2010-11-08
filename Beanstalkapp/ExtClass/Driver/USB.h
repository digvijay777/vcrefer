#pragma once
#include <Setupapi.h>
#include <winioctl.h>
#include <cfgmgr32.h>
#include <regstr.h>   
#include <initguid.h>

namespace usb
{
	BOOL			EjectUdisk(CHAR cDisk, int nWaitTime = 1000);

	DEVINST			GetDrivesDevInstByDiskNumber(long DiskNumber, UINT DriveType);
}
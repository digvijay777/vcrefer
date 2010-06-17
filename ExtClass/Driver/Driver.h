#pragma once
#include <vector>
#include <string>

DEFINE_GUID(GUID_DEVINTERFACE_DISK,                   0x53f56307L, 0xb6bf, 0x11d0, 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b);

#define DiskClassGuid               GUID_DEVINTERFACE_DISK

namespace XDriver
{
	int GetDevicePath(LPGUID lpGuid, std::vector<std::string>& vctPath, int nMaxCount = 10);
	int GetDevicePath2(LPGUID lpGuid, std::vector<std::string>& vctPath);
}


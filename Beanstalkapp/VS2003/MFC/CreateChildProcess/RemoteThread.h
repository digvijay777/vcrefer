#pragma once

#pragma pack(push)
#pragma pack(1)
struct _RemoteThreadInfo
{
	CHAR	szCmd[1024];

	void*	fnCreateThread();
	void*	fnCloseHandle();
};
#pragma pack(pop)

#pragma code_seg("sharecode")
static void WINAPI _RemoteThreadCallFn(LPVOID lpParameter);
#pragma code_seg("sharecode")
static void WINAPI _RemoteThreadCallFn_End();


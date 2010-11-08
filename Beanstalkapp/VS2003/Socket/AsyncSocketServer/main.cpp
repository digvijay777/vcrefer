// #include <afxwin.h>
#include "MySocket.h"

DWORD		g_dwMainThreadId	= 0;

int main(int, char **, char **)
{
	EXCUTEFALSEFUN(AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), FALSE), "³õÊ¼»¯MFC¿âÊ§°Ü");
	EXCUTEFALSEFUN(AfxSocketInit(), "³õÊ¼»¯ÍøÂçÌ×½Ó×ÖÊ§°Ü");

	g_dwMainThreadId = ::GetCurrentThreadId();
	if(FALSE == CreateSockeServer())
	{
		PostThreadMessage(g_dwMainThreadId, WM_QUIT, 0, 0);
	}
	MSG		msg;
	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	g_pSocket->Close();
	WSACleanup();
	system("pause");
	return 0;
}

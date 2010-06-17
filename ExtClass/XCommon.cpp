#include "stdafx.h"
#include "XCommon.h"

/*
 *	����Ϣѭ���ĵȴ�
 */

void ext::WaitObjectWithMsgLoop(HANDLE hHandle, DWORD dwMilliseconds)
{
	DWORD			dwWait		= 0;
	MSG				msg;

	do 
	{
		dwWait = MsgWaitForMultipleObjectsEx(1, &hHandle, dwMilliseconds, QS_ALLEVENTS, MWMO_INPUTAVAILABLE);

		switch(dwWait)
		{
		case WAIT_OBJECT_0:
			break;
		default: // ��Ϣ
			while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if(WM_QUIT == msg.message)
					return;

				if((NULL != AfxGetApp()) && (AfxGetApp()->PumpMessage()))
				{
					continue;
				}

				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
			break;
		}
	} while (WAIT_OBJECT_0 != dwWait);

}
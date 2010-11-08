#include "stdafx.h"

#include "../../../ExtClass/System/ServiceApp.h"

#pragma comment(lib, "Ws2_32.lib")

void RunService(DWORD argc, CHAR* argv[]);

int main(int argc, char* argv[])
{
	CServiceApp		app(str_ServiceName);

	app.SetServiceInfo(str_ServiceDisplay, str_ServiceDescription, TRUE);

	if(argc > 1)
	{
		if(stricmp(&argv[1][1], "create") == 0)
		{
			return app.Create();
		}
		else if(stricmp(&argv[1][1], "start") == 0)
		{
			return app.Start();
		}
	}
	
	app.RunServer(RunService);
	return 0;
}

/*
 *	服务控制函数
 */
void RunService(DWORD argc, CHAR* argv[])
{
	// __asm int 3;
}
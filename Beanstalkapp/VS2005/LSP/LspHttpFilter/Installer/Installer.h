#pragma once

#include <stdio.h>
#include <ws2spi.h>
#include <sporder.h>
#include <TCHAR.H>
#include <stdlib.h>
//
//����Ԥ����
//
void InstallProvider(void);
void RemoveProvider(void);
BOOL GetHookProvider(IN	WSAPROTOCOL_INFOW *pProtocolInfo
					 , OUT TCHAR *sPathName, OUT WSAPROTOCOL_INFOW *pNextProtocolInfo);


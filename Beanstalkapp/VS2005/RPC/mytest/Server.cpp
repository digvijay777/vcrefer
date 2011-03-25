#include <stdlib.h>
#include <stdio.h>
#include "test.h"

void Hello(const unsigned char *psz)
{
	printf("%s\n", psz);
}

void Shutdown()
{
	RpcMgmtStopServerListening(NULL);
	RpcServerUnregisterIf(NULL, NULL, FALSE);
}

int main(int argc, char* argv[])
{
	// ��Named Pipe ��ΪRPC ��ͨ��������EndPoint ��������Named Pipe ������
	// ����Named Pipe �������淶��\pipe\pipename������pipename �����ǳ���\ 
	// ֮��������ַ�����ô������һ��GUID �������������Ա�֤�����ظ�
	//RpcServerUseProtseqEp((unsigned char *)"ncacn_np", 20, (unsigned char *)"\\pipe\\{8dd50205-3108-498f-96e8-dbc4ec074cf9}", NULL);    
	RpcServerUseProtseqEp((unsigned char *)"ncalrpc", RPC_C_PROTSEQ_MAX_REQS_DEFAULT, (unsigned char *)"MyTest", NULL);

	// ע��ӿڣ�HelloWorld_v1_0_s_ifspec ����MIDL ���ɵ�Hello.h �ж����
	RpcServerRegisterIf(HelloWorld_v1_0_s_ifspec, NULL, NULL);

	// ��ʼ��������������һֱ����
	RpcServerListen(1,20,FALSE);

	return 0;
}

// ����ĺ�����Ϊ������������Ҫ��д�ģ�û�еĻ���������Ӵ���
void __RPC_FAR* __RPC_USER midl_user_allocate(size_t len)
{
	return(malloc(len));
}

void __RPC_USER midl_user_free(void __RPC_FAR *ptr)
{
	free(ptr);
}
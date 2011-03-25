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
	// 用Named Pipe 作为RPC 的通道，这样EndPoint 参数就是Named Pipe 的名字
	// 按照Named Pipe 的命名规范，\pipe\pipename，其中pipename 可以是除了\ 
	// 之外的任意字符，那么这里用一个GUID 串来命名，可以保证不会重复
	//RpcServerUseProtseqEp((unsigned char *)"ncacn_np", 20, (unsigned char *)"\\pipe\\{8dd50205-3108-498f-96e8-dbc4ec074cf9}", NULL);    
	RpcServerUseProtseqEp((unsigned char *)"ncalrpc", RPC_C_PROTSEQ_MAX_REQS_DEFAULT, (unsigned char *)"MyTest", NULL);

	// 注册接口，HelloWorld_v1_0_s_ifspec 是在MIDL 生成的Hello.h 中定义的
	RpcServerRegisterIf(HelloWorld_v1_0_s_ifspec, NULL, NULL);

	// 开始监听，本函数将一直阻塞
	RpcServerListen(1,20,FALSE);

	return 0;
}

// 下面的函数是为了满足链接需要而写的，没有的话会出现链接错误
void __RPC_FAR* __RPC_USER midl_user_allocate(size_t len)
{
	return(malloc(len));
}

void __RPC_USER midl_user_free(void __RPC_FAR *ptr)
{
	free(ptr);
}
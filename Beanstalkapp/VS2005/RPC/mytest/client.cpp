#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Test.h"     // 引用MIDL 生成的头文件

int main(int argc, char * argv[])
{
     unsigned char * pszStringBinding = NULL;
     if ( argc != 2 )
     {
         printf("Usage:%s <Hello Text>\n", argv[0]);
         return 1;
     }    

     // 用Named Pipe 作为RPC 的通道。参见server.c 中的RpcServerUseProtseqEp() 部分
     // 第3 个参数NetworkAddr 如果取NULL，那么就是连接本机服务
     // 否则要取\\\\servername 这样的格式，例如你的计算机名为jack，那么就是\\jack
     //RpcStringBindingCompose( NULL, (unsigned char*)"ncacn_np", /*(unsigned char*)"\\\\servername"*/ NULL, (unsigned char*)"\\pipe\\{8dd50205-3108-498f-96e8-dbc4ec074cf9}", NULL, &pszStringBinding );
	 RpcStringBindingCompose( 
		 NULL, 
		 (unsigned char*)"ncalrpc", 
		 NULL, (unsigned char*)"MyTest", 
		 NULL, 
		 &pszStringBinding );

     // 绑定接口，这里要和 Hello.acf 的配置一致，那么就是HelloWorld_Binding
     RpcBindingFromStringBinding(pszStringBinding, & HelloWorld_Binding );    

     // 下面是调用服务端的函数了
     RpcTryExcept
     {
         if ( _stricmp(argv[1], "SHUTDOWN") == 0 )
         {
              Shutdown();
         }
         else
         {
              Hello((unsigned char*)argv[1]);
         }
     }
     RpcExcept(1)
     {
         printf( "RPC Exception %d\n", RpcExceptionCode() );
     }
     RpcEndExcept

 
     // 释放资源
     RpcStringFree(&pszStringBinding);
     RpcBindingFree(&HelloWorld_Binding);
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
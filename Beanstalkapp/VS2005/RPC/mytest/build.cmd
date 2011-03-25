midl test.idl
cl /D_WIN32_WINNT=0x500 server.cpp Test_s.c rpcrt4.lib
cl /D_WIN32_WINNT=0x500 client.cpp test_c.c rpcrt4.lib
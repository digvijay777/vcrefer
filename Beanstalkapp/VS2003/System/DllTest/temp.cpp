//HookDll.h
#pragma once
#include <windows.h>
//硬编码结构体

#pragma pack (push ,1) // 保证下面的结构体采用BYTE对齐（必须）

typedef struct _INJECT_LOADLIBRARY_CODE

{

	BYTE int_PUSHAD; // pushad 0x60

	BYTE int_PUSH; // push &szDLL 0x68

	DWORD push_Value; // &szDLL = "HookDll.dll"的path

	BYTE int_MOVEAX; // move eax &LoadLibrary 0xB8

	DWORD eax_Value; // &LoadLibrary

	WORD call_eax; // call eax 0xD0FF(FF D0) (LoadLibrary("ApiSpy.dll");

	BYTE jmp_MOVEAX; // move eax &ReplaceOldCode 0xB8

	DWORD jmp_Value; // JMP的参数

	WORD jmp_eax; // jmp eax 0xE0FF(FF E0) jmp ReplaceOldCode;

	char szDLL[MAX_PATH]; // "ApiSpy.dll"的FullPath

}INJECT_LOADLIBRARY_CODE, *LPINJECT_CODE;


#pragma pack (pop , 1)


typedef struct _JMP_CODE
{
	DWORD lpEntryPoint;
	DWORD OldAddr;
	DWORD OldCode[4];
}JMP_CODE,* LPJMP_CODE;

// FileMaping的结构体

typedef struct _SPY_MEM_SHARE

{

	LPBYTE lpEntryPoint; // 目标进程的入口地址

	BYTE oldcode[sizeof(INJECT_LOADLIBRARY_CODE)]; // 目标进程的代码保存

}SPY_MEM_SHARE, * LPSPY_MEM_SHARE;

typedef  void   (__stdcall *FP)(unsigned char *,unsigned int);
BOOL WINAPI InitMyDll();

void __stdcall InitDecrypt();
void __stdcall HookDecrypt();
void __stdcall UnHookDecrypt();

void __stdcall InitEncrypt();
void __stdcall HookEncrypt();
void __stdcall UnHookEncrypt();
DWORD RecvThread(void* param) ;

// HookDll.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include <stddef.h>
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "HookDll.h"

#pragma comment(lib,"Ws2_32.lib")

static JMP_CODE _lpCode;

// 转入处理程序



BYTE btOldDecryptBytes[9];
BYTE btOldEncryptBytes[9];

DWORD old_adress_decrypt = 0x0078DFB0;
DWORD old_adress_encrypt = 0x0078DF30;

DWORD ret_address_encrypt = 0;
DWORD inbuf_encrypt = 0;
DWORD longth_encrypt  = 0;

DWORD ret_address_decrypt = 0;
DWORD inbuf_decrypt = 0;
DWORD longth_decrypt  = 0;

#define DEFAULT_PORT "7110"
SOCKET ListenSocket = INVALID_SOCKET,ClientSocket = INVALID_SOCKET;

DWORD RecvThread(void* param)
{
	WSADATA wsaData;

	struct addrinfo *result = NULL,hints;
	unsigned char recvbuf[1024];
	int iResult;
	int recvbuflen = 1024;



	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if ( iResult != 0 )
	{
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET)
	{
		printf("socket failed: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// Setup the TCP listening socket
	iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		printf("bind failed: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR)
	{
		printf("listen failed: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	// Accept a client socket
	while(1)
	{

		ClientSocket = accept(ListenSocket, NULL, NULL);
		if (ClientSocket == INVALID_SOCKET)
		{
			printf("accept failed: %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			ClientSocket = INVALID_SOCKET;
			continue;
		}

		memset(recvbuf,0,sizeof(recvbuf));

		iResult = recv(ClientSocket, (char *)recvbuf, recvbuflen, 0);

		if(iResult != recvbuflen)
		{
			printf("recv failed: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			ClientSocket = INVALID_SOCKET;
			continue;
		}



		//这里可以调用exe函数

		//结果保存在recvbuf里，返回去


		send(ClientSocket,(char *)&recvbuf,recvbuflen,0);

		iResult = shutdown(ClientSocket, SD_SEND);
		if (iResult == SOCKET_ERROR)
		{
			printf("shutdown failed: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			ClientSocket = INVALID_SOCKET;
			continue;
		}
		closesocket(ClientSocket);

	}
	WSACleanup();
	return 0;
}

char* __stdcall PrintBin(const unsigned char* buf, int buf_size)
{
	static char tmp_buf[1024 * 4] = { 0 } ;
	memset(tmp_buf, 0, sizeof(tmp_buf)) ;
	char tmp_str[5] = { 0 } ;
	int i = 0 ;
	int j = 0 ;

	if (buf_size > 1024)
	{
		return tmp_buf ;
	}

	for (; i < buf_size; i += 16)
	{
		for (j = 0; j < 16; j++)
		{
			if (i + j >= buf_size)
			{
				break ;
			}
			wsprintf(tmp_str, "%02x ", buf[i + j]) ;
			strcat(tmp_buf, tmp_str) ;
		}
		strcat(tmp_buf, "\r\n");
	}

	strcat(tmp_buf,"\r\n");
	const char *filename = "f:\\01.txt";
	const char *recvline = "---------------------recv---------------------\r\n\r\n";
	HANDLE hFile = CreateFile(filename, GENERIC_WRITE, FILE_SHARE_WRITE, NULL,OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if ( hFile != INVALID_HANDLE_VALUE)
	{
		DWORD num;
		LONG lDistance = 0x00;
		SetFilePointer (hFile, lDistance, NULL, FILE_END) ;
		WriteFile(hFile,recvline,strlen(recvline),&num,NULL);
		SetFilePointer (hFile, lDistance, NULL, FILE_END) ;
		WriteFile(hFile,tmp_buf,strlen(tmp_buf),&num,NULL);
		CloseHandle(hFile);
	}
	return tmp_buf;
}

char* __stdcall PrintBin01(const unsigned char* buf, int buf_size)
{
	static char tmp_buf[1024 * 4] = { 0 } ;
	memset(tmp_buf, 0, sizeof(tmp_buf)) ;
	char tmp_str[5] = { 0 } ;
	int i = 0 ;
	int j = 0 ;

	if (buf_size > 1024)
	{
		return tmp_buf ;
	}

	for (; i < buf_size; i += 16)
	{
		for (j = 0; j < 16; j++)
		{
			if (i + j >= buf_size)
			{
				break ;
			}
			wsprintf(tmp_str, "%02x ", buf[i + j]) ;
			strcat(tmp_buf, tmp_str) ;
		}
		strcat(tmp_buf, "\r\n");
	}

	strcat(tmp_buf,"\r\n");
	const char *filename = "f:\\01.txt";
	const char *recvline = "---------------------send---------------------\r\n\r\n";
	HANDLE hFile = CreateFile(filename, GENERIC_WRITE, FILE_SHARE_WRITE, NULL,OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if ( hFile != INVALID_HANDLE_VALUE)
	{
		DWORD num;
		LONG lDistance = 0x00;
		SetFilePointer (hFile, lDistance, NULL, FILE_END) ;
		WriteFile(hFile,recvline,strlen(recvline),&num,NULL);
		SetFilePointer (hFile, lDistance, NULL, FILE_END) ;
		WriteFile(hFile,tmp_buf,strlen(tmp_buf),&num,NULL);
		CloseHandle(hFile);
	}
	return tmp_buf;
}


//这里注意堆栈平衡，与现场保护
void  __declspec(naked) DeCrypt()
{
	//因为前边
	//push eax //保护现场把eax压入
	//mov eax 0xxxxxxxxx
	//jmp eax
	//这里恢复eax

	_asm pop eax
		//又需要用到eax进行保存参数
		_asm push eax
		_asm mov eax,dword ptr [esp+0x4]
		_asm mov ret_address_decrypt, eax
			_asm mov eax,dword ptr[esp+0x8]
			_asm mov inbuf_decrypt,eax
				_asm mov eax,dword ptr[esp+0x0c]
				_asm mov longth_decrypt,eax
					_asm pop eax
					//保存现场
					_asm pushad
					//剪除挂钩
					_asm call UnHookDecrypt
					//恢复挂钩之前的现场
					_asm popad
					//把返回地址弹出去
					_asm add esp,4
					//调用原始函数
					_asm call old_adress_decrypt
					//恢复挂钩
					_asm call HookDecrypt
					//dump出参数
					_asm push longth_decrypt
					_asm push inbuf_decrypt
					_asm call PrintBin
					//跳回挂钩函数的下条指令
					_asm jmp ret_address_decrypt

}



void  __declspec(naked) EnCrypt()
{
	_asm pop eax
		_asm push eax
		_asm mov eax,dword ptr [esp+0x4]
		_asm mov ret_address_encrypt, eax
			_asm mov eax,dword ptr[esp+0x8]
			_asm mov inbuf_encrypt,eax
				_asm mov eax,dword ptr[esp+0x0c]
				_asm mov longth_encrypt,eax
					_asm pop eax
					_asm pushad
					_asm push longth_encrypt
					_asm push inbuf_encrypt
					_asm call PrintBin01
					_asm call UnHookEncrypt
					_asm popad
					_asm add esp,4
					_asm call old_adress_encrypt
					_asm call HookEncrypt
					_asm jmp ret_address_encrypt

}

void __stdcall InitDecrypt()
{
	HANDLE hGame = GetCurrentProcess();


	DWORD dwCodeSize = 0;

	ReadProcessMemory(hGame, (void *)old_adress_decrypt, (void *)btOldDecryptBytes, sizeof(btOldDecryptBytes), &dwCodeSize);



	//push eax 
	//mov eax 0xxxxxxxxx
	//jmp eax
	BYTE btNewBytes[9] = {0x50,0xB8,0x00,0x00,0x40,0x00,0x0FF,0x0E0,0x00 }; 

	*(DWORD *)( btNewBytes + 2) = (DWORD)DeCrypt;

	dwCodeSize = 0;

	DWORD fNew, fOld;

	fNew = PAGE_READWRITE;

	VirtualProtect((void*)old_adress_decrypt, sizeof(btOldDecryptBytes), fNew, &fOld);

	WriteProcessMemory(hGame, (void *)old_adress_decrypt, (void *)btNewBytes, sizeof(btOldDecryptBytes), &dwCodeSize);

	VirtualProtect((void*)old_adress_decrypt, sizeof(btOldDecryptBytes), fOld, &fNew);

}
void __stdcall InitEncrypt()
{
	HANDLE hGame = GetCurrentProcess();


	DWORD dwCodeSize = 0;

	ReadProcessMemory(hGame, (void *)old_adress_encrypt, (void *)btOldEncryptBytes, sizeof(btOldEncryptBytes), &dwCodeSize);





	BYTE btNewBytes[9] = {0x50,0xB8,0x00,0x00,0x40,0x00,0x0FF,0x0E0,0x00 }; 

	*(DWORD *)( btNewBytes + 2) = (DWORD)EnCrypt;

	dwCodeSize = 0;

	DWORD fNew, fOld;

	fNew = PAGE_READWRITE;

	VirtualProtect((void*)old_adress_encrypt, sizeof(btOldEncryptBytes), fNew, &fOld);

	WriteProcessMemory(hGame, (void *)old_adress_encrypt, (void *)btNewBytes, sizeof(btOldEncryptBytes), &dwCodeSize);

	VirtualProtect((void*)old_adress_encrypt, sizeof(btOldEncryptBytes), fOld, &fNew);

}


void __stdcall HookDecrypt()
{



	DWORD dwCodeSize = 0;

	BYTE btNewBytes[9] = {0x50,0xB8,0x00,0x00,0x40,0x00,0x0FF,0x0E0,0x00 }; 

	*(DWORD *)( btNewBytes + 2) = (DWORD)DeCrypt;
	DWORD fNew, fOld;

	fNew = PAGE_READWRITE;

	HANDLE hGame = GetCurrentProcess();



	VirtualProtect((void*)old_adress_decrypt, sizeof(btOldDecryptBytes), fNew, &fOld);

	WriteProcessMemory(hGame, (void *)old_adress_decrypt, (void *)btNewBytes, sizeof(btOldDecryptBytes), &dwCodeSize);

	VirtualProtect((void*)old_adress_decrypt, sizeof(btOldDecryptBytes), fOld, &fNew);

}
void __stdcall HookEncrypt()
{



	DWORD dwCodeSize = 0;

	BYTE btNewBytes[9] = {0x50,0xB8,0x00,0x00,0x40,0x00,0x0FF,0x0E0,0x00 }; 

	*(DWORD *)( btNewBytes + 2) = (DWORD)EnCrypt;
	DWORD fNew, fOld;

	fNew = PAGE_READWRITE;

	HANDLE hGame = GetCurrentProcess();



	VirtualProtect((void*)old_adress_encrypt, sizeof(btOldEncryptBytes), fNew, &fOld);

	WriteProcessMemory(hGame, (void *)old_adress_encrypt, (void *)btNewBytes, sizeof(btOldEncryptBytes), &dwCodeSize);

	VirtualProtect((void*)old_adress_encrypt, sizeof(btOldEncryptBytes), fOld, &fNew);

}
void _stdcall UnHookDecrypt()
{


	DWORD dwCodeSize = 0;

	DWORD fNew, fOld;

	fNew = PAGE_READWRITE;

	HANDLE hGame = GetCurrentProcess();

	VirtualProtect((void*)old_adress_decrypt, sizeof(btOldDecryptBytes), fNew, &fOld);

	WriteProcessMemory(hGame, (void *)old_adress_decrypt, (void *)btOldDecryptBytes, sizeof(btOldDecryptBytes), &dwCodeSize);

	VirtualProtect((void*)old_adress_decrypt, sizeof(btOldDecryptBytes), fOld, &fNew);

}
void _stdcall UnHookEncrypt()
{


	DWORD dwCodeSize = 0;

	DWORD fNew, fOld;

	fNew = PAGE_READWRITE;

	HANDLE hGame = GetCurrentProcess();

	VirtualProtect((void*)old_adress_encrypt, sizeof(btOldEncryptBytes), fNew, &fOld);

	WriteProcessMemory(hGame, (void *)old_adress_encrypt, (void *)btOldEncryptBytes, sizeof(btOldEncryptBytes), &dwCodeSize);

	VirtualProtect((void*)old_adress_encrypt, sizeof(btOldEncryptBytes), fOld, &fNew);

}

DWORD* _glpMovEax;

DWORD _gfNew, _gfOld;

void __declspec(naked) DoJmpEntryPoint ()

{

	// 恢复call LoadLibrary后面的代码

	_gfNew = PAGE_READWRITE;

	_glpMovEax = (DWORD*)_lpCode.OldAddr;

	VirtualProtect(_glpMovEax, 2*sizeof(DWORD), _gfNew, &_gfOld);

	*_glpMovEax = _lpCode.OldCode[0];

	*(_glpMovEax + 1) = _lpCode.OldCode[1];



	VirtualProtect(_glpMovEax, 2*sizeof(DWORD), _gfOld, &_gfNew);

	//第八步：目标进程Jmp至原来的入口，继续运行程序。

	// 跳至目标代码的入口

	_asm popad

		_asm jmp _lpCode.lpEntryPoint

}
BOOL WINAPI InitMyDll()
{

	HANDLE hMap;

	LPSPY_MEM_SHARE lpMem;

	DWORD dwSize;

	BOOL rc;

	BYTE* _lpBytes;

	// 取得FileMapping的句柄


	hMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, 0, "MyDllMapView");

	if(hMap)

	{


		lpMem = (LPSPY_MEM_SHARE)MapViewOfFile(hMap,FILE_MAP_ALL_ACCESS,0, 0, 0);

		if(lpMem)

		{

			//第七步：目标进程运行完LoadLibrary(MyDll)后，将原来的代码写回目标进程的入口。
			// 恢复目标进程的入口代码
			// 得到INJECT_LOADLIBRARY_CODE结构成员BYTE jmp_MOVEAX地址;

			_lpCode.OldAddr = (DWORD)((BYTE*)lpMem->lpEntryPoint + offsetof(INJECT_LOADLIBRARY_CODE, jmp_MOVEAX));

			_lpCode.lpEntryPoint = (DWORD)lpMem->lpEntryPoint;

			// 保存INJECT_LOADLIBRARY_CODE结构成员BYTE jmp_MOVEAX地址后四个字节，注意这里是原始子进程未修改前的数据

			memcpy(&_lpCode.OldCode, (BYTE*)lpMem->oldcode + offsetof(INJECT_LOADLIBRARY_CODE, jmp_MOVEAX), 2*sizeof(DWORD));

			// 恢复目标进程的入口代码
			DWORD fNew, fOld;



			fNew = PAGE_READWRITE;

			VirtualProtect(lpMem->lpEntryPoint, sizeof(INJECT_LOADLIBRARY_CODE), fNew, &fOld);

			rc = WriteProcessMemory(GetCurrentProcess(), lpMem->lpEntryPoint, lpMem->oldcode, sizeof(INJECT_LOADLIBRARY_CODE), &dwSize);

			VirtualProtect(lpMem->lpEntryPoint, sizeof(INJECT_LOADLIBRARY_CODE), fOld, &fNew);

			_lpBytes = (BYTE*)lpMem->lpEntryPoint + offsetof(INJECT_LOADLIBRARY_CODE, jmp_MOVEAX);

			UnmapViewOfFile(lpMem);

		}

		CloseHandle(hMap);

	}
	//
	//这里可以干一些好事或者坏事

	//...........................

	InitDecrypt();
	InitEncrypt();

	//创建远线程
	//DWORD dwThreadId;
	//hRecvThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RecvThread,
	//                               (void*)RecvThread, 0, &dwThreadId);


	//后边恢复原始的代码
	// 将LoadLibrary后面的代码写为转入处理程序中

	// 指令为：mov eax, objAddress

	// jmp eax

	BYTE* lpMovEax;

	DWORD* lpMovEaxValue;

	WORD* lpJmp;

	DWORD fNew, fOld;



	fNew = PAGE_READWRITE;

	lpMovEax = _lpBytes;

	VirtualProtect(lpMovEax, 2*sizeof(DWORD), fNew, &fOld);

	//move eax &DoJmpEntryPoint

	*lpMovEax = 0xB8; 

	lpMovEaxValue = (DWORD*)(lpMovEax + 1);

	*lpMovEaxValue = (DWORD)&DoJmpEntryPoint; 


	//jmp eax
	lpJmp = (WORD*)(lpMovEax + 5);

	*lpJmp = 0xE0FF; // (FF E0)

	VirtualProtect(lpMovEax, 2*sizeof(DWORD), fOld, &fNew);

	return TRUE;

}


BOOL WINAPI DllMain( HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved )
{
	switch( ul_reason_for_call )
	{
	case DLL_PROCESS_ATTACH:
		{

			return InitMyDll();
		}

		break;

	case DLL_THREAD_ATTACH:
		break;

	case DLL_THREAD_DETACH:
		break;

	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

// myhook.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <imagehlp.h>



#pragma comment(lib,"Imagehlp.lib")
//硬编码结构体

#pragma pack (push ,1) // 保证下面的结构体采用BYTE对齐（必须）

typedef struct _INJECT_LOADLIBRARY_CODE
{

	BYTE int_PUSHAD; // pushad 0x60

	BYTE int_PUSH; // push &szDLL 0x68

	DWORD push_Value; // &szDLL = "HookDll.dll"的path

	BYTE int_MOVEAX; // move eax &LoadLibrary 0xB8

	DWORD eax_Value; // &LoadLibrary

	WORD call_eax; // call eax 0xD0FF(FF D0) (LoadLibrary("HookDll.dll");

	BYTE jmp_MOVEAX; // move eax &ReplaceOldCode 0xB8

	DWORD jmp_Value; // JMP的参数

	WORD jmp_eax; // jmp eax 0xE0FF(FF E0) jmp ReplaceOldCode;

	char szDLL[MAX_PATH]; // "ApiSpy.dll"的FullPath

}INJECT_LOADLIBRARY_CODE, *LPINJECT_CODE;



#pragma pack (pop , 1)

// FileMaping的结构体

typedef struct 
{

	LPBYTE  lpEntryPoint;   // 目标进程的入口地址

	BYTE    oldcode[sizeof(INJECT_LOADLIBRARY_CODE)];        // 目标进程的代码保存

}SPY_MEM_SHARE, * LPSPY_MEM_SHARE;

LPBYTE  GetExeEntryPoint(char *filename)

{

	PIMAGE_NT_HEADERS      pNTHeader;

	DWORD pEntryPoint;

	PLOADED_IMAGE       pImage;



	pImage = ImageLoad(filename, NULL);



	if(pImage == NULL)

		return NULL;

	pNTHeader = pImage->FileHeader;

	pEntryPoint = pNTHeader->OptionalHeader.AddressOfEntryPoint + pNTHeader->OptionalHeader.ImageBase;

	ImageUnload(pImage);

	return (LPBYTE)pEntryPoint;

}

int _tmain(int argc, _TCHAR* argv[])
{

	////////////Create Process//////////////
	char LoadExe[MAX_PATH];
	ZeroMemory(LoadExe,MAX_PATH);

	char lpCommandLine[MAX_PATH];
	ZeroMemory(lpCommandLine,MAX_PATH);
	//注入dll的exe文件名
	strcat(LoadExe,"test.exe");
	//strcpy(lpCommandLine,LoadExe);

	//exe传入的命令行参数
	strcat(lpCommandLine," xxxxx");



	printf("%s\n",LoadExe);

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );

	// Start the child process. 
	if( !CreateProcess( LoadExe, // No module name (use command line). 
		lpCommandLine, // Command line. 
		NULL,             // Process handle not inheritable. 
		NULL,             // Thread handle not inheritable. 
		FALSE,            // Set handle inheritance to FALSE. 
		CREATE_SUSPENDED,                // No creation flags. 
		NULL,             // Use parent's environment block. 
		NULL,             // Use parent's starting directory. 
		&si,              // Pointer to STARTUPINFO structure.
		&pi )             // Pointer to PROCESS_INFORMATION structure.
		) 
	{
		printf( "CreateProcess failed.\n" );
		DWORD dwError = GetLastError();
		printf("Error %d \n",dwError);
		getchar();
		return 0;
	}

	PBYTE pEntryPoint = GetExeEntryPoint(LoadExe);

	//创建FileMapping

	HANDLE hMap = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL,PAGE_READWRITE, 0, sizeof(SPY_MEM_SHARE), "MyDllMapView");

	LPSPY_MEM_SHARE  lpMap = (LPSPY_MEM_SHARE)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);

	DWORD cBytesMoved;

	ReadProcessMemory(pi.hProcess, pEntryPoint,

		&lpMap->oldcode, sizeof(INJECT_LOADLIBRARY_CODE),

		&cBytesMoved);

	lpMap->lpEntryPoint = pEntryPoint;




	// 第四步：在目标进程的入口写入LoadLibrary（MyDll）实现Dll的注入。


	// 准备注入DLL的代码

	INJECT_LOADLIBRARY_CODE     newCode;

	ZeroMemory(&newCode,sizeof(INJECT_LOADLIBRARY_CODE));

	// 写入MyDll―――用全路径

	char szMyDll[MAX_PATH];
	memset(szMyDll,0,sizeof(szMyDll));
	strcat(szMyDll,"HookDll.dll");


	lstrcpy(newCode.szDLL, szMyDll);




	// 准备硬代码（汇编代码）

	newCode.int_PUSHAD = 0x60;   // pushad 0x60 

	newCode.int_PUSH = 0x68;     //push &szDLL 0x68

	newCode.int_MOVEAX = 0xB8;   //move eax &LoadLibrary 0xB8

	newCode.call_eax = 0xD0FF;   // call eax 0xD0FF(FF D0) (LoadLibrary("ApiSpy.dll");

	newCode.jmp_MOVEAX = 0xB8;   // move eax &ReplaceOldCode 0xB8

	newCode.jmp_eax = 0xE0FF;    // jmp eax 0xE0FF(FF E0) jmp ReplaceOldCode;

	newCode.eax_Value = (DWORD)&LoadLibrary; // &LoadLibrary

	newCode.push_Value= (DWORD)(pEntryPoint + offsetof(INJECT_LOADLIBRARY_CODE,szDLL)); //&szDLL = "HookDll.dll"的path

	/*
	上面结构体的代码为汇编代码，对应的汇编为：

	pushad

	push szDll

	mov eax, &LoadLibraryA

	call eax  // 实现调用LoadLibrary(szDll)的代码

	mov eax, oldentry

	jmp eax // 实现在LoadLibrary运行完后, 跳至目标进程的入口继续运行
	*/



	// 将硬代码写入目标进程的入口

	// 修改内存属性

	DWORD dwNew***, dwOld***;



	dwNew*** = PAGE_READWRITE;

	VirtualProtectEx(pi.hProcess, (LPVOID)pEntryPoint, sizeof(DWORD), dwNew***, &dwOld***);



	DWORD dwWrited;
	WriteProcessMemory(pi.hProcess, pEntryPoint,&newCode, sizeof(newCode), &dwWrited);
	VirtualProtectEx(pi.hProcess, (LPVOID)pEntryPoint, sizeof(DWORD), dwOld***, &dwNew***);

	// 释放FileMaping  注意，不是Closehandle(hMap)

	UnmapViewOfFile(lpMap);

	// 继续目标进程的运行

	//第五步：用ResumeThread运行目标进程。

	ResumeThread(pi.hThread);

	//一定要加个sleep，不加的话CreateFileMapping文件就会delete掉，dll就不能找到这个映射文件了。



	Sleep(20000);


	return 0;
} 
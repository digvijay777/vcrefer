//HookDll.h
#pragma once
#include <windows.h>
//Ӳ����ṹ��

#pragma pack (push ,1) // ��֤����Ľṹ�����BYTE���루���룩

typedef struct _INJECT_LOADLIBRARY_CODE

{

	BYTE int_PUSHAD; // pushad 0x60

	BYTE int_PUSH; // push &szDLL 0x68

	DWORD push_Value; // &szDLL = "HookDll.dll"��path

	BYTE int_MOVEAX; // move eax &LoadLibrary 0xB8

	DWORD eax_Value; // &LoadLibrary

	WORD call_eax; // call eax 0xD0FF(FF D0) (LoadLibrary("ApiSpy.dll");

	BYTE jmp_MOVEAX; // move eax &ReplaceOldCode 0xB8

	DWORD jmp_Value; // JMP�Ĳ���

	WORD jmp_eax; // jmp eax 0xE0FF(FF E0) jmp ReplaceOldCode;

	char szDLL[MAX_PATH]; // "ApiSpy.dll"��FullPath

}INJECT_LOADLIBRARY_CODE, *LPINJECT_CODE;


#pragma pack (pop , 1)


typedef struct _JMP_CODE
{
	DWORD lpEntryPoint;
	DWORD OldAddr;
	DWORD OldCode[4];
}JMP_CODE,* LPJMP_CODE;

// FileMaping�Ľṹ��

typedef struct _SPY_MEM_SHARE

{

	LPBYTE lpEntryPoint; // Ŀ����̵���ڵ�ַ

	BYTE oldcode[sizeof(INJECT_LOADLIBRARY_CODE)]; // Ŀ����̵Ĵ��뱣��

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

// HookDll.cpp : ���� DLL Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <stddef.h>
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "HookDll.h"

#pragma comment(lib,"Ws2_32.lib")

static JMP_CODE _lpCode;

// ת�봦�����



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



		//������Ե���exe����

		//���������recvbuf�����ȥ


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


//����ע���ջƽ�⣬���ֳ�����
void  __declspec(naked) DeCrypt()
{
	//��Ϊǰ��
	//push eax //�����ֳ���eaxѹ��
	//mov eax 0xxxxxxxxx
	//jmp eax
	//����ָ�eax

	_asm pop eax
		//����Ҫ�õ�eax���б������
		_asm push eax
		_asm mov eax,dword ptr [esp+0x4]
		_asm mov ret_address_decrypt, eax
			_asm mov eax,dword ptr[esp+0x8]
			_asm mov inbuf_decrypt,eax
				_asm mov eax,dword ptr[esp+0x0c]
				_asm mov longth_decrypt,eax
					_asm pop eax
					//�����ֳ�
					_asm pushad
					//�����ҹ�
					_asm call UnHookDecrypt
					//�ָ��ҹ�֮ǰ���ֳ�
					_asm popad
					//�ѷ��ص�ַ����ȥ
					_asm add esp,4
					//����ԭʼ����
					_asm call old_adress_decrypt
					//�ָ��ҹ�
					_asm call HookDecrypt
					//dump������
					_asm push longth_decrypt
					_asm push inbuf_decrypt
					_asm call PrintBin
					//���عҹ�����������ָ��
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

	// �ָ�call LoadLibrary����Ĵ���

	_gfNew = PAGE_READWRITE;

	_glpMovEax = (DWORD*)_lpCode.OldAddr;

	VirtualProtect(_glpMovEax, 2*sizeof(DWORD), _gfNew, &_gfOld);

	*_glpMovEax = _lpCode.OldCode[0];

	*(_glpMovEax + 1) = _lpCode.OldCode[1];



	VirtualProtect(_glpMovEax, 2*sizeof(DWORD), _gfOld, &_gfNew);

	//�ڰ˲���Ŀ�����Jmp��ԭ������ڣ��������г���

	// ����Ŀ���������

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

	// ȡ��FileMapping�ľ��


	hMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, 0, "MyDllMapView");

	if(hMap)

	{


		lpMem = (LPSPY_MEM_SHARE)MapViewOfFile(hMap,FILE_MAP_ALL_ACCESS,0, 0, 0);

		if(lpMem)

		{

			//���߲���Ŀ�����������LoadLibrary(MyDll)�󣬽�ԭ���Ĵ���д��Ŀ����̵���ڡ�
			// �ָ�Ŀ����̵���ڴ���
			// �õ�INJECT_LOADLIBRARY_CODE�ṹ��ԱBYTE jmp_MOVEAX��ַ;

			_lpCode.OldAddr = (DWORD)((BYTE*)lpMem->lpEntryPoint + offsetof(INJECT_LOADLIBRARY_CODE, jmp_MOVEAX));

			_lpCode.lpEntryPoint = (DWORD)lpMem->lpEntryPoint;

			// ����INJECT_LOADLIBRARY_CODE�ṹ��ԱBYTE jmp_MOVEAX��ַ���ĸ��ֽڣ�ע��������ԭʼ�ӽ���δ�޸�ǰ������

			memcpy(&_lpCode.OldCode, (BYTE*)lpMem->oldcode + offsetof(INJECT_LOADLIBRARY_CODE, jmp_MOVEAX), 2*sizeof(DWORD));

			// �ָ�Ŀ����̵���ڴ���
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
	//������Ը�һЩ���»��߻���

	//...........................

	InitDecrypt();
	InitEncrypt();

	//����Զ�߳�
	//DWORD dwThreadId;
	//hRecvThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RecvThread,
	//                               (void*)RecvThread, 0, &dwThreadId);


	//��߻ָ�ԭʼ�Ĵ���
	// ��LoadLibrary����Ĵ���дΪת�봦�������

	// ָ��Ϊ��mov eax, objAddress

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

// myhook.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <windows.h>
#include <imagehlp.h>



#pragma comment(lib,"Imagehlp.lib")
//Ӳ����ṹ��

#pragma pack (push ,1) // ��֤����Ľṹ�����BYTE���루���룩

typedef struct _INJECT_LOADLIBRARY_CODE
{

	BYTE int_PUSHAD; // pushad 0x60

	BYTE int_PUSH; // push &szDLL 0x68

	DWORD push_Value; // &szDLL = "HookDll.dll"��path

	BYTE int_MOVEAX; // move eax &LoadLibrary 0xB8

	DWORD eax_Value; // &LoadLibrary

	WORD call_eax; // call eax 0xD0FF(FF D0) (LoadLibrary("HookDll.dll");

	BYTE jmp_MOVEAX; // move eax &ReplaceOldCode 0xB8

	DWORD jmp_Value; // JMP�Ĳ���

	WORD jmp_eax; // jmp eax 0xE0FF(FF E0) jmp ReplaceOldCode;

	char szDLL[MAX_PATH]; // "ApiSpy.dll"��FullPath

}INJECT_LOADLIBRARY_CODE, *LPINJECT_CODE;



#pragma pack (pop , 1)

// FileMaping�Ľṹ��

typedef struct 
{

	LPBYTE  lpEntryPoint;   // Ŀ����̵���ڵ�ַ

	BYTE    oldcode[sizeof(INJECT_LOADLIBRARY_CODE)];        // Ŀ����̵Ĵ��뱣��

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
	//ע��dll��exe�ļ���
	strcat(LoadExe,"test.exe");
	//strcpy(lpCommandLine,LoadExe);

	//exe����������в���
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

	//����FileMapping

	HANDLE hMap = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL,PAGE_READWRITE, 0, sizeof(SPY_MEM_SHARE), "MyDllMapView");

	LPSPY_MEM_SHARE  lpMap = (LPSPY_MEM_SHARE)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);

	DWORD cBytesMoved;

	ReadProcessMemory(pi.hProcess, pEntryPoint,

		&lpMap->oldcode, sizeof(INJECT_LOADLIBRARY_CODE),

		&cBytesMoved);

	lpMap->lpEntryPoint = pEntryPoint;




	// ���Ĳ�����Ŀ����̵����д��LoadLibrary��MyDll��ʵ��Dll��ע�롣


	// ׼��ע��DLL�Ĵ���

	INJECT_LOADLIBRARY_CODE     newCode;

	ZeroMemory(&newCode,sizeof(INJECT_LOADLIBRARY_CODE));

	// д��MyDll�D�D�D��ȫ·��

	char szMyDll[MAX_PATH];
	memset(szMyDll,0,sizeof(szMyDll));
	strcat(szMyDll,"HookDll.dll");


	lstrcpy(newCode.szDLL, szMyDll);




	// ׼��Ӳ���루�����룩

	newCode.int_PUSHAD = 0x60;   // pushad 0x60 

	newCode.int_PUSH = 0x68;     //push &szDLL 0x68

	newCode.int_MOVEAX = 0xB8;   //move eax &LoadLibrary 0xB8

	newCode.call_eax = 0xD0FF;   // call eax 0xD0FF(FF D0) (LoadLibrary("ApiSpy.dll");

	newCode.jmp_MOVEAX = 0xB8;   // move eax &ReplaceOldCode 0xB8

	newCode.jmp_eax = 0xE0FF;    // jmp eax 0xE0FF(FF E0) jmp ReplaceOldCode;

	newCode.eax_Value = (DWORD)&LoadLibrary; // &LoadLibrary

	newCode.push_Value= (DWORD)(pEntryPoint + offsetof(INJECT_LOADLIBRARY_CODE,szDLL)); //&szDLL = "HookDll.dll"��path

	/*
	����ṹ��Ĵ���Ϊ�����룬��Ӧ�Ļ��Ϊ��

	pushad

	push szDll

	mov eax, &LoadLibraryA

	call eax  // ʵ�ֵ���LoadLibrary(szDll)�Ĵ���

	mov eax, oldentry

	jmp eax // ʵ����LoadLibrary�������, ����Ŀ����̵���ڼ�������
	*/



	// ��Ӳ����д��Ŀ����̵����

	// �޸��ڴ�����

	DWORD dwNew***, dwOld***;



	dwNew*** = PAGE_READWRITE;

	VirtualProtectEx(pi.hProcess, (LPVOID)pEntryPoint, sizeof(DWORD), dwNew***, &dwOld***);



	DWORD dwWrited;
	WriteProcessMemory(pi.hProcess, pEntryPoint,&newCode, sizeof(newCode), &dwWrited);
	VirtualProtectEx(pi.hProcess, (LPVOID)pEntryPoint, sizeof(DWORD), dwOld***, &dwNew***);

	// �ͷ�FileMaping  ע�⣬����Closehandle(hMap)

	UnmapViewOfFile(lpMap);

	// ����Ŀ����̵�����

	//���岽����ResumeThread����Ŀ����̡�

	ResumeThread(pi.hThread);

	//һ��Ҫ�Ӹ�sleep�����ӵĻ�CreateFileMapping�ļ��ͻ�delete����dll�Ͳ����ҵ����ӳ���ļ��ˡ�



	Sleep(20000);


	return 0;
} 
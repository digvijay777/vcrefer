// asmtest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

void _stdcall	LockedTest(long* pVal);

int _tmain(int argc, _TCHAR* argv[])
{
	long		l	= 0xfffd;
	// �����ڴ�����
	LockedTest(&l);
	return 0;
}

// �ڴ�����
_declspec(naked) void _stdcall LockedTest(long* pVal)
{
	_asm{
		push	eax
		push	edx

		mov		eax, 1
		//lock add [esp], eax
		mov		edx, [esp+4]
		//add		[esp+4], eax
		//mov		dword ptr [edx], eax
		//lock add dword ptr [edx], eax
		//lock or dword ptr [edx], eax
		// ָ��λ��1
		// bts dword ptr [edx], eax
		// ָ��λ��0
		// btr dword ptr[edx], eax 
		// λ���Բ�ȡ��
		btc dword ptr[edx], eax

		pop		edx
		pop		eax
		retn	4
	}	
}


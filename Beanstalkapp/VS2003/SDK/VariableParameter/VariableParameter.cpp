// VariableParameter.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

void asm_WriteLog(char* pFmt, ...);

#define	TRACELOG_MAXPARAMNUM			0xa				// 最大参数个数
#define TRACELOG						asm_WriteLog

int _tmain(int argc, _TCHAR* argv[])
{

	TRACELOG("Test %d %d %s", 1, 2, "hello");
	return 0;
}

int	MyPrintf(char* date, char* time, char* fmt, ...)
{
	char			szBuff[1024];
	va_list			vaList;

	va_start(vaList, fmt);
	vsprintf(szBuff, fmt, vaList);
	va_end(vaList);

	return printf("%s %s: %s", date, time, szBuff);
}

// 汇编版参数
__declspec(naked)  void asm_WriteLog(char* pFmt, ...)
{
	static const char*	pH1		= "HD";
	static const char*	pH2		= "01";

	__asm{
		// 保存现场
		push	eax
		push	ebx
		push	ecx
		push	ebp
		push	esi
		push	edi

		// 操作
		mov		ebx, TRACELOG_MAXPARAMNUM			// 参数个数
		imul	bl, 4h
		mov		ebp, esp
		mov		esi, ebp
		add		esi, 1Ch
		mov		edi, ebp
		sub		edi, ebx
		cld											// DF = 0
		mov		ecx, ebx
label_copy:											// 复制参数
		movsb
		dec		ecx
		jnz		label_copy

		sub		esp, ebx
		push	pH2
		push	pH1
		call	MyPrintf
		add		esp, 8h								// 传递了两个参数
		add		esp, ebx

		// 恢复现场
		pop		edi
		pop		esi
		pop		ebp
		pop		ecx
		pop		ebx
		pop		eax
	}

	__asm ret
}

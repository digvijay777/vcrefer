#pragma once
#include <string>
#include <assert.h>

#if defined(UNICODE) || defined(_UNICODE)
#define String		std::wstring
#else
#define String		std::string
#endif

#if !defined(ASSERT)
#define ASSERT	assert
#endif

template <class T>
class TDLLExports
{
private:
	HINSTANCE		m_hInstance;
public:
	TDLLExports(LPSTR lpDllFile)
	{
		m_hInstance = ::LoadLibraryA(lpDllFile);
		ASSERT(NULL != m_hInstance);
	}
	~TDLLExports()
	{
		if(NULL != m_hInstance)
			::FreeLibrary(m_hInstance);
	}
public:
	void*		CallStdFun(LPSTR lpFunName, int sizeParam, ...)
	{
		if(NULL == m_hInstance)
			return NULL;
		if(0 != (sizeParam % 4))
		{
			ASSERT(0 == (sizeParam % 4));
			return NULL;
		}

		void*		pRet		= NULL;
		void*		pParam		= &sizeParam;
		FARPROC		pFun		= GetProcAddress(m_hInstance, lpFunName);

		if(NULL == pFun)
		{
			ASSERT(NULL != pFun);
			return NULL;
		}
		__asm{
			push	eax
			push	ebx
			push	ecx
step_init:
			mov		ecx, sizeParam
			sar		ecx, 2
			mov		ebx, pParam
			add		ebx, sizeParam
			test	ecx, ecx
step_pushparam:
			jz      step_call
			push	[ebx]
			sub		ebx, 4
			dec		ecx
			jmp		step_pushparam
step_call:
			call	pFun
			mov		pRet, eax

			pop		ecx
			pop		ebx
			pop		eax
		}
		

		return pRet;
	}
	void*		CallCdeclFun(LPSTR lpFunName, int sizeParam, ...)
	{
		if(NULL == m_hInstance)
			return NULL;
		if(0 != (sizeParam % 4))
		{
			ASSERT(0 == (sizeParam % 4));
			return NULL;
		}

		void*		pRet		= NULL;
		void*		pParam		= &sizeParam;
		FARPROC		pFun		= GetProcAddress(m_hInstance, lpFunName);

		if(NULL == pFun)
		{
			ASSERT(NULL != pFun);
			return NULL;
		}
		__asm{
			push	eax
			push	ebx
			push	ecx
step_init:
			mov		ecx, sizeParam
			sar		ecx, 2
			mov		ebx, pParam
			add		ebx, sizeParam
			test	ecx, ecx
step_pushparam:
			jz      step_call
			push	[ebx]
			sub		ebx, 4
			dec		ecx
			jmp		step_pushparam
step_call:
			call	pFun
			mov		pRet, eax
step_resumestake:
			add		esp, sizeParam

			pop		ecx
			pop		ebx
			pop		eax
		}
		return pRet;
	}

};
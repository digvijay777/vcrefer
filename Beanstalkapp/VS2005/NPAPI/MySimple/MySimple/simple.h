#pragma once
#include "../../../../ExtClass/String/MultiString.h"

extern NPNetscapeFuncs*	gpnpf;
//////////////////////////////////////////////////////////////////////////
class CScriptTable : public NPObject
{
public:
	CScriptTable(NPP npp)
	{
		m_instance = npp;
		memset(&m_window, 0, sizeof(m_window));
	}

public:
	NPP			m_instance;
	NPWindow	m_window;
};

BOOL CALLBACK EnumThreadFirstWndProc(HWND hwnd, LPARAM lParam)
{
	DWORD   ProcId;
	DWORD   ThreadId;

	ThreadId = GetWindowThreadProcessId(hwnd, &ProcId);
	if(ProcId == GetCurrentProcessId())
	{
		HWND*   pWin	= (HWND *)lParam;       //进程主窗口

		*pWin = hwnd;
		while(true)
		{
			HWND   hTemp;

			hTemp = GetParent(*pWin);
			if(ThreadId == GetWindowThreadProcessId(hTemp, NULL))
				*pWin = hTemp;
			else
				break;
		}
		return !IsWindowVisible(*pWin);
	}
	
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
// 这个测试

bool hasMethod(NPObject* obj, NPIdentifier methodName) 
{
	return true;
}

bool invokeDefault(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) 
{
	result->type = NPVariantType_Int32;
	result->value.intValue = 42;
	return true;
}

bool invoke(NPObject* obj, NPIdentifier methodName, const NPVariant *args, uint32_t argCount, NPVariant *result) 
{
	int error = 1;
	char *name = gpnpf->utf8fromidentifier(methodName);
	if(name) 
	{
		if(!strcmp(name, "alert")) 
		{
			if(argCount != 0)
			{
				// 				NPVariant		txt	;
				// 
				// 				txt.value.stringValue.UTF8Characters = "这是NPAPI数据";
				// 				return gpnpf->invoke(0, 0, args[0], &txt, 1, result);

				NPVariant		arg;
				CScriptTable*	pScript		= (CScriptTable *)obj;
				wchar_t*		pString		= L"这里是NPAPI的字符串";
				CHAR			szString[1024]		= {0};

				WideCharToMultiByte(CP_UTF8, 0, pString, (int)wcslen(pString)+1, szString, (int)sizeof(szString), NULL, NULL);

				STRINGN_TO_NPVARIANT(szString, (int)strlen(szString)+1, arg);

				HWND			hWnd		= NULL;
// 				STARTUPINFO		info		= {0};
// 
// 				GetStartupInfo(&info);
// 				EnumWindows(EnumThreadFirstWndProc, (WPARAM)&hWnd);
				gpnpf->getvalue(pScript->m_instance, NPNVnetscapeWindow, (void *)&hWnd);
				
				::MessageBox(hWnd, pString, L"MySimple", MB_OK|MB_ICONINFORMATION);
				// (NPP npp, NPObject* obj, const NPVariant *args, uint32_t argCount, NPVariant *result)
				return gpnpf->invokeDefault(pScript->m_instance, NPVARIANT_TO_OBJECT(args[0]), &arg, 1, result);
			}
			//*result = NULL;
			NULL_TO_NPVARIANT(*result);
			return true;
		}
	}
	// aim exception handling
	gpnpf->setexception(obj, "exception during invocation");
	return false;
}

bool hasProperty(NPObject *obj, NPIdentifier propertyName) 
{
	//logmsg("npsimple: hasProperty\n");

	return false;
}

bool getProperty(NPObject *obj, NPIdentifier propertyName, NPVariant *result) 
{
	// logmsg("npsimple: getProperty\n");
	return false;
}



NPObject* AllocateScriptable(NPP npp, NPClass *aClass)
{
	return new CScriptTable(npp);
}

void DeallocateScriptable(NPObject *obj)
{
	if (!obj) {

		return;
	}

	CScriptTable *s = (CScriptTable *)obj;
	delete s;
}


NPClass npcRefObject = {
	NP_CLASS_STRUCT_VERSION,
	AllocateScriptable,
	DeallocateScriptable,
	NULL,
	hasMethod,
	invoke,
	invokeDefault,
	hasProperty,
	getProperty,
	NULL,
	NULL,
};
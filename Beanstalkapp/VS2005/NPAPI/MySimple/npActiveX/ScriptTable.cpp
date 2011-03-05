#include "StdAfx.h"
#include "ScriptTable.h"

CScriptTable::CScriptTable(NPP npp)
{
	m_instance = npp;
	memset(&m_window, 0, sizeof(m_window));

}

CScriptTable::~CScriptTable()
{
}

// 初始化
void CScriptTable::Init(int16_t argc, char* argn[], char* argv[])
{
	for(int i = 0; i < argc; i++)
	{
		if(strnicmp(argn[i], "param_", 6) == 0)
		{
			m_propery[&argn[i][6]] = argv[i];
		}
		else if(strnicmp(argn[i], "event_", 6) == 0)
		{
			m_method[&argn[i][6]] = argv[i];
		}
		else if(stricmp(argn[i], "clsid") == 0)
		{
			m_clsid = argv[i];
		}
		else if(stricmp(argn[i], "codebase") == 0)
		{
			m_codebase = argv[i];
		}
	}
}
// 设置窗体
void CScriptTable::SetWindow(NPWindow* window)
{
	memcpy(&m_window, window, sizeof(NPWindow));

	RECT	rect		= {0, 0, window->width, window->height};

	if(FALSE == m_dialog.IsWindow())
	{
		m_dialog.Create((HWND)window->window, (LPARAM)this);
	}

	if(m_dialog.IsWindow())
	{
		m_dialog.MoveWindow(&rect, TRUE);
	}
}

//////////////////////////////////////////////////////////////////////////
bool hasMethod(NPObject* obj, NPIdentifier methodName)
{
	return true;
}

bool invokeDefault(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) 
{
	return false;
}

bool invoke(NPObject* obj, NPIdentifier methodName, const NPVariant *args, uint32_t argCount, NPVariant *result) 
{
	gpnpf->setexception(obj, "exception during invocation");
	return false;
}

bool hasProperty(NPObject *obj, NPIdentifier propertyName) 
{
	return false;
}

bool getProperty(NPObject *obj, NPIdentifier propertyName, NPVariant *result) 
{
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
	if(s->m_dialog.IsWindow())
		s->m_dialog.DestroyWindow();
	delete s;
}

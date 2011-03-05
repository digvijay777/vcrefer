#pragma once
#include "../../../../ExtClass/String/MultiString.h"
#include <string>
#include <map>
#include "ActiveContainer.h"
#include <Npapi.h>
#include <prtypes.h>
#include <npfunctions.h>

extern NPNetscapeFuncs*	gpnpf;
//////////////////////////////////////////////////////////////////////////
class CScriptTable : public NPObject
{
public:
	CScriptTable(NPP npp);
	~CScriptTable();

public:
	NPP					m_instance;
	NPWindow			m_window;
	std::string			m_clsid;
	std::string			m_codebase;
	std::map<std::string, std::string>		m_propery;
	std::map<std::string, std::string>		m_method;
	CActiveContainer	m_dialog;

public:
	void	Init(int16_t argc, char* argn[], char* argv[]);
	void	SetWindow(NPWindow* window);
};



//////////////////////////////////////////////////////////////////////////
// ’‚∏ˆ≤‚ ‘

bool hasMethod(NPObject* obj, NPIdentifier methodName);
bool invokeDefault(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result);
bool invoke(NPObject* obj, NPIdentifier methodName, const NPVariant *args, uint32_t argCount, NPVariant *result);
bool hasProperty(NPObject *obj, NPIdentifier propertyName);
bool getProperty(NPObject *obj, NPIdentifier propertyName, NPVariant *result);
NPObject* AllocateScriptable(NPP npp, NPClass *aClass);
void DeallocateScriptable(NPObject *obj);



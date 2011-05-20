#include "nptest.h"
#include "TestApp.h"
#include "TestPlug.h"

CTestApp::CTestApp(void)
{
}

CTestApp::~CTestApp(void)
{
}

NPError CTestApp::New(NPMIMEType pluginType, NPP instance, uint16_t mode 
					  , int16_t argc, char* argn[], char* argv[], NPSavedData* saved)
{
	if(strcmp((char *)pluginType, "application/x-yuanjie-test") == 0)
	{
		instance->pdata = gpnpf->createobject(instance, (NPClass *)&CTestPlug::Object);
	}
	return NPERR_NO_ERROR;
}

NPError CTestApp::Destroy(NPP instance, NPSavedData** save)
{
	if(NULL != instance->pdata && NULL != gpnpf)
	{
		gpnpf->releaseobject((NPObject *)instance->pdata);
	}

	return NPERR_NO_ERROR;
}

NPError CTestApp::GetValue(NPP instance, NPPVariable variable, void *ret_value)
{
	switch(variable) 
	{
	default:
		return NPERR_GENERIC_ERROR;
	case NPPVpluginNameString:
		*((char **)ret_value) = "TestNPP";
		break;
	case NPPVpluginDescriptionString:
		*((char **)ret_value) = "<a href=\"www.yunjie.org\">yuanjie.org</a> plugin.";
		break;
	case NPPVpluginScriptableNPObject:
		gpnpf->retainobject((NPObject *)instance->pdata);
		*((NPObject **)ret_value) = (NPObject *)instance->pdata;
		break;
	case NPPVpluginNeedsXEmbed:
		*((PRBool *)ret_value) = PR_FALSE;
		break;
	}
	return NPERR_NO_ERROR;
}
#include "stdafx.h"

#include "ScriptTable.h"

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

NPNetscapeFuncs*	gpnpf		= NULL;
//////////////////////////////////////////////////////////////////////////
NPError NPP_MyNew(NPMIMEType pluginType, NPP instance, uint16_t mode, int16_t argc
				  , char* argn[], char* argv[], NPSavedData* saved)
{
	// 创建对像
	if(gpnpf->version >= 14)
		instance->pdata = gpnpf->createobject((NPP)instance, &npcRefObject);

	CScriptTable*	pTable		= (CScriptTable *)instance->pdata;

	if(NULL == pTable)
		return NPERR_INVALID_PLUGIN_ERROR;
	// 设置ID
	pTable->Init(argc, argn, argv);

	return NPERR_NO_ERROR;
}

NPError NPP_MyDestroy(NPP instance, NPSavedData** save)
{
	if(NULL != instance->pdata && NULL != gpnpf)
	{
		gpnpf->releaseobject((NPObject *)instance->pdata);
	}

	return NPERR_NO_ERROR;
}

NPError NPP_MyGetValue(NPP instance, NPPVariable variable, void *value)
{
	switch(variable) 
	{
	default:
		return NPERR_GENERIC_ERROR;
	case NPPVpluginNameString:
		*((char **)value) = "AplixFooPlugin";
		break;
	case NPPVpluginDescriptionString:
		*((char **)value) = "<a href=\"mailto:yuanjie.xia@hotmail.com\">Send mail to yuanjie</a>";
		break;
	case NPPVpluginScriptableNPObject:
		gpnpf->retainobject((NPObject *)instance->pdata);
		*(NPObject **)value = (NPObject *)instance->pdata;
		break;
	case NPPVpluginNeedsXEmbed:
		*((PRBool *)value) = PR_FALSE;
		break;
	}
	return NPERR_NO_ERROR;
}
NPError NPP_MySetValue(NPP instance, NPNVariable variable, void *value)
{
	return NPERR_NO_ERROR;
}

NPError NPP_MySetWindow(NPP instance, NPWindow* window)
{
	CScriptTable*	pTable	= (CScriptTable *)instance->pdata;

	if(NULL != pTable)
		pTable->SetWindow(window);

	return NPERR_NO_ERROR;
}
//////////////////////////////////////////////////////////////////////////
NPError OSCALL NP_GetEntryPoints(NPPluginFuncs *nppfuncs) 
{
	//memset(nppfuncs, 0, sizeof(NPPluginFuncs));
	nppfuncs->size			= sizeof(NPPluginFuncs);
	nppfuncs->version       = (NP_VERSION_MAJOR << 8) | NP_VERSION_MINOR;

	nppfuncs->newp = NPP_MyNew;
	nppfuncs->destroy = NPP_MyDestroy;
	nppfuncs->getvalue = NPP_MyGetValue;
	nppfuncs->setwindow = NPP_MySetWindow; // opera必须要用的操作，不然会加载失败
	nppfuncs->javaClass = NULL;

	return NPERR_NO_ERROR;
}

#ifndef HIBYTE
#define HIBYTE(x) ((((uint32)(x)) & 0xff00) >> 8)
#endif

NPError OSCALL NP_Initialize(NPNetscapeFuncs *npnf)
{
	if(npnf == NULL)
		return NPERR_INVALID_FUNCTABLE_ERROR;

	if(HIBYTE(npnf->version) > NP_VERSION_MAJOR)
		return NPERR_INCOMPATIBLE_VERSION_ERROR;

	gpnpf = npnf;

	return NPERR_NO_ERROR;
}

NPError OSCALL NP_Shutdown() 
{
	gpnpf = NULL;

	return NPERR_NO_ERROR;
}

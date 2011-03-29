#include "stdafx.h"
#include "npWebPlugins.h"
#include "npWebObject.h"
#include "TestObject.h"

// 全局的netscape对像
NPNetscapeFuncs*	gpnpf		= NULL;

//////////////////////////////////////////////////////////////////////////
NPError NPP_MyNew(NPMIMEType pluginType, NPP instance, uint16_t mode, int16_t argc
				  , char* argn[], char* argv[], NPSavedData* saved)
{
	//	gpnpf->setvalue(instance, NPPVpluginWindowBool, 0);	// 无窗体组件
	if(gpnpf->version >= 14 && NULL != gpnpf)
		instance->pdata = gpnpf->createobject((NPP)instance, &CNPWebObject::Object);

	CNPWebObject*	pTable		= (CNPWebObject *)instance->pdata;

	if(NULL == pTable)
		return NPERR_INVALID_PLUGIN_ERROR;
	// 设置ID
	pTable->Initialize(argc, argn, argv, CTestObject::CreateObject());

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
		*((char **)value) = "WebRequest";
		break;
	case NPPVpluginDescriptionString:
		*((char **)value) = "<a href=\"www.yunsec.com\">WebRequest</a> plugin.";
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
	return NPERR_NO_ERROR;
}

void NPP_MyPrint(NPP instance, NPPrint* platformPrint)
{
}

NPError NPP_MyHandleEvent(NPP instance, void* event)
{
	return NPERR_NO_ERROR;
}

void NPP_MyStreamAsFile(NPP instance, NPStream* stream, const char* fname)
{
}

void NPP_MyURLNotify(NPP instance, const char* url, NPReason reason, void* notifyData)
{
}
NPError NPP_MyNewsStream(NPP instance, NPMIMEType type, NPStream* stream, NPBool seekable, uint16_t* stype)
{
	return NPERR_NO_ERROR;
}
int32_t NPP_MyWriteready(NPP instance, NPStream* stream)
{
	return 0;
}
NPError NPP_MyDestroyStream(NPP instance, NPStream* stream, NPReason reason)
{
	return NPERR_NO_ERROR;
}
int32_t NPP_MyWrite(NPP instance, NPStream* stream, int32_t offset,
					int32_t len, void* buffer)
{
	return 0;
}
//////////////////////////////////////////////////////////////////////////
NPError OSCALL NP_GetEntryPoints(NPPluginFuncs *nppfuncs) 
{
	nppfuncs->size			= sizeof(NPPluginFuncs);
	nppfuncs->version       = (NP_VERSION_MAJOR << 8) | NP_VERSION_MINOR;

	nppfuncs->newp = NPP_MyNew;
	nppfuncs->destroy = NPP_MyDestroy;
	nppfuncs->getvalue = NPP_MyGetValue;
	nppfuncs->setwindow = NPP_MySetWindow; // opera必须要用的操作，不然会加载失败
	nppfuncs->setvalue = NPP_MySetValue;
	nppfuncs->event = NPP_MyHandleEvent;
	nppfuncs->asfile = NPP_MyStreamAsFile;
	nppfuncs->urlnotify = NPP_MyURLNotify;
	nppfuncs->print = NPP_MyPrint;
	nppfuncs->newstream = NPP_MyNewsStream;
	nppfuncs->writeready = NPP_MyWriteready;
	nppfuncs->destroystream = NPP_MyDestroyStream;
	nppfuncs->write = NPP_MyWrite;	
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

//////////////////////////////////////////////////////////////////////////
BOOL NPVariantToVARIANT(VARIANT* lpVal, const NPVariant var)
{
	if(NPVARIANT_IS_OBJECT(var))
		return FALSE;

	VariantInit(lpVal);
	if(NPVARIANT_IS_BOOLEAN(var))
	{
		lpVal->vt = VT_BOOL;
		lpVal->boolVal = (FALSE != NPVARIANT_TO_BOOLEAN(var))?VARIANT_TRUE:VARIANT_FALSE;
	}
	else if(NPVARIANT_IS_DOUBLE(var))
	{
		lpVal->vt = VT_R8;
		lpVal->dblVal = NPVARIANT_TO_DOUBLE(var);
	}
	else if(NPVARIANT_IS_INT32(var))
	{
		lpVal->vt = VT_I4;
		lpVal->intVal = NPVARIANT_TO_INT32(var);
	}
	else if(NPVARIANT_IS_STRING(var))
	{
		LPCSTR		lpData		= NPVARIANT_TO_STRING(var).UTF8Characters;
		LPWSTR		lpBstr		= NULL;
		LONG		nSize		= 0;

		lpVal->vt = VT_BSTR;
		nSize = MultiByteToWideChar(CP_UTF8, 0, lpData, NPVARIANT_TO_STRING(var).UTF8Length, NULL, 0);
		if(0 == nSize)
			lpVal->bstrVal = ::SysAllocString(L"");
		else
		{
			nSize++;
			lpBstr = new WCHAR[nSize];
			memset(lpBstr, 0, nSize * sizeof(WCHAR));
			MultiByteToWideChar(CP_UTF8, 0, lpData, NPVARIANT_TO_STRING(var).UTF8Length, lpBstr, nSize);
			lpVal->bstrVal = ::SysAllocString(lpBstr);
			delete [] lpBstr;
		}
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

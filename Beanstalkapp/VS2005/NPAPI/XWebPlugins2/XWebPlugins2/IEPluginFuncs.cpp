#include "StdAfx.h"
#include "IEPluginFuncs.h"

NPPluginFuncs		gIEPlugFuncs	= {
	sizeof(NPPluginFuncs),
	14,
	IE_NPP_New,
	IE_NPP_Destroy,
	IE_NPP_SetWindow,
	IE_NPP_NewStream,
	IE_NPP_DestroyStream,
	IE_NPP_StreamAsFile,
	IE_NPP_WriteReady,
	IE_NPP_Write,
	IE_NPP_Print,
	IE_NPP_HandleEvent,
	IE_NPP_URLNotify,
	NULL,
	IE_NPP_GetValue,
	IE_NPP_SetValue,
};

NPError NP_LOADDS IE_NPP_New(NPMIMEType pluginType, NPP instance, uint16_t mode
							 , int16_t argc, char* argn[], char* argv[], NPSavedData* saved)
{
	ATLASSERT(FALSE);
	return NPERR_NO_ERROR;
}
NPError NP_LOADDS IE_NPP_Destroy(NPP instance, NPSavedData** save)
{
	ATLASSERT(FALSE);
	return NPERR_NO_ERROR;
}
NPError NP_LOADDS IE_NPP_SetWindow(NPP instance, NPWindow* window)
{
	ATLASSERT(FALSE);
	return NPERR_NO_ERROR;
}
NPError NP_LOADDS IE_NPP_NewStream(NPP instance, NPMIMEType type, NPStream* stream, NPBool seekable, uint16_t* stype)
{
	ATLASSERT(FALSE);
	return NPERR_NO_ERROR;
}
NPError NP_LOADDS IE_NPP_DestroyStream(NPP instance, NPStream* stream, NPReason reason)
{
	ATLASSERT(FALSE);
	return NPERR_NO_ERROR;
}
void    NP_LOADDS IE_NPP_StreamAsFile(NPP instance, NPStream* stream, const char* fname)
{
	ATLASSERT(FALSE);
}
int32_t NP_LOADDS IE_NPP_WriteReady(NPP instance, NPStream* stream)
{
	ATLASSERT(FALSE);
	return 0;
}
int32_t NP_LOADDS IE_NPP_Write(NPP instance, NPStream* stream, int32_t offset, int32_t len, void* buffer)
{
	ATLASSERT(FALSE);
	return 0;
}
void    NP_LOADDS IE_NPP_Print(NPP instance, NPPrint* platformPrint)
{
	ATLASSERT(FALSE);
}
int16_t NP_LOADDS IE_NPP_HandleEvent(NPP instance, void* event)
{
	ATLASSERT(FALSE);
	return 0;
}
void    NP_LOADDS IE_NPP_URLNotify(NPP instance, const char* url, NPReason reason, void* notifyData)
{
	ATLASSERT(FALSE);
}
NPError NP_LOADDS IE_NPP_GetValue(NPP instance, NPPVariable variable, void *value)
{
	ATLASSERT(FALSE);
	return NPERR_NO_ERROR;
}
NPError NP_LOADDS IE_NPP_SetValue(NPP instance, NPNVariable variable, void *value)
{
	ATLASSERT(FALSE);
	return NPERR_NO_ERROR;
}
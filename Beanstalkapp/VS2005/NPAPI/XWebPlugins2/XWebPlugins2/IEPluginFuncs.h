#pragma once
#include "npWebPlugins.h"

extern NPPluginFuncs		gIEPlugFuncs;

NPError NP_LOADDS IE_NPP_New(NPMIMEType pluginType, NPP instance, uint16_t mode, int16_t argc, char* argn[], char* argv[], NPSavedData* saved);
NPError NP_LOADDS IE_NPP_Destroy(NPP instance, NPSavedData** save);
NPError NP_LOADDS IE_NPP_SetWindow(NPP instance, NPWindow* window);
NPError NP_LOADDS IE_NPP_NewStream(NPP instance, NPMIMEType type, NPStream* stream, NPBool seekable, uint16_t* stype);
NPError NP_LOADDS IE_NPP_DestroyStream(NPP instance, NPStream* stream, NPReason reason);
void    NP_LOADDS IE_NPP_StreamAsFile(NPP instance, NPStream* stream, const char* fname);
int32_t NP_LOADDS IE_NPP_WriteReady(NPP instance, NPStream* stream);
int32_t NP_LOADDS IE_NPP_Write(NPP instance, NPStream* stream, int32_t offset, int32_t len, void* buffer);
void    NP_LOADDS IE_NPP_Print(NPP instance, NPPrint* platformPrint);
int16_t NP_LOADDS IE_NPP_HandleEvent(NPP instance, void* event);
void    NP_LOADDS IE_NPP_URLNotify(NPP instance, const char* url, NPReason reason, void* notifyData);
NPError NP_LOADDS IE_NPP_GetValue(NPP instance, NPPVariable variable, void *value);
NPError NP_LOADDS IE_NPP_SetValue(NPP instance, NPNVariable variable, void *value);

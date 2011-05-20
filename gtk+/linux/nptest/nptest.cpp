#include "nptest.h"

NPNetscapeFuncs*	gpnpf;

/*
 *	描述信息
 */
char* NP_GetMIMEDescription(void)
{
	return "application/yuanjie-x-test:bsp:Basic Example Plug-in for Mozilla";
}

/*
 *	获取列表入口
 */
NPError OSCALL NP_GetEntryPoints(NPPluginFuncs *nppfuncs) 
{
	nppfuncs->size			= sizeof(NPPluginFuncs);
	nppfuncs->version       = (NP_VERSION_MAJOR << 8) | NP_VERSION_MINOR;

// 	nppfuncs->newp = NPP_MyNew;
// 	nppfuncs->destroy = NPP_MyDestroy;
// 	nppfuncs->getvalue = NPP_MyGetValue;
// 	nppfuncs->setwindow = NPP_MySetWindow; // opera必须要用的操作，不然会加载失败
// 	nppfuncs->setvalue = NPP_MySetValue;
// 	nppfuncs->event = NPP_MyHandleEvent;
// 	nppfuncs->asfile = NPP_MyStreamAsFile;
// 	nppfuncs->urlnotify = NPP_MyURLNotify;
// 	nppfuncs->print = NPP_MyPrint;
// 	nppfuncs->newstream = NPP_MyNewsStream;
// 	nppfuncs->writeready = NPP_MyWriteready;
// 	nppfuncs->destroystream = NPP_MyDestroyStream;
// 	nppfuncs->write = NPP_MyWrite;	
// 	nppfuncs->javaClass = NULL;

	return NPERR_NO_ERROR;
}

#ifndef HIBYTE
#define HIBYTE(x) ((((uint32)(x)) & 0xff00) >> 8)
#endif

NPError OSCALL NP_Initialize(NPNetscapeFuncs *npnf)
{
	if(npnf == NULL)
		return NPERR_INVALID_FUNCTABLE_ERROR;

// 	if(HIBYTE(npnf->version) > NP_VERSION_MAJOR)
// 		return NPERR_INCOMPATIBLE_VERSION_ERROR;

	gpnpf = npnf;

	return NPERR_NO_ERROR;
}

NPError OSCALL NP_Shutdown() 
{
	gpnpf = NULL;

	return NPERR_NO_ERROR;
}

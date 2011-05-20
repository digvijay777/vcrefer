#include "nptest.h"
#include "TestPlug.h"
#include "Testapp.h"

NPNetscapeFuncs*	gpnpf;

//////////////////////////////////////////////////////////////////////////
/*
 *	描述信息
 */
char* NP_GetMIMEDescription(void)
{
	return "application/x-yuanjie-test:bsp:Basic Example Plug-in for Mozilla";
}

/*
 *	获取列表入口
 */
NPError OSCALL NP_GetEntryPoints(NPPluginFuncs *nppfuncs) 
{
	*nppfuncs = CTestApp::PluginFuncs;

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

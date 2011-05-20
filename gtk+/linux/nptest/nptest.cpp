#include "nptest.h"
#include "testplug.h"
#include "testapp.h"

#ifndef HIBYTE
#define HIBYTE(x) ((((uint32)(x)) & 0xff00) >> 8)
#endif

NPNetscapeFuncs*	gpnpf;

//////////////////////////////////////////////////////////////////////////
/*
 *	描述信息
 */
char* NP_GetMIMEDescription(void)
{
	return (char *)"application/x-yuanjie-test:bsp:Basic Example Plug-in for Mozilla";
}

/*
 *	获取列表入口
 */

#if defined(XP_UNIX)
// linux/unix 版本 
NPError OSCALL NP_Initialize(NPNetscapeFuncs* npnf, NPPluginFuncs* nppfuncs)
{
	printf("[NP_Initialize] enter\n");
	*nppfuncs = CTestApp::PluginFuncs;
	
	if(npnf == NULL)
		return NPERR_INVALID_FUNCTABLE_ERROR;

	if(HIBYTE(npnf->version) > NP_VERSION_MAJOR)
		return NPERR_INCOMPATIBLE_VERSION_ERROR;

	gpnpf = npnf;

	return NPERR_NO_ERROR;

}

NPError NP_GetValue(void *future, NPPVariable aVariable, void *aValue)
{
	CTestApp::_class.GetValue((NPP)future, aVariable, aValue);
}

char* NP_GetPluginVersion()
{
	return (char *)"v1.0";
}
#else
// windows 版本
NPError OSCALL NP_GetEntryPoints(NPPluginFuncs *nppfuncs) 
{
	*nppfuncs = CTestApp::PluginFuncs;

	return NPERR_NO_ERROR;
}

NPError OSCALL NP_Initialize(NPNetscapeFuncs *npnf)
{
	if(npnf == NULL)
		return NPERR_INVALID_FUNCTABLE_ERROR;

	if(HIBYTE(npnf->version) > NP_VERSION_MAJOR)
		return NPERR_INCOMPATIBLE_VERSION_ERROR;

	gpnpf = npnf;

	return NPERR_NO_ERROR;
}
#endif // XP_UNIX

NPError OSCALL NP_Shutdown() 
{
	gpnpf = NULL;

	return NPERR_NO_ERROR;
}


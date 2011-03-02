#include <Windows.h>
#include <Npapi.h>
#include <prtypes.h>
#include <npfunctions.h>
#include "simple.h"


NPNetscapeFuncs*	gpnpf		= NULL;
//////////////////////////////////////////////////////////////////////////
NPError NPP_MyNew(NPMIMEType pluginType, NPP instance, uint16_t mode, int16_t argc
				  , char* argn[], char* argv[], NPSavedData* saved)
{
	// 设置为无窗口对像
// 	NPVariant		var;
// 	int				nb		= 0;
// 
// 	BOOLEAN_TO_NPVARIANT(false, var);
// 	gpnpf->setvalue(instance, NPPVpluginWindowBool, &nb);
// 	gpnpf->setvalue(instance, NPPVpluginWindowBool, &var);
	gpnpf->setvalue(instance, NPPVpluginWindowBool, NULL);	// 强制绘制动作

	// 创建对像
	if(gpnpf->version >= 14)
		instance->pdata = gpnpf->createobject((NPP)instance, &npcRefObject);
	//instance->pdata = new char[1024];
	//bool		n = 0;
	return NPERR_NO_ERROR;
}

NPError NPP_MyDestroy(NPP instance, NPSavedData** save)
{
	if(NULL != instance->pdata && NULL != gpnpf)
	{
		gpnpf->releaseobject((NPObject *)instance->pdata);
	}

	//delete [] instance->pdata;
	return NPERR_NO_ERROR;
}
NPError NPP_MyGetValue(NPP instance, NPPVariable variable, void *value)
{
	switch(variable) 
	{
	default:
		//logmsg("npsimple: getvalue - default\n");
		return NPERR_GENERIC_ERROR;
	case NPPVpluginNameString:
		//logmsg("npsimple: getvalue - name string\n");
		*((char **)value) = "AplixFooPlugin";
		break;
	case NPPVpluginDescriptionString:
		//logmsg("npsimple: getvalue - description string\n");
		*((char **)value) = "<a href=\"http://www.aplix.co.jp/\">AplixFooPlugin</a> plugin.";
		break;
	case NPPVpluginScriptableNPObject:
		//logmsg("npsimple: getvalue - scriptable object\n");
		if(NULL == instance->pdata && NULL != gpnpf)
			instance->pdata = gpnpf->createobject(instance, &npcRefObject);
		gpnpf->retainobject((NPObject *)instance->pdata);
		*(NPObject **)value = (NPObject *)instance->pdata;
		//gObjects[instance]->_class.setProperty();
		break;
	case NPPVpluginNeedsXEmbed:
		//logmsg("npsimple: getvalue - xembed\n");
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
// 	window->type = NPWindowTypeDrawable;

	HWND			hWnd		= NULL;

	gpnpf->getvalue(instance, NPNVnetscapeWindow, (void *)&hWnd);
 	gpnpf->forceredraw(instance);
//	window->type = NPWindowTypeDrawable;
// 	gpnpf->invalidaterect(instance, &window->clipRect);
// 	gpnpf->invalidateregion(instance, window);
	CScriptTable*	pTable	= (CScriptTable *)instance->pdata;
	memcpy(&pTable->m_window, window, sizeof(NPWindow));
	return NPERR_NO_ERROR;
}

void NPP_MyPrint(NPP instance, NPPrint* platformPrint)
{
	int i = 0;//return NPERR_NO_ERROR;
}

NPError NPP_MyHandleEvent(NPP instance, void* event)
{
	NPEvent*		pEvent		= (NPEvent *)event;

	if(WM_PAINT == pEvent->event)
	{
		CScriptTable*		pTable		= (CScriptTable *)instance->pdata;
		HWND				hWnd		= NULL;
		HDC					hDC			= NULL;
		RECT				rect		= {pTable->m_window.x, pTable->m_window.y
			, pTable->m_window.x + pTable->m_window.width, pTable->m_window.y + pTable->m_window.height};

		gpnpf->getvalue(instance, NPNVnetscapeWindow, (void *)&hWnd);
		hDC = ::GetDC(hWnd);
		

		::SetBkColor(hDC, RGB(0xff, 0x0, 0x0));
		::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);

		::ReleaseDC(hWnd, hDC);
	}
	return NPERR_NO_ERROR;
}

void NPP_MyStreamAsFile(NPP instance, NPStream* stream, const char* fname)
{
	int i = 0;
	//return NPERR_NO_ERROR;
}

void NPP_MyURLNotify(NPP instance, const char* url, NPReason reason, void* notifyData)
{
	int i = 0;
	//return NPERR_NO_ERROR;
}
//////////////////////////////////////////////////////////////////////////
// 入口函数
NPError OSCALL NP_GetEntryPoints(NPPluginFuncs *nppfuncs) 
{
	//memset(nppfuncs, 0, sizeof(NPPluginFuncs));
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
	//NP_GetEntryPoints(nppf);

	return NPERR_NO_ERROR;
}

NPError OSCALL NP_Shutdown() 
{
	gpnpf = NULL;

	return NPERR_NO_ERROR;
}


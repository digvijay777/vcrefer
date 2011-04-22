#include "StdAfx.h"
#include "IENetscapeFuncs.h"

NPNetscapeFuncs		gIENpFuncs		= {
	sizeof(NPNetscapeFuncs);
	0;
	IE_NPN_GetURLProcPtr;
	IE_NPN_PostURLProcPtr;
	IE_NPN_RequestReadProcPtr;
	IE_NPN_NewStreamProcPtr;
	NPN_WriteProcPtr;
	NPN_DestroyStreamProcPtr destroystream;
	NPN_StatusProcPtr status;
	NPN_UserAgentProcPtr uagent;
	NPN_MemAllocProcPtr memalloc;
	NPN_MemFreeProcPtr memfree;
	NPN_MemFlushProcPtr memflush;
	NPN_ReloadPluginsProcPtr reloadplugins;
	NPN_GetJavaEnvProcPtr getJavaEnv;
	NPN_GetJavaPeerProcPtr getJavaPeer;
	NPN_GetURLNotifyProcPtr geturlnotify;
	NPN_PostURLNotifyProcPtr posturlnotify;
	NPN_GetValueProcPtr getvalue;
	NPN_SetValueProcPtr setvalue;
	NPN_InvalidateRectProcPtr invalidaterect;
	NPN_InvalidateRegionProcPtr invalidateregion;
	NPN_ForceRedrawProcPtr forceredraw;
	NPN_GetStringIdentifierProcPtr getstringidentifier;
	NPN_GetStringIdentifiersProcPtr getstringidentifiers;
	NPN_GetIntIdentifierProcPtr getintidentifier;
	NPN_IdentifierIsStringProcPtr identifierisstring;
	NPN_UTF8FromIdentifierProcPtr utf8fromidentifier;
	NPN_IntFromIdentifierProcPtr intfromidentifier;
	NPN_CreateObjectProcPtr createobject;
	NPN_RetainObjectProcPtr retainobject;
	NPN_ReleaseObjectProcPtr releaseobject;
	NPN_InvokeProcPtr invoke;
	NPN_InvokeDefaultProcPtr invokeDefault;
	NPN_EvaluateProcPtr evaluate;
	NPN_GetPropertyProcPtr getproperty;
	NPN_SetPropertyProcPtr setproperty;
	NPN_RemovePropertyProcPtr removeproperty;
	NPN_HasPropertyProcPtr hasproperty;
	NPN_HasMethodProcPtr hasmethod;
	NPN_ReleaseVariantValueProcPtr releasevariantvalue;
	NPN_SetExceptionProcPtr setexception;
	NPN_PushPopupsEnabledStateProcPtr pushpopupsenabledstate;
	NPN_PopPopupsEnabledStateProcPtr poppopupsenabledstate;
	NPN_EnumerateProcPtr enumerate;
	NPN_PluginThreadAsyncCallProcPtr pluginthreadasynccall;
	NPN_ConstructProcPtr construct;
	NPN_GetValueForURLPtr getvalueforurl;
	NPN_SetValueForURLPtr setvalueforurl;
	NPN_GetAuthenticationInfoPtr getauthenticationinfo;

};


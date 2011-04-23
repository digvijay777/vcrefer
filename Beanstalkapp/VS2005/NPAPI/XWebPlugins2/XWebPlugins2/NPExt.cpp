#include "stdafx.h"
#include "npWebPlugins.h"

bool StringToNPVariantV(NPVariant& npvar, char* pfmt, va_list arr)
{
	CHAR		szBuf[1024]		= {0};
	CHAR*		pStr			= NULL;
	int			nSize			= 0;

	vsnprintf(szBuf, sizeof(szBuf), pfmt, arr);
	nSize = strlen(szBuf) + 1;
	pStr = (char *)gpnpf->memalloc(nSize);
	strcpy(pStr, szBuf);
	STRINGZ_TO_NPVARIANT(pStr, npvar);
	return true;
}
bool StringToNPVariant(NPVariant& npvar, char* pfmt, ...)
{
	va_list		arr;
	bool		bret;

	va_start(arr, pfmt);
	bret = StringToNPVariantV(npvar, pfmt, arr);
	va_end(arr);
	return bret;
}
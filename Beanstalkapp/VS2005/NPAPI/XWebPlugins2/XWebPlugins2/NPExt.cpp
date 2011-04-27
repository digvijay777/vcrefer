#include "stdafx.h"
#include "npWebPlugins.h"
#include "NPExt.h"
#include "IENAPAI.h"

bool StringToNPVariantV(NPVariant& npvar, const char* pfmt, va_list arr)
{
	CHAR		szBuf[1024]		= {0};
	CHAR*		pStr			= NULL;
	CHAR*		putf8			= NULL;
	int			nSize			= 0;

	vsnprintf(szBuf, sizeof(szBuf), pfmt, arr);
	// ×Ö·û¼¯×ª»»
	putf8 = AnsiToUtf8(szBuf);
	nSize = strlen(putf8) + 1;
	pStr = (char *)gpnpf->memalloc(nSize);
	strcpy(pStr, putf8);
	free(putf8);
	STRINGZ_TO_NPVARIANT(pStr, npvar);
	return true;
}
bool StringToNPVariant(NPVariant& npvar, const char* pfmt, ...)
{
	va_list		arr;
	bool		bret;

	va_start(arr, pfmt);
	bret = StringToNPVariantV(npvar, pfmt, arr);
	va_end(arr);
	return bret;
}
bool ClearStringNPVariant(NPVariant& npvar)
{
	if(FALSE == NPVARIANT_IS_STRING(npvar))
		return false;
	gpnpf->memfree((void *)npvar.value.stringValue.UTF8Characters);
	return true;
}

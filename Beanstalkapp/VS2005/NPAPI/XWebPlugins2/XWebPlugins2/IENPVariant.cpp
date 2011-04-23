#include "stdafx.h"
#include "IENAPAI.h"

// 清除NPVariant变量内容
void	IEClearNPVariant(NPVariant& npvar)
{
	if(NPVARIANT_IS_STRING(npvar))
	{
		NPString	str = NPVARIANT_TO_STRING(npvar);

		if(NULL != str.UTF8Characters)
			free((char *)str.UTF8Characters);
	}
	else if(NPVARIANT_IS_OBJECT(npvar))
	{
		NPObject*	pObject		= NPVARIANT_TO_OBJECT(npvar);

		if(NULL != pObject)
			gIENpFuncs.releaseobject(pObject);

	}
	NULL_TO_NPVARIANT(npvar);
}

// 类型转换
bool		IEVariantToNPVariant(NPVariant& npvar, const VARIANT& ievar)
{
	NULL_TO_NPVARIANT(npvar);

	switch(ievar.vt)
	{
	case VT_BOOL:
		{
			BOOLEAN_TO_NPVARIANT((VARIANT_FALSE != ievar.boolVal)?true:false, npvar);
		}
		break;
	case VT_I1:
	case VT_I2:
	case VT_I4:
	case VT_I8:
	case VT_UI1:
	case VT_UI2:
	case VT_UI4:
	case VT_UI8:
		{
			INT32_TO_NPVARIANT(ievar.lVal, npvar);
		}
		break;
	case VT_BSTR:
		{
			NPUTF8*			putf8	= GetUTF8(ievar.bstrVal);
			
			STRINGZ_TO_NPVARIANT(putf8, npvar);
		}
		break;
	case VT_R4:
	case VT_R8:
		{
			DOUBLE_TO_NPVARIANT(ievar.dblVal, npvar);
		}
		break;
	case VT_NULL:
	case VT_EMPTY:
		{
			NULL_TO_NPVARIANT(npvar);
		}
		break;
	case VT_DISPATCH:
		{
			NPP_t		nppt		= {0};
			NPObject*	pObject		= NULL;

			nppt.ndata = ievar.pdispVal;
			pObject = gIENpFuncs.createobject(&nppt, &gNPDispatch);
			OBJECT_TO_NPVARIANT(pObject, npvar);
		}
		break;
	default:
		return false;
	}

	return true;
}

bool		IENPVariantToVariant(VARIANT& ievar, const NPVariant& npvar)
{
	VariantInit(&ievar);


	if(NPVARIANT_IS_OBJECT(npvar))
	{
		NPObject*		pObject		= NPVARIANT_TO_OBJECT(npvar);

		if(NULL == pObject)
			return false;
		ievar.vt = VT_DISPATCH;
		((CNPDispatchEx *)pObject)->QueryInterface(IID_IDispatch, (void **)&ievar.pdispVal);
	}
	else if(NPVARIANT_IS_BOOLEAN(npvar))
	{
		ievar.vt = VT_BOOL;
		ievar.boolVal = (FALSE != NPVARIANT_TO_BOOLEAN(npvar))?VARIANT_TRUE:VARIANT_FALSE;
	}
	else if(NPVARIANT_IS_DOUBLE(npvar))
	{
		ievar.vt = VT_R8;
		ievar.dblVal = NPVARIANT_TO_DOUBLE(npvar);
	}
	else if(NPVARIANT_IS_INT32(npvar))
	{
		ievar.vt = VT_I4;
		ievar.intVal = NPVARIANT_TO_INT32(npvar);
	}
	else if(NPVARIANT_IS_STRING(npvar))
	{
		LPCSTR		lpData		= NPVARIANT_TO_STRING(npvar).UTF8Characters;
		
		ievar.vt = VT_BSTR;
		ievar.bstrVal = Utf8ToBSTR(lpData);
	}
	else
	{
		return false;
	}

	return true;
}
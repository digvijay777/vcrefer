#pragma once

// 全局的Netscape函数
extern NPNetscapeFuncs*	gpnpf;

/*
 *	NPVariant类模板
 */
template <class T = void>
class TNPVariant : public NPVariant
{
public:
	TNPVariant()
	{
	}
	~TNPVariant()
	{
		Clear();
	}

public:
	bool Clear()
	{
		if(NULL == gpnpf)
		{
			assert(NULL != gpnpf);
			return false;
		}
		if(NPVARIANT_IS_OBJECT(*this))
		{
			gpnpf->releaseobject(NPVARIANT_TO_OBJECT(*this));
		}
		else if(NPVARIANT_IS_STRING(*this))
		{
			gpnpf->memfree((void *)this->value.stringValue.UTF8Characters);
		}

		NULL_TO_NPVARIANT(*this);
		return true;
	}
public:
	TNPVariant& operator = (long ul)
	{
		Clear();

		INT32_TO_NPVARIANT(ul, *this);
		return *this;
	}

	TNPVariant& operator = (double d)
	{
		Clear();

		DOUBLE_TO_NPVARIANT(d, *this);
		return *this;
	}

	TNPVariant& operator = (bool b)
	{
		Clear();

		BOOLEAN_TO_NPVARIANT(b, *this);
		return *this;
	}

	TNPVariant& operator = (const char* pstr)
	{
		if( !Clear() )
			return *this;

		char*			pMem		= NULL;

		pMem = (char *)gpnpf->memalloc(strlen(pstr)+1);
		strncpy(pMem, pstr, strlen(pstr)+1);
		STRINGZ_TO_NPVARIANT(pMem, *this);
		return *this;
	}

public:
	NPVariant		Detach()
	{
		NPVariant		var;

		var = *this;
		NULL_TO_NPVARIANT(var);
		return var;
	}
};

typedef TNPVariant<void>		CNPVariant;

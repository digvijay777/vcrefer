#pragma once

CHAR* GetUTF8(LPCWSTR lpStr);
CHAR* AnsiToUtf8(LPCSTR lpStr);

template <class T = void>
class TNPVariant : public NPVariant
{
public:
	TNPVariant(NPNetscapeFuncs* pnpf)
	{
		m_pnpf = pnpf;
	}
	~TNPVariant()
	{
		Clear();
	}

private:
	NPNetscapeFuncs*	m_pnpf;
public:
	void Clear()
	{
		if(NPVARIANT_IS_OBJECT(*this))
		{
			m_pnpf->releaseobject(NPVARIANT_TO_OBJECT(*this));
		}
		else if(NPVARIANT_IS_STRING(*this))
		{
			m_pnpf->memfree((void *)this->value.stringValue.UTF8Characters);
		}

		NULL_TO_NPVARIANT(*this);
	}
public:
	TNPVariant& operator = (LONG ul)
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
		Clear();
		char*	putf8		= AnsiToUtf8(pstr);
		char*	pmem		= NULL;

		pmem = (char *)m_pnpf->memalloc(strlen(putf8)+1);
		strncpy(pmem, putf8, strlen(putf8)+1);
		free(putf8);
		STRINGZ_TO_NPVARIANT(pmem, *this);
		return *this;
	}

	TNPVariant& operator = (const wchar_t* pwstr)
	{
		Clear();
		char*	putf8		= GetUTF8(pwstr);
		char*	pmem		= NULL;

		pmem = (char *)m_pnpf->memalloc(strlen(putf8)+1);
		strncpy(pmem, putf8, strlen(putf8)+1);
		free(putf8);
		STRINGZ_TO_NPVARIANT(pmem, *this);
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
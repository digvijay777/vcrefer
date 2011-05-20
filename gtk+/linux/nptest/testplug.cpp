#include "nptest.h"
#include "testplug.h"

CTestPlug::CTestPlug(NPP npp)
: TNPObject<CTestPlug>(npp)
{
}

CTestPlug::~CTestPlug(void)
{
}

bool CTestPlug::HasMethod(NPIdentifier name)
{
	char*		pname	= gpnpf->utf8fromidentifier(name);

	if(strcmp(pname, "test")==0)
		return true;
	return false;
}

bool CTestPlug::Invoke(NPIdentifier name, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	char*		pname	= gpnpf->utf8fromidentifier(name);

	if(strcmp(pname, "test")==0)
	{
		*result = CNPVariant("This is plug string.").Detach();
		return true;
	}

	return false;
}

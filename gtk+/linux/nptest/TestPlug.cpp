#include "nptest.h"
#include "TestPlug.h"

CTestPlug::CTestPlug(NPP npp)
: TNPObject<CTestPlug>(npp)
{
}

CTestPlug::~CTestPlug(void)
{
}

bool CTestPlug::HasMethod(NPIdentifier name)
{
	return false;
}

bool CTestPlug::Invoke(NPIdentifier name, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	return false;
}
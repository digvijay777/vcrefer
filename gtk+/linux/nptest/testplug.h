#pragma once

class CTestPlug : public TNPObject<CTestPlug>
{
public:
	CTestPlug(NPP npp);
public:
	~CTestPlug(void);

public:
	bool	HasMethod(NPIdentifier name);
	bool	Invoke(NPIdentifier name, const NPVariant *args, uint32_t argCount, NPVariant *result);

};

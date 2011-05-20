#pragma once

class CTestApp : public TNpApp<CTestApp>
{
public:
	CTestApp(void);
	~CTestApp(void);

public:
	NPError		New(NPMIMEType pluginType, NPP instance, uint16_t mode
		, int16_t argc, char* argn[], char* argv[], NPSavedData* saved);
	NPError		Destroy(NPP instance, NPSavedData** save);
	NPError		GetValue(NPP instance, NPPVariable variable, void *ret_value);
};

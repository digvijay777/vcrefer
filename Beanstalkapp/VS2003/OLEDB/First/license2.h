[
	db_source(L"Provider=SQLOLEDB.1;User ID=sa;Password=123456;Persist Security Info=True;Initial Catalog=vms;Network Address=192.168.1.2"),
	db_command("select UserSN, PermitClient, AuthorizationDate from license2")
]
class CAuthors
{
public:
	DWORD m_dwAuIDStatus;
	DWORD m_dwAuthorStatus;
	DWORD m_dwYearBornStatus;
	DWORD m_dwAuIDLength;
	DWORD m_dwAuthorLength;
	DWORD m_dwYearBornLength;
	[ db_column(1, status=m_dwAuIDStatus, length=m_dwAuIDLength) ] TCHAR m_szID[50];
	[ db_column(2, status=m_dwAuthorStatus, length=m_dwAuthorLength) ] LONG m_lClient;
	[ db_column(3, status=m_dwYearBornStatus, length=m_dwYearBornLength) ] TCHAR m_szTime[20];
	void GetRowsetProperties(CDBPropSet* pPropSet)
	{
		pPropSet->AddProperty(DBPROP_CANFETCHBACKWARDS, true);
		pPropSet->AddProperty(DBPROP_CANSCROLLBACKWARDS, true);
		pPropSet->AddProperty(DBPROP_IRowsetChange, true);
	}
};

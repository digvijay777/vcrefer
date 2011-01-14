#pragma once

#ifndef _ConnectionPtr
#import "C:\Program Files\Common Files\System\ADO\msado15.dll" \
	no_namespace rename("EOF", "adoEOF")
#endif
//////////////////////////////////////////////////////////////////////////
// QuickADO
#ifndef ATLTRACE
#ifdef TRACE
#define ATLTRACE		TRACE
#else
#define ATLTRACE
#endif
#endif
#ifndef ATLASSERT
#ifdef ASSERT
#define ATLASSERT		ASSERT
#else
#define ATLASSERT
#endif
#endif

//////////////////////////////////////////////////////////////////////////
// 操作定义
#define GetRecordItemValue(val, def, type)			( ((val).vt != VT_NULL && (val).vt != VT_EMPTY)?(type)(val):(type)(def) )
#define GetRecordValueEx(res, key, def, type)		( ( ((res) == NULL) || (VARIANT_FALSE != (res)->adoEOF) )?(type)def:(type) \
	GetRecordItemValue((res)->Fields->GetItem(_variant_t(key))->Value, def, type) )

// 得到Record的值
#pragma warning(disable: 4101)

template <typename T>
T GetRecordValueT(_RecordsetPtr& res, _variant_t key, T def)
{
	try
	{
		return GetRecordValueEx(res, key, def, T);
	}
	catch(_com_error& e)
	{
		ATLTRACE("GetRecordValueT '%s' error: %s", (LPCSTR)(_bstr_t)key, (LPCSTR)(_bstr_t)e.Description());
		ATLASSERT(FALSE);
	}
	return def;
}
#pragma warning(default: 4101)
// 是不到RECODE结束
BOOL inline IsRecordEofEx(_RecordsetPtr& res)	
{
	try
	{
		if(VARIANT_FALSE == res->adoEOF)
			return FALSE;
	}
	catch(_com_error& )
	{
	}

	return TRUE;
}

#define GetRecordValue(res, key, def, type)		GetRecordValueT<type>(res, key, def)
#define GetRecordStringA(res, key)				(LPCSTR)GetRecordValue(res, key, "", _bstr_t)
#define GetRecordStringExA(res, key, def)		(LPCSTR)GetRecordValue(res, key, def, _bstr_t)
#define GetRecordStringW(res, key)				(LPCWSTR)GetRecordValue(res, key, L"", _bstr_t)
#define GetRecordStringExW(res, key, def)		(LPCWSTR)GetRecordValue(res, key, def, _bstr_t)
#define GetRecordLong(res, key)					GetRecordValue(res, key, 0L, LONG)
#define GetRecordLongEx(res, key, def)			GetRecordValue(res, key, def, LONG)
#define GetRecordULong(res, key)				GetRecordValue(res, key, 0L, ULONG)
#define GetRecordULongEx(res, key, def)			GetRecordValue(res, key, def, ULONG)
#define GetRecordBSTR(res, key)					GetRecordValue(res, key, L"", _bstr_t)

#define IsRecordEof(res)						( ((res) == NULL)?TRUE:(VARIANT_FALSE != (res)->adoEOF) )

#ifdef _UNICODE
#define GetRecordString							GetRecordStringA
#define GetRecordStringEx						GetRecordStringExA
#else
#define GetRecordString							GetRecordStringW
#define GetRecordStringEx						GetRecordStringExW
#endif

//////////////////////////////////////////////////////////////////////////
class AdoSetLastError
{
public:
	AdoSetLastError()
	{
		memset(m_szErr, 0, sizeof(m_szErr));
	}

	void SetLastError(LPCSTR lpFmt, ...)
	{
		va_list		vaArr;

		va_start(vaArr, lpFmt);
		_vsnprintf(m_szErr, sizeof(m_szErr)-2, lpFmt, vaArr);
		va_end(vaArr);
		strcat(m_szErr, "\n");
		ATLTRACE(lpFmt);
	}

	LPCSTR GetLastError()
	{
		return m_szErr;
	}

protected:
	char		m_szErr[1024];
	
};


template <class T, class SetError = AdoSetLastError>
class TQuickADO : public SetError
{
public:
	TQuickADO()
	{
		m_nLastAffect = 0;
		m_nTryCnn = 0;
	}
	~TQuickADO()
	{

	}
	TQuickADO(_ConnectionPtr& pCnn)
	{
		try
		{
			pCnn->QueryInterface(&m_spConn);
		}
		catch(_com_error&)
		{
		}
	}
public:
	_ConnectionPtr		m_spConn;			
protected:
	int					m_nLastAffect;
	int					m_nTryCnn;				// 连续五次失败就认为是断开

public:
	// 判断是否打开， 如果没有打开就关闭
	BOOL IsOpen(BOOL bAutoConn = TRUE)
	{
		if(NULL == m_spConn)
			return FALSE;

		if(m_nTryCnn > 5 && FALSE != bAutoConn)
		{
			// 重新连接
			_bstr_t		strCnn		= m_spConn->GetConnectionString();
			
			m_spConn.Release();
			return Open(strCnn);
		}

		if(m_spConn->GetState() == adStateOpen)
			return TRUE;

		return FALSE;
	}
	// 得到连接字串
	_bstr_t GetConnectionString()
	{
		if(NULL == m_spConn)
			return "";

		return m_spConn->GetConnectionString();
	}
	// 连接
	BOOL Open(LPCWSTR lpCnn = NULL)
	{
		HRESULT			hres;

		if(NULL == lpCnn && FALSE != IsOpen(FALSE))
			return TRUE;

		if(NULL == lpCnn)
		{
			SetLastError("[TQuickADO::Open] Is not has connection string.");
			return FALSE;
		}
		try
		{
			if(NULL != m_spConn)
				m_spConn.Release();

			CoInitialize(NULL);
			hres = m_spConn.CreateInstance(__uuidof(Connection));
			if(FAILED(hres))
			{
				ATLASSERT(SUCCEEDED(hres));
				return FALSE;
			}
			hres = m_spConn->Open(lpCnn, "", "", 0);
		}
		catch(_com_error& e)
		{
			SetLastError("[TQuickADO::Open] error: [0x%08X]%s", e.Error(), (LPCSTR)e.Description());
			return FALSE;
		}

		return SUCCEEDED(hres);
	}
	// 执行
	_RecordsetPtr Execute(LPCWSTR lpSQL)
	{
		if(FALSE == IsOpen())
		{
			SetLastError("[TQuickADO::Execute] Not connect to server.");
			return NULL;
		}
		try
		{
			VARIANT			var;
			_RecordsetPtr	spRes;
			
			VariantInit(&var);
			spRes = m_spConn->Execute(lpSQL, &var, 0);
			m_nLastAffect = var.lVal;
			m_nTryCnn = 0;

			return spRes;
		}
		catch(_com_error& e)
		{
			m_nTryCnn++;
			SetLastError("[TQuickADO::Execute] execute '%S' error: [0x%08X]%s", lpSQL, e.Error(), (LPCSTR)e.Description());
		}

		return NULL;
	}
	// 静态执行
	BOOL ExecuteStatic(LPCWSTR lpSQL, _RecordsetPtr& spRes)
	{
		HRESULT			hres;
		VARIANT			vt;

		if(FALSE == IsOpen())
		{
			SetLastError("[TQuickADO::ExecuteStatic] Not connect to server.");
			return FALSE;
		}
		try
		{
			if(NULL != spRes)
				spRes.Release();
			
			hres = spRes.CreateInstance(__uuidof(Recordset));
			if(NULL == spRes)
			{
				SetLastError("[TQuickADO::ExecuteStatic] Can not create Recordset '%S' Interface: %08X", lpSQL, hres);
				return FALSE;
			}
			
			spRes->put_CursorLocation(adUseClient);
			vt.vt = VT_DISPATCH;
			vt.pdispVal = m_spConn;
			spRes->Open(lpSQL
				, vt
				, adOpenStatic
				, adLockReadOnly
				, adCmdUnknown);

			m_nTryCnn = 0;
			if(spRes->RecordCount < 1)
			{
				spRes.Release();
				return FALSE;	// 没有数据
			}
		}
		catch(_com_error& e)
		{
			m_nTryCnn++;
			SetLastError("[TQuickADO::ExecuteStatic] Execute failed: [0x%08X]%s", e.Error(), (LPCSTR)e.Description());
			return FALSE;
		}

		return TRUE;
	}
	// 得到最后插入的标识符
	LONG GetIdentity()
	{
		LONG		nRet		= -1;

		if(m_spConn == NULL || m_spConn->GetState() != adStateOpen)
		{
			SetLastError("[TQuickADO::GetIdentity] Not connect to server.");
			return -1;
		}
		try{
			_RecordsetPtr		spRes;

			spRes = m_spConn->Execute(L"SELECT SCOPE_IDENTITY()", 0, 0);
			if(spRes != NULL)
			{
				if(VARIANT_FALSE == spRes->adoEOF)
					nRet = GetRecordLong(spRes, 0L);
				spRes->Close();
			}
			m_nTryCnn = 0;
		}
		catch(_com_error&)
		{
			m_nTryCnn++;
		}

		return nRet;
	}

	// 得到最后一次影响行数
	LONG GetLastAffect()
	{
		return m_nLastAffect;
	}
};






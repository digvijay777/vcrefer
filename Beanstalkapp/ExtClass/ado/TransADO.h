#pragma once
#include "QuickADO.h"

template <class T, class SetError = AdoSetLastError>
class TTransADO : public TQuickADO<T, SetError>
{
public:
	TTransADO(LPCWSTR lpCnn)
	{
		m_bTrans = FALSE;
		BeginTrans(lpCnn);
	}
	TTransADO()
	{
		m_bTrans = FALSE;
	}
	~TTransADO()
	{
		Rollback();

		if(m_spConn != NULL)
		{
			if(m_spConn->GetState() == adStateOpen)
				m_spConn->Close();
			m_spConn.Release();
			m_spConn = NULL;
		}
	}
protected:
	BOOL	m_bTrans;
public:
	BOOL IsTrans()
	{
		return m_bTrans;
	}
	HRESULT BeginTrans(LPCWSTR lpCnn)
	{
		HRESULT		hres		= E_FAIL;

		m_bTrans = TRUE;	// 开启了事务
		if(FALSE == TQuickADO<T, SetError>::Open(lpCnn))
			return hres;

		try
		{
			hres = m_spConn->BeginTrans();
		}
		catch(_com_error& e)
		{
			m_spConn.Release();
			SetLastError("[TTransADO] BeginTrans failed: [0x%08X]%s"
				, e.Error(), (LPCSTR)e.Description());
		}

		return hres;
	}

	HRESULT Commit()
	{
		if(m_spConn == NULL || m_spConn->GetState() != adStateOpen)
			return E_FAIL;

		HRESULT		hres		= E_FAIL;

		try	
		{
			hres = m_spConn->CommitTrans();
		}
		catch(_com_error& e)
		{
			SetLastError("[TTransADO::Commit] failed: [0x%08X]%s", e.Error(), (LPCSTR)e.Description());
		}
		m_spConn->Close();
		m_spConn.Release();
		m_spConn = NULL;

		return hres;
	}

	HRESULT Rollback()
	{
		if(m_spConn == NULL || m_spConn->GetState() != adStateOpen)
			return E_FAIL;

		HRESULT		hres		= E_FAIL;
		try
		{
			hres = m_spConn->RollbackTrans();
		}
		catch(_com_error& e)
		{
			SetLastError("[TTransADO::Rollback] failed: [0x%08X]%s", e.Error(), (LPCSTR)e.Description());
		}

		m_spConn->Close();
		m_spConn.Release();
		m_spConn = NULL;

		return hres;
	}

	_RecordsetPtr Execute(LPCWSTR lpSQL, BOOL bErrRollback = TRUE)
	{
		if(FALSE == IsOpen(FALSE))
		{
			SetLastError("[TTransADO::Execute] Not connect to server.");
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
			SetLastError("[TTransADO::Execute] execute '%S' error: [0x%08X]%s", lpSQL, e.Error(), (LPCSTR)e.Description());
		}
		
		if(bErrRollback)
			Rollback();
		return NULL;
	}
};

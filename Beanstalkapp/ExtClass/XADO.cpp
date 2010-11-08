#include ".\xado.h"
#include <atltrace.h>

BOOL XAdo::SplitSQL(std::vector<std::string>& vSQL, LPCSTR lpData, LPCSTR database, LPCSTR user, LPCSTR passwd)
{
	std::string			strLine;
	LPCSTR				DATABASE		= "#database#";
	LPCSTR				USER			= "#user#";
	LPCSTR				PASSWD			= "#passwd#";

	strLine.clear();
	for(int i = 0; lpData[i]; i++)
	{
		if(strLine == "" && NULL != strchr("\r\n\t ", lpData[i]))
			continue;

		if('-' == lpData[i])
		{
			// 去除注释
			if(strnicmp(&lpData[i], "--", 2) == 0)
			{
				for(; 0 != lpData[i] && '\n' != lpData[i]; i++);
				if(0 == lpData[i]) break;
			}
		}
		else if('/' == lpData[i])
		{
			// 去除注释
			if(strnicmp(&lpData[i], "/*", 2) == 0)
			{
				for(; 0 != lpData[i] && strnicmp(&lpData[i], "*/", 2) != 0; i++);
				if(0 == lpData[i]) break;
				if(strnicmp(&lpData[i], "*/", 2) == 0) i += 2;
			}
		}

		if('\n' == lpData[i])
		{
			strLine.push_back(lpData[i]);
			// 完成一行
			if(strnicmp(&lpData[i], "\ngo", 3) == 0)
			{
				// 完成一个命令
				if(strLine != "")
				{
					vSQL.push_back(strLine);
					strLine.clear();
				}
				i += 2;
			}
		}
		else if('#' != lpData[i])
		{
			// 非替换文本
			strLine.push_back(lpData[i]);
			continue;
		}
		else if(strnicmp(&lpData[i], DATABASE, strlen(DATABASE)) == 0)
		{
			// 替换DATABASE
			strLine.insert(strLine.end(), &database[0], &database[strlen(database)]);
			i += strlen(DATABASE) - 1;
		}
		else if(strnicmp(&lpData[i], USER, strlen(USER)) == 0)
		{
			// 替换用户
			strLine.insert(strLine.end(), &user[0], &user[strlen(user)]);
			i += strlen(USER) - 1;
		}
		else if(strnicmp(&lpData[i], PASSWD, strlen(PASSWD)) == 0)
		{
			// 替换用户名
			strLine.insert(strLine.end(), &passwd[0], &passwd[strlen(passwd)]);
			i += strlen(PASSWD) - 1;
		}
		else
		{
			assert(0);
		}
	}

	if(strLine != "")
		vSQL.push_back(strLine);

	return TRUE;
}

BOOL XAdo::ExecuteSQL(_ConnectionPtr& spCon, std::vector<std::string>& vSQL)
{
	try
	{
		for(int i = 0; i < (int)vSQL.size(); i++)
		{
			spCon->Execute(vSQL[i].c_str(), NULL, adExecuteNoRecords | adCmdText);
		}
	}
	catch(_com_error& e)
	{
		SetLastError(e.Error());
		ATLTRACE("ExecuteSQL exception: %s", (LPCSTR)e.Description());
		return FALSE;
	}

	return TRUE;
}

// 参数格式: name(S)|type(N)|Direction(N)|size(N)|value(X)
BOOL XAdo::ExecuteProc(_ConnectionPtr& spCon, LPCSTR proc, int nParam, ...)
{
	_CommandPtr			spCmd;
	// 创建接口
	spCmd.CreateInstance(__uuidof(Command));
	spCmd->ActiveConnection = spCon;
	spCmd->CommandText = proc;
	spCmd->CommandType = adCmdStoredProc;
	// 创建参数
	try
	{
		va_list			vaArr;

		va_start(vaArr, nParam);
		for(int i = 0; i < nParam; i++)
		{
			XAdo::ProcParam		pParam		= va_arg(vaArr, XAdo::ProcParam);
			_ParameterPtr		spParam		= spCmd->CreateParameter(pParam.name, pParam.type, pParam.direction, pParam.size, pParam.value);

			spCmd->Parameters->Append(spParam);
		}
		va_end(vaArr);
		// 执行过程
		spCmd->Execute(NULL, NULL, adExecuteNoRecords);
	}
	catch(_com_error& e)
	{
		SetLastError(e.Error());
		ATLTRACE("ExecuteProc exception: %s", (LPCSTR)e.Description());
		return FALSE;
	}

	return TRUE;
}
BOOL XAdo::CreateDatabase(_ConnectionPtr& spCon, LPCSTR database)
{
	CHAR			szSQL[500]		= {0};

	try
	{
		// 创建数据库
		_snprintf(szSQL, sizeof(szSQL), "create database %s", database);
		spCon->Execute(szSQL, NULL, adExecuteNoRecords);
	}
	catch(_com_error& e)
	{
		SetLastError(e.Error());
		ATLTRACE("CreateDatabase exception: %s", (LPCSTR)e.Description());
		return FALSE;
	}

	return TRUE;
}

BOOL XAdo::DeleteDatabase(_ConnectionPtr& spCon, LPCSTR database)
{
	CHAR			szSQL[500]		= {0};

	try
	{
		// 创建数据库
		_snprintf(szSQL, sizeof(szSQL), "ALTER DATABASE %s SET SINGLE_USER with ROLLBACK IMMEDIATE", database);
		spCon->Execute(szSQL, NULL, adExecuteNoRecords);
		spCon->Execute("use master", NULL, adExecuteNoRecords);
		_snprintf(szSQL, sizeof(szSQL), "drop database %s", database);
		spCon->Execute(szSQL, NULL, adExecuteNoRecords);
	}
	catch(_com_error& e)
	{
		SetLastError(e.Error());
		ATLTRACE("DeleteDatabase exception: %s", (LPCSTR)e.Description());
		return FALSE;
	}

	return TRUE;
}

BOOL XAdo::CreateDatabase(_ConnectionPtr& spCon, LPCSTR database, LPCSTR user, LPCSTR passwd, LPCSTR lpTableSQL)
{
	std::vector<std::string>		vSQL;
	BYTE*							pData			= NULL;
	BOOL							bRes			= FALSE;

	// 验证操作
	if(spCon == NULL || spCon->GetState() != adStateOpen)
	{
		SetLastError(ERROR_BAD_ARGUMENTS);
		return FALSE;
	}
	// 分解SQL语句
	bRes = SplitSQL(vSQL, lpTableSQL, database, user, passwd);
	if(FALSE == bRes)
	{
		SetLastError(ERROR_BAD_FORMAT);
		return FALSE;
	}
	// 执行sql语句
	_bstr_t				bstrCnn;
	_ConnectionPtr		spTranCnn;
	HRESULT				hres			= E_FAIL;
	CHAR				szSQL[1024]		= {0};
	BOOL				bCanComit		= TRUE;

	// 创建数据库
	if(FALSE == CreateDatabase(spCon, database))
		return FALSE;

	bstrCnn = spCon->GetConnectionString();
	spTranCnn.CreateInstance(__uuidof(Connection));
	try
	{
		// 连接
		spTranCnn->Open(bstrCnn, "", "", 0);
		_snprintf(szSQL, sizeof(szSQL), "use %s", database);
		spTranCnn->Execute(szSQL, NULL, adExecuteNoRecords);

		// 创建登录用户
		ExecuteProc(spTranCnn, "sp_droplogin", 1
			, XAdo::ProcParam("loginame", adVarChar, adParamInput, strlen(user), user));

		if(FALSE == ExecuteProc(spTranCnn, "sp_addlogin", 3
			, XAdo::ProcParam("loginame", adVarChar, adParamInput, strlen(user), user)
			, XAdo::ProcParam("passwd", adVarChar, adParamInput, strlen(passwd), passwd)
			, XAdo::ProcParam("defdb", adVarChar, adParamInput, strlen(database), database)) )
			bCanComit = FALSE;
		if(FALSE == ExecuteProc(spTranCnn, "sp_adduser", 3
			, XAdo::ProcParam("loginame", adVarChar, adParamInput, strlen(user), user)
			, XAdo::ProcParam("name_in_db", adVarChar, adParamInput, strlen(user), user)
			, XAdo::ProcParam("grpname", adVarChar, adParamInput, strlen("db_datawriter"), "db_datawriter")) )
			bCanComit = FALSE;
		if(FALSE == ExecuteProc(spTranCnn, "sp_addrolemember", 2
			, XAdo::ProcParam("rolename", adVarChar, adParamInput, strlen("db_datareader"), "db_datareader")
			, XAdo::ProcParam("membername", adVarChar, adParamInput, strlen(user), user)) )
			bCanComit = FALSE;		// 开启事务
		spTranCnn->BeginTrans();
		// 执行操作
		if(FALSE != bCanComit && FALSE != ExecuteSQL(spTranCnn, vSQL))
		{
			hres = spTranCnn->CommitTrans();
		}
		else
		{
			spTranCnn->RollbackTrans();
			DeleteDatabase(spCon, database);
			ExecuteProc(spTranCnn, "sp_droplogin", 1
				, XAdo::ProcParam("loginame", adVarChar, adParamInput, strlen(user), user));
		}		
	}
	catch(_com_error& e)
	{
		ATLTRACE("CreateDatabase exception: %s", (LPCSTR)e.Description());
		SetLastError(e.Error());
	}

	return SUCCEEDED(hres);
}

BOOL XAdo::IsDatabase(_ConnectionPtr& spCon, LPCSTR database)
{
	CHAR		szSQL[512]		= {0};

	_snprintf(szSQL, sizeof(szSQL)
		, "select 1 from master..sysdatabases where name='%s'"
		, database);
	try
	{
		_RecordsetPtr		spRes;

		spRes = spCon->Execute(szSQL, NULL, 0);
		if(NULL == spRes || VARIANT_FALSE != spRes->adoEOF)
			return FALSE;
	}
	catch (...)
	{
		return FALSE;
	}

	return TRUE;
}

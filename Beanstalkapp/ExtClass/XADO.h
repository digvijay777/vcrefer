#pragma once
#include <string>
#include <vector>

#import "C:\Program Files\Common Files\System\ADO\msado15.dll" \
	no_namespace rename("EOF", "adoEOF") 

namespace XAdo
{
	BOOL	SplitSQL(std::vector<std::string>& vSQL, LPCSTR lpData, LPCSTR database, LPCSTR user, LPCSTR passwd);
	BOOL	ExecuteSQL(_ConnectionPtr& spCon, std::vector<std::string>& vSQL);
	BOOL	ExecuteProc(_ConnectionPtr& spCon, LPCSTR proc, int nParam, ...);
	BOOL	DeleteDatabase(_ConnectionPtr& spCon, LPCSTR database);
	BOOL	CreateDatabase(_ConnectionPtr& spCon, LPCSTR database);
	BOOL	CreateDatabase(_ConnectionPtr& spCon, LPCSTR database, LPCSTR user, LPCSTR passwd, LPCSTR lpTableSQL);
	BOOL	IsDatabase(_ConnectionPtr& spCon, LPCSTR database);

	// 存储过程参数
	class ProcParam
	{
	public:
		_bstr_t						name;
		DataTypeEnum				type;
		ParameterDirectionEnum		direction;
		LONG						size;
		_variant_t					value;

	public:
		ProcParam(_bstr_t n, DataTypeEnum t, ParameterDirectionEnum d, LONG s, _variant_t v)
			: name(n)
			, type(t)
			, direction(d)
			, size(s)
			, value(v)
		{
		}
	};
}

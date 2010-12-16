#include "StdAfx.h"
#include ".\sendonemail.h"
#include "../../../ExtClass/XStr.h"

CSendOneMail::CSendOneMail(void)
{
	memset(m_szLastError, 0, sizeof(m_szLastError));
	m_pTemData = NULL;
	m_nTemSize = 0;
}

CSendOneMail::~CSendOneMail(void)
{
}

void CSendOneMail::SetLastError(const char* pError, ...)
{
	va_list		vaArr;

	va_start(vaArr, pError);
	_vsnprintf(m_szLastError, sizeof(m_szLastError), pError, vaArr);
	va_end(vaArr);
}

const char* CSendOneMail::GetLastError()
{
	return m_szLastError;
}

// 设置替换变量
void CSendOneMail::SetVariant(const char* pKey, const char* pValue)
{
	m_mapVariant[pKey] = pValue;
}
void CSendOneMail::SetVariant(std::map<std::string, std::string>& mapVar)
{
	m_mapVariant.clear();
	m_mapVariant.insert(mapVar.begin(), mapVar.end());
}
// 获取替换变量
const char* CSendOneMail::GetVariant(const char* pKey)
{
	std::map<std::string, std::string>::iterator	iValue;

	iValue = m_mapVariant.find(pKey);
	if(iValue == m_mapVariant.end())
		return NULL;

	return iValue->second.c_str();
}

void CSendOneMail::SetMailServer(const char* pServer, const char* pUser, const char* pPasswd)
{
	m_MailServer = pServer;
	m_MailUser = pUser;
	m_MailPasswd = pPasswd;
}

void CSendOneMail::SetMailFrom(const char* pEmail, const char* pName)
{
	m_MailFrom = pEmail;
	m_MailFromName = pName;
}

void CSendOneMail::SetMailTo(const char* pEmails, const char* pName, const char* pTitle)
{
	m_MailToList.clear();
	SplitEmail(pEmails, m_MailToList);
	m_MailToName = pName;
	m_MailToTitle = pTitle;
}

void CSendOneMail::SetMailCC(const char* pEmails)
{
	SplitEmail(pEmails, m_MailToList);
}

void CSendOneMail::SetDirectory(const char* pDir)
{
	DWORD	dwAttrib			= INVALID_FILE_ATTRIBUTES;
	char	szModFile[MAX_PATH]	= {0};
	char*	pName				= NULL;

	GetFullPathNameA(pDir, sizeof(szModFile), szModFile, &pName);
	dwAttrib = GetFileAttributesA(szModFile);
	if(INVALID_FILE_ATTRIBUTES == dwAttrib)
	{
		GetModuleFileNameA(NULL, szModFile, sizeof(szModFile));
	}
	else if(FILE_ATTRIBUTE_DIRECTORY & dwAttrib)
	{
		m_Directory = pDir;
		return;
	}

	// 分解文件目录
	pName = strrchr(szModFile, '\\');
	if(NULL == pName)
		pName = strrchr(szModFile, '/');
	if(NULL == pName)
		return;
	*pName = 0;
	m_Directory = szModFile;
}

void CSendOneMail::SetAttachment(std::vector<std::string>& vctAttach)
{
	m_Attachment.clear();
	m_Attachment.insert(m_Attachment.begin(), vctAttach.begin(), vctAttach.end());
}

void CSendOneMail::SplitEmail(LPCSTR lpstr, std::vector<std::string>& vct)
{
	std::string			strline;
	CHAR*				pStr		= (CHAR *)lpstr;

	for(; *pStr; pStr++)
	{
		if(';' == *pStr)
		{
			if("" != strline)
			{
				char*		pA		= strchr(strline.c_str(), '@');

				if(NULL != pA 
					&& pA == strrchr(strline.c_str(), '@') 
					&& strlen(pA + 1) > 0)
				{
					vct.push_back(strline.c_str());
				}
			}
			strline.clear();
			continue;
		}
		strline.push_back(*pStr);
	}

	if("" != strline)
	{
		char*		pA		= strchr(strline.c_str(), '@');

		if(NULL != pA 
			&& pA == strrchr(strline.c_str(), '@') 
			&& strlen(pA + 1) > 0)
		{
			vct.push_back(strline.c_str());
		}
	}
}

void CSendOneMail::SetTemplate(const unsigned char* pData, long nSize)
{
	m_pTemData = pData;
	m_nTemSize = nSize;
}

// 发送邮件
BOOL CSendOneMail::Send(const unsigned char* pData /* = NULL */, long nSize /* = 0 */)
{
	if(NULL != pData)
		SetTemplate(pData, nSize);
	if(NULL == m_pTemData || 0 >= m_nTemSize || IsBadReadPtr(m_pTemData, m_nTemSize))
	{
		SetLastError("错误的模板数据");
		return FALSE;
	}
	// 创建JMAIL指针
	if(FALSE == CreateJMailPtr())
	{
		return FALSE;
	}
	// 开始发送
	try
	{
		std::string			strSend;

		if(FALSE == CreateSendData(strSend))
			return FALSE;

		if(0 == m_MailToList.size())
		{
			SetLastError("[CSendOneMail::Send] 没有要发送地址");
			return FALSE;
		}

		m_spMail->Charset = "utf-8";
		m_spMail->From = m_MailFrom.c_str();
		m_spMail->FromName = m_MailFromName.c_str();
		m_spMail->AddRecipient(m_MailToList[0].c_str(), m_MailToName.c_str(), "");
		for(int i = 1; i < (int)m_MailToList.size(); i++)
		{
			m_spMail->AddRecipientBCC(m_MailToList[i].c_str(), "");
		}
		m_spMail->Priority = 1;	// 立即发送
		m_spMail->Subject = m_MailToTitle.c_str();
		m_spMail->HTMLBody = strSend.c_str();

		m_spMail->MailServerUserName = m_MailUser.c_str();
		m_spMail->MailServerPassWord = m_MailPasswd.c_str();

		m_spMail->Send(m_MailServer.c_str(), VARIANT_FALSE);

		m_spMail->Close();
		m_spMail.Release();
	}
	catch(_com_error& e)
	{
		SetLastError("[CSendOneMail::Send] error: [0x%X]%S"
			, e.Error(), (const wchar_t*)e.Description());
		return FALSE;
	}

	return TRUE;
}

BOOL CSendOneMail::CreateJMailPtr()
{
	try
	{
		if(NULL != m_spMail)
			m_spMail.Release();

		HRESULT			hres		= E_FAIL;

		CoInitialize(NULL);
		hres = m_spMail.CreateInstance(L"JMail.Message");
		if(FAILED(hres))
		{
			// 注册组件
			char		szJMail[MAX_PATH]		= {0};
			char*		pName					= NULL;

			GetModuleFileNameA(NULL, szJMail, sizeof(szJMail));
			pName = strrchr(szJMail, '\\');
			if(NULL == pName)
				pName = strrchr(szJMail, '/');
			if(NULL == pName)
			{
				SetLastError("[CSendOneMail::CreateJMailPtr] 错误的模块名称: %s", szJMail);
				return FALSE;
			}
			strcpy(++pName, "jmail.dll");
			// 注册组件
			HINSTANCE	hJMail		= LoadLibraryA(szJMail);
			if(NULL == hJMail)
			{
				SetLastError("[CSendOneMail::CreateJMailPtr] 找不到组件文件: %s", szJMail);
				return FALSE;
			}
			typedef HRESULT	(*DllRegisterServerFn)();
			DllRegisterServerFn DllRegisterServer = (DllRegisterServerFn)GetProcAddress(hJMail, "DllRegisterServer");
			if(NULL != DllRegisterServer)
				hres = DllRegisterServer();
			FreeLibrary(hJMail);
			if(FAILED(hres))
			{
				SetLastError("[CSendOneMail::CreateJMailPtr] 注册组件失败: 0x%X", hres);
				return FALSE;
			}
			// 再一次创建
			hres = m_spMail.CreateInstance(L"JMail.Message");
		}

		return SUCCEEDED(hres);
	}
	catch(_com_error& e)
	{
		SetLastError("[CSendOneMail::CreateJMailPtr] error: [0x%X]%S"
			, e.Error(), (const wchar_t*)e.Description());
	}

	return FALSE;
}

// 创建发送数据
BOOL CSendOneMail::CreateSendData(std::string& strData)
{
	char		szData[MAX_PATH]		= {0};

	GetCurrentDirectoryA(sizeof(szData), szData);
	SetCurrentDirectoryA(m_Directory.c_str());
	// 展开变量
	ExplainVariant(strData);
	// 添加附件
	AddAttachment();

	SetCurrentDirectoryA(szData);
	return TRUE;
}

// 添加附件
void CSendOneMail::AddAttachment()
{
	try
	{
		for(int i = 0; i < (int)m_Attachment.size(); i++)
		{
			m_spMail->AddAttachment(m_Attachment[i].c_str(), VARIANT_TRUE, "application/octet-stream");
		}
	}
	catch(_com_error& e)
	{
		SetLastError("[CSendOneMail::AddAttachment] failed: [0x%X]%S"
			, e.Error(), (LPCWSTR)e.Description());
	}
}

// 展开变量
void CSendOneMail::ExplainVariant(std::string& strData)
{
	for(int nIndex = 0; m_pTemData[nIndex] != 0 && nIndex < m_nTemSize; nIndex++)
	{
		if('{' == m_pTemData[nIndex])
		{
			// 变理处理
			CHAR		szVarName[MAX_PATH]		= {0};
			int			m						= nIndex+1;

			for(int j = 0; m < m_nTemSize && j < (sizeof(szVarName)-1); j++, m++)
			{
				if('}' == m_pTemData[m])
					break;
				else if('{' == m_pTemData[m])
					break;
				else if(0 == m_pTemData[m])
					break;
				
				szVarName[j] = m_pTemData[m];
			}

			if('}' == m_pTemData[m])
			{
				nIndex = m;
				// 展开变量
				if(FALSE != ExpandVariant(szVarName, strData))
				{
					continue;
				}
				// 未处理变量
				strData.push_back('{');
				strData.append(szVarName);
				strData.push_back('}');
				continue;
			}
		}
		else if('<' == m_pTemData[nIndex] && '/' != m_pTemData[nIndex+1])
		{
			// 处理标记头
			CHAR		szTag[1024]			= {0};
			CHAR*		pTag				= szTag;
			int			n					= nIndex+1;

			for(int j = 0; n < m_nTemSize && j < (sizeof(szTag)-1); j++, n++)
			{
				if('>' == m_pTemData[n])
					break;
				else if('<' == m_pTemData[n])
					break;

				szTag[j] = m_pTemData[n];
			}

			if('>' == m_pTemData[n])
			{
				nIndex = n;
				// 替换标记
				if(FALSE != ReplaceTagAttribute(szTag, strData, '/'==m_pTemData[n-1]))
				{
					continue;
				}
				// 未处理标签
				strData.push_back('<');
				strData.append(szTag);
				strData.push_back('>');
				continue;
			}
		}

		strData.push_back(m_pTemData[nIndex]);
	}
}

// 展开变量
BOOL CSendOneMail::ExpandVariant(const char* pKey, std::string& strData)
{
	std::map<std::string, std::string>::iterator		iVal;

	iVal = m_mapVariant.find(pKey);
	if(iVal != m_mapVariant.end())
	{
		strData.append(iVal->second.c_str());
		return TRUE;
	}
	return FALSE;
}

// 替换变量
BOOL CSendOneMail::ReplaceTagAttribute(const char* pTag, std::string& strData, bool bClose /* = false */)
{
	if('i' != *pTag && 'm' != *pTag && 'I' != *pTag && 'M' != *pTag)
		return FALSE;

	int			nDel			= 0;
	const char* pHtmlTag		= pTag;
	CHAR		szTag[1024]		= {0};
	char*		pNewTag			= NULL;

	if(strncmp(pTag, "img ", 3) == 0)
	{
		nDel = 1;		
		pHtmlTag += 4;
		strcpy(szTag, "img");
		pNewTag = &szTag[3];
	}
	else if('m' == *pTag && strncmp(pTag, "meta ", 5) == 0)
	{
		nDel = 2;
		pHtmlTag += 5;
		strcpy(szTag, "meta");
		pNewTag = &szTag[4];
	}
	if(0 == nDel)
		return FALSE;	// 不是要处理的标签 
	
	if(2 == nDel)
	{
		if(FALSE == DoTagMeta(pHtmlTag, pNewTag, sizeof(szTag)-10))
			return FALSE;
	}
	else if(1 == nDel)
	{
		char	szOldDir[512]		= {0};

		GetCurrentDirectoryA(sizeof(szOldDir), szOldDir);
		if(FALSE == DoTagImg(pHtmlTag, pNewTag, sizeof(szTag)-10))
		{
			SetCurrentDirectoryA(szOldDir);
			return FALSE;
		}
		SetCurrentDirectoryA(szOldDir);
	}
	// 处理标签完成
	strData.push_back('<');
	strData.append(szTag);
	if(false != bClose)
		strData.push_back('/');
	strData.push_back('>');

	return TRUE;
}

// 处理meta标签
BOOL CSendOneMail::DoTagMeta(const char* pTag, char* pNew, int nSize)
{
	CHtmlLiteAttrib			attr;

	attr.Parse(pTag);
	if(stricmp("Content-Type", attr.Get("http-equiv")) != 0)
		return FALSE;

	attr.Set("content", "text/html; charset=utf-8");
	attr.Dump(pNew, nSize);
	
	return TRUE;
}

// 处理图片
BOOL CSendOneMail::DoTagImg(const char* pTag, char* pNew, int nSize)
{
	CHtmlLiteAttrib			attr;
	CHAR					szImg[512]		= {0};
	char*					pFile			= NULL;
	CHAR					szDir[512]		= {0};
	std::map<std::string, std::string>::iterator	iValue;

	attr.Parse(pTag);
	GetFullPathNameA(attr.Get("src"), sizeof(szImg), szImg, &pFile);
	if(NULL == pFile)
		return FALSE;
	strncpy(szDir, szImg, (int)(pFile-szImg));
	strlwr(szImg);
	iValue = m_mapImg.find(szImg);
	if(iValue == m_mapImg.end())
	{
		SetCurrentDirectoryA(szDir);
		try
		{
			_bstr_t		bstrCID;
			char*		pImgPath		= pFile;
			char*		pImgType		= strrchr(pFile, '.');
			char		szIMMS[128]		= {0};

			// 检查图片类型
			pImgType++;
			if(IsBadStringPtr(pImgType, 5))
			{
				return FALSE;
			}
			if(stricmp(pImgType, "jpg") == 0)
				strcpy(szIMMS, "image/jpeg");
			else 
			{
				strcpy(szIMMS, "image/");
				strcat(szIMMS, strlwr(pImgType));
			}
			try
			{
				bstrCID = m_spMail->AddAttachment(pImgPath, VARIANT_TRUE, szIMMS);
			}
			catch(_com_error& )
			{
				return FALSE;
			}
			attr.Set("src", "cid:" + bstrCID);
			m_mapImg[szImg] = "cid:" + bstrCID;
		}
		catch(_com_error& e)
		{
			return FALSE;
		}
	}
	else
	{
		attr.Set("src", iValue->second.c_str());
	}


	attr.Dump(pNew, nSize);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 轻量级属性
CHtmlLiteAttrib::CHtmlLiteAttrib()
{

}
CHtmlLiteAttrib::~CHtmlLiteAttrib()
{

}

void CHtmlLiteAttrib::Parse(const char* pStr)
{
	const char*			pKey1			= NULL;
	const char*			pKey2			= NULL;
	const char*			pValue1			= NULL;
	const char*			pValue2			= NULL;
	const char*			pSrc			= pStr+1;
	
	if(NULL == pStr || 0 == *pStr)
		return;

	for(; *pSrc; pSrc++)
	{
		if('=' == *pSrc)
		{
			pKey2 = pSrc;
			pValue1 = pSrc+1;

			while(('\x20' == *(pKey2-1) || '\t' == *(pKey2-1)) && pKey2 != pStr)
				pKey2--;
			while(*pValue1 && ('\x20' == *pValue1 || 't' == *pValue1))
				pValue1++;

			pKey1 = pKey2;
			pValue2 = pValue1;

			while( pStr != pKey1 && '\x20' != *(pKey1-1) && '\t' != *(pKey1-1) )
				pKey1--;

			if('\"' == *pValue2)
			{
				pValue1++;
				pValue2++;
				while(*pValue2 && ('\"' != *pValue2) )
					pValue2++;
			}
			else
			{
				while(*pValue2 && '\x20' != *pValue2 && '\t' != *pValue2)
					pValue2++;
			}

			// 添加处理的值
			CHAR				szKey[64]		= {0};
			CHAR				szValue[256]	= {0};

			strncpy( szKey, pKey1, min((int)(pKey2-pKey1), (int)sizeof(szKey)) );
			strncpy( szValue, pValue1, min((int)(pValue2-pValue1), (int)sizeof(szValue)) );
			strlwr(szKey);
			m_mapData[szKey] = szValue;
			pSrc = pValue2;
		}
	}
}

const char* CHtmlLiteAttrib::Get(const char* pKey)
{
	std::map<std::string, std::string>::iterator		iValue;

	iValue = m_mapData.find(pKey);
	if(iValue != m_mapData.end())
		return iValue->second.c_str();

	return "";
}

void CHtmlLiteAttrib::Set(const char* pKey, const char* pValue)
{
	CHAR		szKey[64]		= {0};
	CHAR		szValue[256]	= {0};

	strncpy(szKey, pKey, sizeof(szKey));
	strncpy(szValue, pValue, sizeof(szValue));
	strlwr(szKey);
	xtd::StrReplace(szValue, '\"', '\'');
	m_mapData[szKey] = szValue;
}

void CHtmlLiteAttrib::Dump(char* pStr, int nLen)
{
	std::map<std::string, std::string>::iterator		iValue;

	iValue = m_mapData.begin();
	*pStr = 0;
	for(int i = 0; iValue != m_mapData.end() && i < nLen; iValue++)
	{
		const char*		pKey		= iValue->first.c_str();
		const char*		pValue		= iValue->second.c_str();

		if(0 == *pValue)
			continue;	// 值为空时不添加
		if((strlen(pKey) + strlen(pValue) + 4 + i) > nLen)
			continue;
		i += strlen(pKey) + strlen(pValue) + 4;
		strcat(pStr, " ");
		strcat(pStr, pKey);
		strcat(pStr, "=\"");
		strcat(pStr, pValue);
		strcat(pStr, "\"");
	}
}

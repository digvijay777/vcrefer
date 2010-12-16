/*
 *	发送一封电子邮件
 */
#pragma once
#include <map>
#include <vector>
#include <string>
#import "jmail.dll"

class CSendOneMail
{
public:
	CSendOneMail(void);
	virtual ~CSendOneMail(void);
private:
	char									m_szLastError[1024];
	const unsigned char*					m_pTemData;
	long									m_nTemSize;
	std::map<std::string, std::string>		m_mapVariant;
	std::map<std::string, std::string>		m_mapImg;
	jmail::IMessagePtr						m_spMail;
	std::string								m_Charset;
	std::string								m_MailFrom;
	std::string								m_MailFromName;
	std::string								m_MailUser;
	std::string								m_MailPasswd;
	std::string								m_MailServer;
	std::vector<std::string>				m_MailToList;
	std::string								m_MailToName;
	std::string								m_MailToTitle;
	std::string								m_Directory;
	std::vector<std::string>				m_Attachment;

protected:
	BOOL			CreateJMailPtr();
	BOOL			CreateSendData(std::string& strData);
	void			AddAttachment();
	void			ExplainVariant(std::string& strData);
	BOOL			ExpandVariant(const char* pKey, std::string& strData);
	BOOL			ReplaceTagAttribute(const char* pTag, std::string& strData, bool bClose = false);
	BOOL			DoTagMeta(const char* pTag, char* pNew, int nSize);
	BOOL			DoTagImg(const char* pTag, char* pNew, int nSize);

public:
	void			SetLastError(const char* pError, ...);
	const char*		GetLastError();

	void			SetVariant(const char* pKey, const char* pValue);
	const char*		GetVariant(const char* pKey);
	void			SetVariant(std::map<std::string, std::string>& mapVar);
	void			SetMailServer(const char* pServer, const char* pUser, const char* pPasswd);
	void			SetMailFrom(const char* pEmail, const char* pName);
	void			SetMailTo(const char* pEmails, const char* pName, const char* pTitle);
	void			SetMailCC(const char* pEmails);
	void			SplitEmail(LPCSTR lpstr, std::vector<std::string>& vct);
	void			SetTemplate(const unsigned char* pData, long nSize);
	void			SetDirectory(const char* pDir);
	void			SetAttachment(std::vector<std::string>& vctAttach);

	BOOL			Send(const unsigned char* pData = NULL, long nSize = 0);
};

class CHtmlLiteAttrib
{
public:
	CHtmlLiteAttrib();
	~CHtmlLiteAttrib();

private:
	std::map<std::string, std::string>		m_mapData;

public:
	void			Parse(const char* pStr);
	const char*		Get(const char* pKey);
	void			Set(const char* pKey, const char* pValue);
	void			Dump(char* pStr, int nLen);
};


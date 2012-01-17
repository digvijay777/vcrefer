#pragma once

/*Ini操作函数集*/
unsigned long		GetConfigProfileString(const char* pAppName
											, const char* pKeyName
											, const char* pDefault, char* pReturnedString
											, unsigned long nSize, const char* pFileName); 
int					GetConfigProfileInt(const char* pAppName
										 , const char* pKeyName
										 , int nDef, const char* pFileName);
int					GetConfigBufferString(const char* pAppName, const char* pKeyName
										, char* pReturnedString, unsigned long nSize
										, const char* pBuff);
void				GetConfigBufferValue(const char* pAppName, const char* pKeyName
									   , char** pStrStart, char** pStrEnd
									   , const char* pBuff);
void				GetConfigBufferSection(const char* pAppName
										, char** pSecStart, char** pSecEnd
										, const char* pBuff);
int					WriteConfigProfileSection(const char* pAppName
										   , const char* pSection
										   , const char* pFileName);
int					WriteConfigProfileString(const char* pAppName
											 , const char* pKeyName
											 , const char* pValue
											 , const char* pFileName);


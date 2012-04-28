#pragma once

/*Ini操作函数集*/
unsigned long	ini_get_string(const char* papp, const char* pkey, 
							   const char* pdef, char* pret, 
							   unsigned long len, const char* pfile); 

int				ini_get_int(const char* papp, const char* pkey, 
							int def, const char* pfile);

int				buff_get_string(const char* papp, const char* pkey, 
								char* pret, unsigned long len, 
								const char* buff);

void			buff_get_value(const char* papp, const char* pkey, 
							   char** pstart, char** pend, 
							   const char* buff);

void			buff_get_section(const char* papp , char** pstart, 
								 char** pend , const char* buff);

int				ini_set_section(const char* papp, const char* psection, 
								const char* pfile);

int				ini_set_string(const char* papp, const char* pkey, 
							   const char* pvalue, const char* pfile);


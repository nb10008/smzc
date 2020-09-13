#ifndef _GET_INI_H_
#define _GET_INI_H_

int open_ini_file(const char *pFile);

int close_ini_file(void);

int get_ini_string(const char *s_section,const char *s_key, char *s_result);

int cfg_get_key(const char *pFile, const char *s_section, 
                const char *s_key, char *s_val);

int cfg_get_int(const char *pFile, const char *s_section, const char *s_key);

long cfg_get_long(const char *pFile, const char *s_section, const char *s_key);

double cfg_get_double(const char *pFile,const char *s_section,const char *s_key);

#endif
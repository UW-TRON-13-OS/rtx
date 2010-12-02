#ifndef _UTILS_H_
#define _UTILS_H_

#include "coldfire.h"

CHAR * rtx_spritf(CHAR * str, const CHAR * format, void * params[]);

CHAR * rtx_strcpy(CHAR * str, const CHAR * cpy_str);
int rtx_strcmp(const CHAR * str1,const CHAR * str2);

int rtx_strcmp(const CHAR * str1, const CHAR * str2);

CHAR * rtx_strtok(CHAR * str, const CHAR * delimiters);

#endif

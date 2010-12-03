#ifndef _UTILS_H_
#define _UTILS_H_

#include "rtx.h"
#include "msg_env_queue.h"

CHAR * rtx_sprintf(CHAR * str, const CHAR * format, void * params[]);

CHAR * rtx_strcpy(CHAR * str, const CHAR * cpy_str, int str_size);

int rtx_strcmp(const CHAR * str1, const CHAR * str2);

void rtx_strtok(const CHAR * str, CHAR * ret, const CHAR * delimiters);

int rtx_atoi (const CHAR * str, int *num_p);

int strLength(CHAR * str);

void strConvert(CHAR * str, const CHAR * con_str);

MsgEnv * print_ack(char * msg, MsgEnv* print_env, msg_env_queue_t *queue);

#endif

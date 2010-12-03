#ifndef _CCI_UTIL_H
#define _CCI_UTIL_H

#include "rtx.h"
#include "msg_env_queue.h"

void CCI_print(char * msg);

//gets and prints process statuses
int CCI_printProcessStatuses (char* raw_data, MsgEnv* send_env);

//prints trace buffers on console given the envelope message data
int CCI_printTraceBuffers (char* data, MsgEnv* send_env);

void CCI_displayWallClock (MsgEnv *send_env, int disp_b);
    
int CCI_setWallClock (MsgEnv *send_env, char* newTime);

#endif


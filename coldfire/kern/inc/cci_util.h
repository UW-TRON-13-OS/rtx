#ifndef _CCI_UTIL_H
#define _CCI_UTIL_H

#include "rtx.h"
#include "msg_env_queue.h"

extern msg_env_queue_t *msg_queue;
extern MsgEnv *print_env;
MsgEnv * CCI_print(char * msg);

//gets and prints process statuses
int CCI_printProcessStatuses (char* raw_data);

//prints trace buffers on console given the envelope message data
int CCI_printTraceBuffers (char* data);

void CCI_displayWallClock (MsgEnv *send_env, int disp_b);
    
int CCI_setWallClock (MsgEnv *send_env, char* newTime);

#endif


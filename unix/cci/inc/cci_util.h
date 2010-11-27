#ifndef _CCI_UTIL_H
#define _CCI_UTIL_H

#include "rtx.h"
#include "msg_env_queue.h"

//gets and prints process statuses
int CCI_printProcessStatuses (char* raw_data, MsgEnv* send_env,
                              msg_env_queue_t* msgQ);

//prints trace buffers on console given the envelope message data
int CCI_printTraceBuffers (char* data, MsgEnv* send_env, msg_env_queue_t* msgQ);

void CCI_displayWallClock (MsgEnv *send_env, msg_env_queue_t *msgQ, int disp_b);
    
int CCI_setWallClock (MsgEnv *send_env, msg_env_queue_t *msgQ, char* newTime);

#endif


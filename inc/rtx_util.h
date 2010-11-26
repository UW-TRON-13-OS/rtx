#ifndef RTX_UTIL_H
#define RTX_UTIL_H

#include "rtx.h"
#include "msg_env_queue.h"
#include <stdarg.h>

/** Adaption of printf for the CRT using variable arguments **/
int RTX_printf(MsgEnv* send_env, msg_env_queue_t* msgQ, const char* format,...);

#endif

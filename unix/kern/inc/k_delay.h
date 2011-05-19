#ifndef _KERN_DELAY_H_
#define _KERN_DELAY_H_

#include <rtx.h>

int k_request_delay(int time_delay, int wakeup_code, MsgEnv *msg_env);

#endif

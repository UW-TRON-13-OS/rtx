#ifndef _TIMEOUT_QUEUE_H_
#define _TIMEOUT_QUEUE_H_ 

#include <rtx.h>

int timeout_queue_is_empty();
void timeout_queue_insert (MsgEnv* new_msg_env);
MsgEnv * check_timeout_queue(MsgEnv* msg_env);
MsgEnv * get_timeout_queue();
MsgEnv * timeout_queue;

#endif

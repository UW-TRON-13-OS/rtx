#ifndef _MSG_ENV_QUEUE_H_
#define _MSG_ENV_QUEUE_H_

#include <rtx_ipc.h>

typedef struct msg_env_queue {
	MsgEnv* head;
	MsgEnv* tail;
} msg_env_queue_t;

msg_env_queue msg_env_queue_create();
MsgEnv* msg_env_queue_dequeue(msg_env_queue* queue);
int msg_env_queue_enqueue(msg_env_queue* queue, MsgEnv* env);

#endif
#ifndef _MSG_ENV_QUEUE_H_
#define _MSG_ENV_QUEUE_H_

#include <rtx.h>

typedef struct msg_env_queue msg_env_queue_t;

msg_env_queue_t * msg_env_queue_create();
void msg_env_queue_destroy(msg_env_queue_t * queue);
int msg_env_queue_is_empty(msg_env_queue_t* queue);
MsgEnv* msg_env_queue_dequeue(msg_env_queue_t* queue);
int msg_env_queue_enqueue(msg_env_queue_t* queue, MsgEnv* env);

#endif

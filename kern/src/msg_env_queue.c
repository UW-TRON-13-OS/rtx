#include "msg_env_queue.h"

#include <stdlib.h>

struct msg_env_queue {
	MsgEnv* head;
	MsgEnv* tail;
};

msg_env_queue_t * msg_env_queue_create()
{
    msg_env_queue_t * queue = malloc(sizeof(*queue));
    if (queue)
    {
        queue->head = queue-> tail = NULL;
    }
    return queue;
}

void msg_env_queue_destroy(msg_env_queue_t * queue)
{
    free(queue);
}

MsgEnv* msg_env_queue_dequeue(msg_env_queue_t *queue)
{
	MsgEnv* returnEnv = queue->head;
	if (queue->head == queue->tail)
	{
		queue->tail = NULL;
	}
	if (queue->head != NULL)
	{
		queue->head = queue->head->next;
	}
	
	returnEnv->next = NULL;
	return returnEnv;
}


int msg_env_queue_enqueue(msg_env_queue_t *queue, MsgEnv* env)
{
	if (env == NULL)
	{
		return ERROR_NULL_ARG;
	}
	
	queue->tail->next = env;
	env->next = NULL;
	queue->tail = env;

    return CODE_SUCCESS;
}

#include "msg_env_queue.h"
#include "k_globals.h"
#include "k_config.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

struct msg_env_queue {
	MsgEnv* head;
	MsgEnv* tail;
};

msg_env_queue_t * msg_env_queue_create()
{
    msg_env_queue_t * queue = malloc(sizeof(*queue));
    if (queue)
    {
        queue->head = NULL;
        queue-> tail = NULL;
    }
    return queue;
}

void msg_env_queue_destroy(msg_env_queue_t * queue)
{
    free(queue);
}

int msg_env_queue_is_empty(msg_env_queue_t* queue)
{
    if (queue == NULL)
    {
        return -1;
    }

    if (queue->head == NULL)
    {
        assert(queue->tail == NULL);
    }

    return queue->head == NULL;
}

MsgEnv* msg_env_queue_dequeue(msg_env_queue_t *queue)
{
    if (queue == NULL)
    {
        return NULL;
    }

	MsgEnv* returnEnv = queue->head;
	if (queue->head == queue->tail)
	{
		queue->tail = NULL;
	}
	if (queue->head != NULL)
	{
		queue->head = queue->head->next;
        returnEnv->next = NULL;
	}
	
	return returnEnv;
}


int msg_env_queue_enqueue(msg_env_queue_t *queue, MsgEnv* env)
{
	if (queue == NULL || env == NULL)
	{
		return ERROR_NULL_ARG;
	}

    if (queue->head == NULL)
    {
        queue->head = env;
    }
    else
    {
        queue->tail->next = env;
    }

    queue->tail = env;
    queue->tail->next = NULL;

    return CODE_SUCCESS;
}

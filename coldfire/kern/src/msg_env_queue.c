#include "msg_env_queue.h"
#include "k_globals.h"
#include "trace.h"

struct msg_env_queue {
	MsgEnv* head;
	MsgEnv* tail;
};

msg_env_queue_t * msg_env_queue_create()
{
    msg_env_queue_t * queue = k_malloc(sizeof(*queue));
    if (queue)
    {
        queue->head = NULL;
        queue-> tail = NULL;
    }
    return queue;
}

int msg_env_queue_is_empty(msg_env_queue_t* queue)
{
    return queue ? queue->head == NULL : -1;
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

int msg_env_queue_size(msg_env_queue_t *queue)
{
    if (queue == NULL)
    {
        return -1;
    }

    MsgEnv * env = queue->head;
    int size = 0;
    while (env != NULL)
    {
        env = env->next;
        size++;
    }
    return size;
}

bool msg_env_queue_has(msg_env_queue_t *queue, MsgEnv *target)
{
    if (queue == NULL)
    {
        return FALSE;
    }

    MsgEnv * env = queue->head;
    while (env != NULL)
    {
        if (env == target)
            return TRUE;
        env = env->next;
    }
    return FALSE;
}

void msg_env_queue_print(msg_env_queue_t * queue)
{
    trace_inline(ALWAYS, "     [ -> ");
    MsgEnv *node = queue->head;
    while (node != NULL)
    {
        trace_inline(ALWAYS, node->msg);
        trace_inline(ALWAYS, (" -> "));
        node = node->next;
    }
    trace(ALWAYS, "]");
}

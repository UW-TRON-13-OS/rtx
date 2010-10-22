#include <msg_env_queue.h>
#include <rtx.h>

msg_env_queue msg_env_queue_create()
{
	msg_env_queue queue;
	queue.head = NULL;
	queue.tail = NULL;
	return queue;
}

MsgEnv* msg_env_queue_dequeue(msg_env_queue* queue)
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


int msg_env_queue_enqueue(msg_env_queue* queue, MsgEnv* env)
{
	if (env == NULL)
	{
		return ERROR_NULL_ARGUMENT;
	}
	
	queue->tail->next = env;
	env->next = NULL;
	queue->tail = env;
}
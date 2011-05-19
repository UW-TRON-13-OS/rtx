#include "timeout_i_process.h"
#include "k_ipc.h"
#include "k_globals.h"
#include "msg_env_queue.h"
#include "k_signal_handler.h"
#include "k_clock.h"
#include "timeout_queue.h"

MsgEnv * timeout_queue = NULL;

void timeout_queue_insert (MsgEnv* new_msg_env)
{
    // assume new_msg_env != NULL
    assert(new_msg_env != NULL);
	
	//calculate the time with respect to the system time
    int timeout = k_clock_get_system_time() + *((int *) new_msg_env->msg);
    *((int *) new_msg_env->msg) = timeout;
	
    // Check for empty queue
    if (timeout_queue == NULL)
    {        
        timeout_queue = new_msg_env;
        return;
    }

    // Insert at head of queue
    MsgEnv* node = timeout_queue;
    int timeout_so_far = *((int*)node->msg);
    if (timeout <= *((int *) node->msg))
    {
        new_msg_env->next = node;
        timeout_queue = new_msg_env;
        return;
    }

    // Find the insertion point
    MsgEnv* prev_node = node;
    node = node->next;
    if (node)
    {
        timeout_so_far = *((int*)node->msg);
        while(timeout_so_far < timeout && node != NULL)
        {
            prev_node = node;
            node = node->next;
            timeout_so_far = *((int*)node->msg);
        }
    }

    // Insert into the queue
    prev_node->next = new_msg_env;
    new_msg_env->next = node;
}

int timeout_queue_is_empty()
{
	//check if te timeout queue is null or not.
    return timeout_queue == NULL;
}

MsgEnv * get_timeout_queue()
{
	return timeout_queue;
}

MsgEnv * check_timeout_queue(MsgEnv* msg_env)
{
    if(!timeout_queue_is_empty())
    {
        while (timeout_queue && 
               *((int *) timeout_queue->msg) <= k_clock_get_system_time())
        {
            // Dequeue the head
            msg_env = timeout_queue;
            timeout_queue = timeout_queue->next;

            return msg_env;
        }
    }
	return NULL;
}

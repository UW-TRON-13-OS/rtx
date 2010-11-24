#include "timeout_i_process.h"
#include "k_ipc.h"
#include "k_globals.h"
#include "msg_env_queue.h"
#include "k_signal_handler.h"
#include "k_clock.h"

MsgEnv * timeout_queue = NULL;
void timeout_queue_insert (MsgEnv* new_msg_env);
int timeout_queue_is_empty();

void start_timeout_i_process()
{
    while(1)
    {
        // Record a tick
        k_clock_incr_system_time();

        MsgEnv* msg_env = k_receive_message();
        while (msg_env != NULL)
        {
            timeout_queue_insert(msg_env);
            msg_env = k_receive_message();
        }

        if(!timeout_queue_is_empty())
        {
            // decrement the number of intervals of the head by 1
            (*((int *)timeout_queue->msg))--;

            while (timeout_queue && 
				   *((int *) timeout_queue->msg) <= k_clock_get_system_time())
            {
                // Dequeue the head
                msg_env = timeout_queue;
                timeout_queue = timeout_queue->next;

                // Send the envelope back
                k_send_message(msg_env->send_pid, msg_env);
            }
        }

        k_i_process_exit();

    }
}

void timeout_queue_insert (MsgEnv* new_msg_env)
{
    // assume new_msg_env != NULL
    assert(new_msg_env != NULL);
    
    // Check for empty queue
    if (timeout_queue == NULL)
    {
        timeout_queue = new_msg_env;
        return;
    }

    // Insert at head of queue
    int timeout = k_clock_get_system_time() + *((int *) new_msg_env->msg);
    MsgEnv* node = timeout_queue;
    int timeout_so_far = *((int*)node->msg);
    if (timeout <= *((int *) node->msg))
    {
        new_msg_env->next = node;
        //*((int *) node->msg) -= timeout;
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
    *((int *) new_msg_env->msg) = timeout;
    new_msg_env->next = node;
}

int timeout_queue_is_empty()
{
    return timeout_queue == NULL;
}

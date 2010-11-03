#include "timeout_i_process.h"
#include "k_ipc.h"
#include "k_globals.h"
#include "msg_env_queue.h"

void start_timeout_i_process()
{

}


int k_request_delay (int time_delay, int wakeup_code, MsgEnv* msg_env)
{
    msg_env->msg_type = wakeup_code;
    msg_env->msg = time_delay;
    k_send_message(TIMEOUT_I_PROCESS_PID, msg_env);
}

msg_env_queue_t* timeout_queue;

// called from the signal handler after every clock tick

void timeout_i_process()

{

    while(true)
    {

        MsgEnv* msg_env = k_receive_envelope()

        while(msg_env != NULL)

        {
            insert_into_timeout_queue(msg_env);
        }

        if(msg_env_queue_is_empty(timeout_queue) != 1)
        {
           
            MsgEnv *msg_head = timeout_queue->head;
            msg_head->msg = int(msg_head->msg)--;

            // decrement the number of intervals of the head by 1

            while (msg_head->msg != "0")
            {

                msg_env = msg_env_queue_dequeue(timeout_queue);  // dequeue head of timeout_queue

		k_send_message(msg_env->send_pid, msg_env);
            }
        }

        k_i_process_exit();

    }

}

void insert_into_timeout_queue (MsgEnv* new_msg_env)
{
    if(new_msg_env != NULL)
    {
        int counter = 0;

        MsgEnv* msg_env = timeout_queue->head;
    
        MsgEnv* prev_msg_env = NULL;


        while((counter < int(msg_env->msg)) && (msg_env != NULL))
        {
            counter += int(msg_env->msg);
            prev_msg_env = msg_env;
            msg_env = msg_env->next;
        }

        if(prev_msg_env != NULL)
            prev_msg_env->next = new_msg_env;
        new_msg_env->next = msg_env;
    }
}
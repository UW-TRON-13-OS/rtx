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

void timeout_i_process

{

    while(true)
    {

        MsgEnv* msg_env = k_receive_envelope()

        while(msg_env != NULL)

        {
            msg_env_queue_enqueue(timeout_queue, msg_env);
        }

        if(msg_env_queue_is_empty(timeout_queue) != 1)
        {

            decrement the number of intervals of the head by 1

            while (the number of intervals at the head is zero)

            {

                msg_env = dequeue head of timeout_queue

                k_send_message(msg_env.send_pid, msg_env)

            }

        }

        i_process_exit

    }

}

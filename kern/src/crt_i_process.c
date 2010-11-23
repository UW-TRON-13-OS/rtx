#include "crt_i_process.h"
#include "k_globals.h"
#include "rtx.h"
#include "k_signal_handler.h"
#include "k_ipc.h"
#include "k_uart.h"
#include "msg_env_queue.h"

#include <stdio.h>

void start_crt_i_process()
{
    MsgEnv* message;
    MsgEnv* prev_msg = NULL;
    msg_env_queue_t* displayQueue = msg_env_queue_create();
    int i = 0;
    crt_buf->i_process_wait_flag = CRT_FLAG_FREE;
    while (1)
    {
        if (prev_msg != NULL && crt_buf->i_process_wait_flag == CRT_FLAG_FREE)
        {
            prev_msg->msg_type = DISPLAY_ACK;
            k_send_message(prev_msg->send_pid, prev_msg);
            prev_msg = NULL;
        }
        
        message = k_receive_message();
        msg_env_queue_enqueue(displayQueue, message);
        
        if (crt_buf->i_process_wait_flag == CRT_FLAG_FREE && msg_env_queue_is_empty(displayQueue) != 1)
        {
            message = msg_env_queue_dequeue(displayQueue);
            assert(message != NULL);
            i = 0;
            while (message->msg[i] != '\0')
            {
                crt_buf->data[i] = message->msg[i];
                i++;
            }
            crt_buf->data[i] = '\0';
            crt_buf->i_process_wait_flag = CRT_FLAG_WAIT;
            prev_msg = message;
        }
        k_i_process_exit();
    }
}

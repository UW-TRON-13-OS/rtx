#include "crt_i_process.h"
#include "k_globals.h"
#include "rtx.h"
#include "k_signal_handler.h"
#include "k_ipc.h"
#include "k_uart.h"
#include "msg_env_queue.h"

#include <stdio.h>

msg_env_queue_t* displayQueue = NULL;
MsgEnv* prev_msg = NULL;
void start_crt_i_process()
{
    displayQueue = msg_env_queue_create();
    MsgEnv* message;
    int i = 0;
    crt_buf->i_process_wait_flag = '0';
    while (1)
    {
        if (prev_msg != NULL)
        {
            prev_msg->msg_type = DISPLAY_ACK;
            k_send_message(prev_msg->send_pid, prev_msg);
            prev_msg = NULL;
        }
        
        message = k_receive_message();
        msg_env_queue_enqueue(displayQueue, message);
        
        if (crt_buf->i_process_wait_flag == '0' && msg_env_queue_is_empty(displayQueue) != 1)
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
            crt_buf->i_process_wait_flag = '1';
            prev_msg = message;
        }
        k_i_process_exit();
    }
}

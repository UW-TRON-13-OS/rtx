#include "kb_i_process.h"
#include "k_globals.h"
#include "rtx.h"
#include "k_signal_handler.h"
#include "k_ipc.h"
#include "k_uart.h"

#include <stdio.h>

void start_kb_i_process()
{
    int i;
    while (1)
    {
        MsgEnv* message = k_receive_message();
        if (message != NULL)
        {
            for (i = 0; i < kb_buf->length; i++)
            {
                message->msg[i] = kb_buf->data[i];
            }
            message->msg[i] = '\0';
            message->msg_type = CONSOLE_INPUT;
            kb_buf->kb_wait_flag = KB_FLAG_FREE;
            k_send_message(message->send_pid, message);
        }
        kb_buf->length = 0;
        k_i_process_exit();
    }
}

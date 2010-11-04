#include "crt_i_process.h"
#include "k_globals.h"
#include "rtx.h"
#include "k_signal_handler.h"
#include "k_ipc.h"
//#include "kern/inc/msg_env_queue.h"

void start_crt_i_process()
{
    MsgEnv* message;
    int i = 0;
    while (1)
    {
        message = receive_message();
        
        assert(message != NULL);
        
        if (crt_buf->i_process_wait_flag == '0')
        {
            i = 0;
            assert(crt_buf->length == 0);
            while (message->msg[i] != '\0')
            {
                crt_buf->data[_crt_buf->length++] = message->msg[i++];
            }
            message->msg_type = DISPLAY_ACK;
            crt_buf->i_process_wait_flag = '1';
            k_send_message(message->send_pid, message);
        }
        k_i_process_exit();
    }
}

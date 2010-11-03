#include "kb_i_process.h"
#include "k_globals.h"
#include "rtx.h"
#include "k_signal_handler.h"

recv_buf_t * _kb_buf;

void kb_register_shmem(recv_buf_t * kb_buf)
{
    _kb_buf = kb_buf;
}

void start_kb_i_process()
{
    int i;
    while (1)
    {
        MsgEnv* message = receive_message();
        if (message != NULL)
        {
            for (i = 0; i < _kb_buf->length; i++)
            {
                message->msg[i] = _kb_buf->data[i];
            }
            message->msg_type = CONSOLE_INPUT;
            _kb_buf->kb_wait_flag = '0';
            k_send_message(message->send_pid, message);
        }
        _kb_buf->length = 0;
        k_i_process_exit();
    }
}

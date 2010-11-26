#include "rtx.h"
#include "rtx_util.h"
#include "msg_env_queue.h"
#include <stdarg.h>
#include <stdio.h>

/** Adaption of printf for the CRT using variable arguments **/
int RTX_printf(MsgEnv* send_env, msg_env_queue_t* msgQ, const char* format, ...)
{
    if (format == NULL)
    {
        return ERROR_NULL_ARG;
    }
    va_list args;
    int status;

    va_start (args, format);
    vsprintf(send_env->msg, format, args);
    va_end (args);

    //Send formatted string to CRT
    status = send_console_chars(send_env);
    if (CODE_SUCCESS == status)
    {
        while (1)
        {
            MsgEnv *env = receive_message();
            if (env->msg_type == DISPLAY_ACK)
            {
                break;
            }
            //store envelopes not used in local queue for CCI main loop
            else
            {
                msg_env_queue_enqueue(msgQ, env);
            }
        }
    }
    return status;    
}


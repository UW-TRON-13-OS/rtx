#include "k_console.h"
#include "rtx.h"
#include "k_globals.h"
#include "k_ipc.h"

int k_send_console_chars(MsgEnv *msg_env)
{
    if (msg_env == NULL)
        return ERROR_NULL_ARG;
    msg_env->msg_type = CONSOLE_OUTPUT;
    return k_send_message(CRT_I_PROCESS_PID, msg_env);
}

int k_get_console_chars(MsgEnv *msg_env)
{
    if (msg_env == NULL)
        return ERROR_NULL_ARG;
    msg_env->msg_type = REQUEST_CHAR;
    return k_send_message(KB_I_PROCESS_PID, msg_env);
}

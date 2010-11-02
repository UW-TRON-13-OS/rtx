#include "k_console.h"
#include "rtx.h"
#include "k_globals.h"

int k_send_console_chars(MsgEnv *msg_env)
{
    if (msg_env == NULL)
        return ERROR_NULL_ARG;
    int ret;
    msg_env->msg_type = CONSOLE_OUTPUT;
    ret = k_send_message(CRT_IPROCESS_PID, msg_env);
    // send SIGUSR2 signal to RTX process to trigger CRT i-process
    kill(rtx_pid, SIGUSR2);
    return ret;
}

int k_get_console_chars(MsgEnv *msg_env)
{
    if (msg_env == NULL)
        return ERROR_NULL_ARG;
    msg_env->msg_type = REQUEST_CHAR;
    return k_send_message(KB_I_PROCESS_PID, msg_env);
}

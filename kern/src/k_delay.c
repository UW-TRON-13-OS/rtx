#include "k_delay.h"
#include "k_globals.h"

int k_request_delay(int time_delay, int wakeup_code, MsgEnv *msg_env)
{
    msg_env->msg_type = wakeup_code;
    int * delay_data = (int *) msg_env->msg;
    *delay_data = time_delay;
    return k_send_message(TIMEOUT_I_PROCESS_PID, msg_env);
}

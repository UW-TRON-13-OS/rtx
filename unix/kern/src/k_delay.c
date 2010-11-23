#include "k_delay.h"
#include "k_globals.h"
#include "k_ipc.h"

#ifdef DEBUG_KERN
#include <stdio.h>
#endif

int k_request_delay(int time_delay, int wakeup_code, MsgEnv *msg_env)
{
#ifdef DEBUG_KERN
    printf("%s is requesting a delay of %d with wakeup code %d\n", current_process->name,time_delay, wakeup_code);
#endif
    msg_env->msg_type = wakeup_code;
    int * delay_data = (int *) msg_env->msg;
    *delay_data = time_delay;
    return k_send_message(TIMEOUT_I_PROCESS_PID, msg_env);
}

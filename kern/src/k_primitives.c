#include <rtx.h>
#include "k_atomic.h"
#include "k_ipc.h"
#include "k_process.h"

int send_message(int dest_pid, MsgEnv *msg_env)
{
    atomic(ON);
    int ret_value = k_send_message(dest_pid, msg_env);
    atomic(OFF);
    return ret_value;
}

MsgEnv * receive_message()
{
    atomic(ON);
    MsgEnv* ret_value = k_receive_message();
    atomic(OFF);
    return ret_value;
}

MsgEnv * request_msg_env()
{
    atomic(ON);
    MsgEnv* ret_value = k_request_msg_env();
    atomic(OFF);
    return ret_value;
}

int release_msg_env(MsgEnv * msg_env)
{
    atomic(ON);
    int ret_value = k_release_msg_env(msg_env);
    atomic(OFF);
    return ret_value;
}

int release_processor()
{
    atomic(ON);
    int ret_value = k_release_processor();
    atomic(OFF);
    return ret_value;
}

int request_process_status(MsgEnv *msg_env)
{
    atomic(ON);
    int ret_value = k_request_process_status(msg_env);
    atomic(OFF);
    return ret_value;
}

int terminate()
{
    atomic(ON);
    int ret_value = k_terminate();
    atomic(OFF);
    return ret_value;
}

int change_priority(int new_priority, int target_process_id)
{
    atomic(ON);
    int ret_value = k_change_priority(new_priority, target_process_id);
    atomic(OFF);
    return ret_value;
}


int request_delay(int time_delay, int wakeup_code, MsgEnv *msg_env)
{
    atomic(ON);
    int ret_value = k_request_delay(time_delay, wakeup_code, msg_env);
    atomic(OFF);
    return ret_value;
}

int send_console_chars(MsgEnv *msg_env)
{
    // TODO implement send_console_chars
    return -1;
}

int get_console_chars(MsgEnv *msg_env)
{
    // TODO implement get_console_chars
    return -1;
}

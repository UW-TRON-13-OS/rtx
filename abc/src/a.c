#include "rtx.h"
#include "processes.h"
#include "abc.h"

#include <stdio.h>

void process_A()
{
    MsgEnv * start_msg = receive_message();
    release_msg_env(start_msg);
    int counter = 0;
    while(1)
    {
        MsgEnv * count_msg = request_msg_env();
        count_msg->msg_type = COUNT_REPORT;
        sprintf(count_msg->msg, "%d", counter);
        send_message(PROCESS_B_PID, count_msg);
        counter++;
        release_processor();
    }
}

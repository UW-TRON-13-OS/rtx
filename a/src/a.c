#include "rtx.h"
#include "processes.h"
#include "a.h"

#include <string.h>


int counter;
void process_A()
{
    MsgEnv * received_msg = receive_message;
    release_msg_env(start_msg);
    counter = 0;
    while(1)
    {
        MsgEnv * count_msg = request_msg_env();
        received_msg->msg_type = COUNT_REPORT;
        sprintf(received_msg->msg, "%d", counter);
        send_message(PROCESS_B_PID, received_msg);
        counter++;
        release_processor();
    }
}

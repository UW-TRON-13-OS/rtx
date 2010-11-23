#include "rtx.h"
#include "processes.h"
#include "abc.h"

#include <stdio.h> // TODO remove when done

void process_A()
{
    //recieve a message env from the CCI
    //this envelope will cause process A to start
    MsgEnv * start_msg = receive_message();
    release_msg_env(start_msg);

    //start a counter
    int counter = 0;
    while(1)
    {
        // request a message env to store the counter value
        MsgEnv * count_msg = request_msg_env();
        count_msg->msg_type = COUNT_REPORT;
        *((int *) count_msg->msg) = counter;
        //send the message env to process B
        send_message(PROCESS_B_PID, count_msg);
        //increment the counter by one
        counter++;
        //yield the processor when done
        release_processor();
    }
}

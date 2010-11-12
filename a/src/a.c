#include "cci.h"
#include "rtx.h"
#include "processes.h"
#include "a.h"
//#include <stdlib.h>


int counter;
void process_A()
{
    MsgEnv *msg_env = request_msg_env();
    msg_env->msg = "s";
    send_console_chars(msg_env);

    MsgEnv *received_msg = NULL;
    //fprint("s\n");
    //start_cci();
    do
    {
        received_msg = receive_message();
    }while(received_msg != NULL);
    counter = 0;
    while(1)
    {
        received_msg->msg_type = COUNT_REPORT;
        received_msg->msg = "" + counter;

        send_message(PROCESS_B_PID, received_msg);
        counter++;
        release_processor();
    }
}

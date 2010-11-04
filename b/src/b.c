#include "rtx.h"
#include "processes.h"
#include "b.c"


void process_B()
{
    while(1)
    {
        MsgEnv *received_msg = receive_message();
        send_message(PROCESS_C_PID, received_msg);
        //release_processor();
    }
}

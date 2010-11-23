#include "rtx.h"
#include "processes.h"
#include "abc.h"

#include <stdio.h> // TODO remove when done

void process_B()
{
    while(1)
    {
        MsgEnv *received_msg = receive_message();
        send_message(PROCESS_C_PID, received_msg);
    }
}

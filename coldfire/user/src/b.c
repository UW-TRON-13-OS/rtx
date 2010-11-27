#include "rtx.h"
#include "processes.h"
#include "abc.h"

void process_B()
{
    while(1)
    {
	// Recieve a message env
        MsgEnv *received_msg = receive_message();
	//Send the recieved message env to process C.
        send_message(PROCESS_C_PID, received_msg);
    }
}
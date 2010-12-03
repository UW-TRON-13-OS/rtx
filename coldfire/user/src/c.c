#include "rtx.h"
#include "user_processes.h"
#include "abc.h"
#include "msg_env_queue.h"
#include "utils.h"
#include "trace.h"

void process_C()
{
    msg_env_queue_t* messageQueue = msg_env_queue_create();
    MsgEnv * timeout_msg = request_msg_env();
    while(1)
    {
        // setup variables that will be used by the process
        MsgEnv *deq_msg;
        // check if the process message queue is empty or not
        // if the queue is empty, then recieve a message env
        // and enqueue it!
        if(msg_env_queue_is_empty(messageQueue))
        {
            MsgEnv *received_msg = receive_message();
            msg_env_queue_enqueue(messageQueue, received_msg);
        }
        // dequeue a message env from the message queue
        deq_msg = msg_env_queue_dequeue(messageQueue);

        // check if the dequeued message type is a COUNT_REPORT
        if(deq_msg->msg_type == COUNT_REPORT)
        {
            //check if the message in the env is divisible by 20
            //if it is divisible, then continue into the if statement
            //otherwise release all envelopes and and yeild processor
            if (*((int *)(deq_msg->msg)) % 20 == 0)
            {
                //copys the 'Process C' string into the dequeued msg env
                rtx_strcpy(deq_msg->msg, "\r\nProcess C\r\n", 1024);
                //send the message env to the console for printing
                send_console_chars(deq_msg, 0);

                request_delay(1000, WAKEUP_10, timeout_msg);

                // "Go passive for 10 seconds" in the outline
                while(1)
                {
                    //recieve a message env
                    deq_msg = receive_message();
                    //if the recieved env is a type 'WAKEUP_10' then
                    //exit the while loop
                    //otherwise, enqueue the recieved message env back into the
                    //message env queue
                    if(deq_msg->msg_type == WAKEUP_10)
                    {
                        break;
                    }
                    else
                    {
                        msg_env_queue_enqueue(messageQueue, deq_msg);
                    }
                }
            }
            else
            {
                release_msg_env(deq_msg);
            }
        }
        else
        {
            trace(ERROR, "WTEDF");
            release_msg_env(deq_msg);
        }
        //release all the envelopes and yeild the processor
        release_processor();
    }
}

#include "rtx.h"
#include "processes.h"
#include "abc.h"
#include "msg_env_queue.h"

#include <string.h>

void process_C()
{
    msg_env_queue_t* messageQueue = msg_env_queue_create();
    while(1)
    {
        // setup variables that will be used by the process
        MsgEnv *deq_msg;
        MsgEnv *rec_msg = NULL;
        MsgEnv *rec_msg_2 = NULL;
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
                strcpy(deq_msg->msg, "\nProcess C\n");
                //send the message env to the console for printing
                send_console_chars(deq_msg);

                //loop forever (until 'breaked')
                while(1)
                {
                   //recieve a message
                   rec_msg = receive_message();
                   //check if the recieved message is a type "DISPLAY_ACK"
                   //if not then check if it is a type "COUNT_REPORT"
                   //and if that fails then we got a error and the RTX should report it!
                   if(rec_msg->msg_type == DISPLAY_ACK)
                   {
                       //check for an error case
                       if(request_delay(100, WAKEUP_10, rec_msg) != CODE_SUCCESS)
                       {
                           //printf("An error occurred.\n");
                       }
                       break;
                   }
                   else if (rec_msg->msg_type == COUNT_REPORT)
                   {
                       //enqueue the released env back into the message queue
                       msg_env_queue_enqueue(messageQueue, rec_msg);
                   }
                   else
                   {
                       assert(0);
                   }
                }

                // "Go passive for 10 seconds" in the outline
                while(1)
                {
                    //recieve a message env
                    rec_msg_2 = receive_message();
                    //if the recieved env is a type 'WAKEUP_10' then
                    //exit the while loop
                    //otherwise, enqueue the recieved message env back into the
                    //message env queue
                    if(rec_msg_2->msg_type == WAKEUP_10)
                    {
                        break;
                    }
                    else
                    {
                        msg_env_queue_enqueue(messageQueue, rec_msg_2);
                    }
                }
            }
        }
        //release all the envelopes and yeild the processor
        release_msg_env(rec_msg);
        release_msg_env(rec_msg_2);
        release_msg_env(deq_msg);
        release_processor();
    }
}

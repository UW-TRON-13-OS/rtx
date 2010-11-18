#include "rtx.h"
#include "processes.h"
#include "abc.h"
#include "msg_env_queue.h"

#include <stdio.h> // TODO remove later

msg_env_queue_t* messageQueue = NULL;
void process_C()
{
    MsgEnv* deq_msg;
    while(1)
    {
        MsgEnv *rec_msg = NULL;
        if(msg_env_queue_is_empty(messageQueue))
        {
            MsgEnv *received_msg = receive_message();
            msg_env_queue_enqueue(messageQueue, received_msg);
        }
        else
        {
            deq_msg = msg_env_queue_dequeue(messageQueue);
        }
        if((deq_msg->msg_type == COUNT_REPORT) && (*((int *)(deq_msg->msg)) % 20 == 0))
        {
            deq_msg->msg = "Process C";
            send_console_chars(deq_msg);

            while(1)
            {
               rec_msg = request_msg_env();
               if(rec_msg->msg_type == DISPLAY_ACK)
               {
                   if(request_delay(100, WAKEUP_10, rec_msg) != CODE_SUCCESS)
                   {
                       printf("An error occurred.\n");
                   }

                   break;
               }
               /*else
               {
                   msg_env_queue_enqueue(messageQueue, rec_msg);
               }*/
            }

            // "Go passive for 10 seconds" in the outline
            while(1)
            {
                MsgEnv *rec_msg = request_msg_env();
                if(rec_msg->msg_type == WAKEUP_10)
                {
                    break;
                }
                else
                {
                    msg_env_queue_enqueue(messageQueue, rec_msg);
                }
            }
        }
        release_msg_env(rec_msg);
        release_processor();
    }
}

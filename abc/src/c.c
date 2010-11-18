#include "rtx.h"
#include "processes.h"
#include "abc.h"
#include "msg_env_queue.h"

#include <stdio.h> // TODO remove later
#include <string.h>

void process_C()
{
    MsgEnv* deq_msg;
    msg_env_queue_t* messageQueue = msg_env_queue_create();
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
        printf("alksjdflsjflkdjfs %p\n", deq_msg);
        if(deq_msg->msg_type == COUNT_REPORT)
        { 
            printf("count %d\n", *((int *)(deq_msg->msg)));
            if (*((int *)(deq_msg->msg)) % 20 == 0)
            {
                strcpy(deq_msg->msg, "Process C");
                send_console_chars(deq_msg);

                while(1)
                {
                   rec_msg = receive_message();
                   if(rec_msg->msg_type == DISPLAY_ACK)
                   {
                       if(request_delay(100, WAKEUP_10, rec_msg) != CODE_SUCCESS)
                       {
                           //printf("An error occurred.\n");
                       }
                       break;
                   }
                   else if (rec_msg->msg_type == WAKEUP_10)
                   {
                       msg_env_queue_enqueue(messageQueue, rec_msg);
                   }
                   else
                   {
                       printf("AJSLKFDJALSKJFLKDSJF SHOULD NOT BE HERE WAHHHH\n");
                       assert(0);
                   }
                }

                // "Go passive for 10 seconds" in the outline
                while(1)
                {
                    MsgEnv *rec_msg = receive_message();
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
        }
        release_msg_env(rec_msg);
        release_processor();
    }
}

#include "rtx.h"
#include "processes.h"
#include "c.c"
#include "msg_env_queue.h"

msg_env_queue_t* messageQueue;
void process_C()
{
    while(1)
    {
        if(msg_env_queue_is_empty(messageQueue))
        {
            MsgEnv *received_msg = receive_message();
            msg_env_queue_enqueue(messageQueue, received_msg);
        }
        else
        {
            MsgEnv* deq_msg = msg_env_queue_dequeue(messageQueue);
            if((deq_msg->msg_type == COUNT_REPORT) && 
               (int(deq_msg->msg) % 20 == 0))
            {
                deq_msg->msg = "Process C";
                send_console_chars(deq_msg);

                while(1)
                {
                   MsgEnv *rec_msg = request_msg_env();
                   if(rec_msg->msg_type == DISPLAY_ACK)
                   {
                       if(request_delay(10, WAKEUP_10, rec_msg) != 0)
                       {
                           // An error occured!!!
                       }

                       break;
                   }
                   else
                   {
                       msg_env_queue_enqueue(messageQueue, rec_msg);
                   }
                }

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
        }
        release_msg_env(rec_msg);
        release_processor();
    }
}

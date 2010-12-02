#include "cci_util.h"
#include "rtx.h"
#include "processes.h"
#include "rtx_util.h"

//prints process statuses on console given the envelope message data
int CCI_printProcessStatuses (char* raw_data, MsgEnv* send_env,
                              msg_env_queue_t* msgQ)
{
    if (raw_data == NULL)
        return ERROR_NULL_ARG;
    int * data = (int *) raw_data;
    int num_processes = *data++;
    int i;
	send_env->msg = "PID | STATUS                | PRIORITY\n\0";
    send_console_chars(send_env);
    for (i=0;i<num_processes;i++)
    {
        CHAR format = "  %d   \0";
        CHAR str[100];
        void * params[] = { &(*data++) };
        send_env->msg = rtx_spritf(&str, &format, params);
        send_console_chars(send_env);
        switch(*data)
        {
            case P_READY:
                send_env->msg = "ready                  \0";
                send_console_chars(send_env);
                break;
            case P_EXECUTING:
                send_env->msg = "executing              \0";
                send_console_chars(send_env);
                break;
            case P_BLOCKED_ON_ENV_REQUEST:
                send_env->msg = "blocked on env request \0";
                send_console_chars(send_env);
                break;
            case P_BLOCKED_ON_RECEIVE:
                send_env->msg = "blocked on receive     \0";
                send_console_chars(send_env);
                break;
            default :
                send_env->msg = "                       \0";
                send_console_chars(send_env);
                break;
        }
        data++;

        format = " %d\n\0";
        * params[] = { &(*data++) };
        send_env->msg = rtx_spritf(&str, &format, params);
        send_console_chars(send_env);
    }
    return CODE_SUCCESS;
}

//prints trace buffers on console given the envelope message data
int CCI_printTraceBuffers (char* data, MsgEnv* send_env, msg_env_queue_t* msgQ)
{
    if (data == NULL)
        return ERROR_NULL_ARG;
    int i;
    ipc_trace_t *send_dump = (ipc_trace_t *) data;
    ipc_trace_t *recv_dump = send_dump + IPC_MESSAGE_TRACE_HISTORY_SIZE; 
 
    send_env->msg = "MESSAGE TRACE BUFFERS\n\n"
                   " Send Trace                   || Receive Trace\n"
                   " Dest |Sender|Message|  Time  || Dest |Sender|Message|  Time\n"
                   " PID  |PID   |Type   |        || PID  |PID   |Type   |\n"
                   "--------------------------------------------------------------\n\0";
    send_console_chars(send_env);
    for (i=0;i<IPC_MESSAGE_TRACE_HISTORY_SIZE;i++)
    {
        if (send_dump[i].time_stamp != MAX_UINT32)
        {
            //TODO: Fix '%2u' and such in sprintf...

            CHAR format = "   %2u |   %2u |   %3d | %6u ||\0";
            CHAR str[100];
            void * params[] = { &(send_dump[i].dest_pid), &(send_dump[i].send_pid),
                                &(send_dump[i].msg_type), &(send_dump[i].time_stamp)};
            send_env->msg = rtx_spritf(&str, &format, params);

            send_console_chars(send_env);
        }
        else if (recv_dump[i].time_stamp != MAX_UINT32)
        {
            send_env->msg = "      |      |       |       ||\0";
            send_console_chars(send_env);
        }
        else
        {
            break;
        }
        
        if (recv_dump[i].time_stamp != MAX_UINT32)
        {
            //TODO: Fix '%2u' and such in sprintf...

            CHAR format = "   %2u |   %2u |   %3d | %6u\n\0";
            CHAR str[100];
            void * params[] = { &(send_dump[i].dest_pid), &(recv_dump[i].send_pid),
                                &(recv_dump[i].msg_type), &(recv_dump[i].time_stamp)};
            send_env->msg = rtx_spritf(&str, &format, params);

            send_console_chars(send_env);
        }
        else
        {
            send_env->msg = "      |      |       |       \n\0";
            send_console_chars(send_env);
        }
    }        
    
    send_env->msg = "\n\0";
    send_console_chars(send_env);

    return CODE_SUCCESS;
}

int CCI_setWallClock (MsgEnv *send_env, msg_env_queue_t *msgQ, char* newTime)
{
    int status;
    strcpy(send_env->msg, newTime);
    send_env->msg_type = CLOCK_SET;
    send_message(PROCESS_WALLCLOCK_PID,send_env);

    while (1)
    {
        MsgEnv *env = receive_message();
        if (env->msg_type == CLOCK_RET)
        {
            status = *((int *)env->msg);
            break;
        }
        //store envelopes not used in local queue for CCI main loop
        else
        {
            msg_env_queue_enqueue(msgQ, env);
        }
    }

    return status;
}

void CCI_displayWallClock (MsgEnv *send_env, msg_env_queue_t *msgQ, int disp_b)
{
    if (disp_b)
    {
        send_env->msg_type = CLOCK_ON;
    }
    else
    {
        send_env->msg_type = CLOCK_OFF;
    }

    send_message(PROCESS_WALLCLOCK_PID,send_env);

    while (1)
    {
        MsgEnv *env = receive_message();
        if (env->msg_type == CLOCK_RET)
        {
            break;
        }
        //store envelopes not used in local queue for CCI main loop
        else
        {
            msg_env_queue_enqueue(msgQ, env);
        }
    }
}

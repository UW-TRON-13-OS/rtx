#include "cci_util.h"
#include "rtx.h"
#include "processes.h"
#include "rtx_util.h"
#include "wallclock.h"
#include <string.h>

//prints process statuses on console given the envelope message data
int CCI_printProcessStatuses (char* raw_data, MsgEnv* send_env,
                              msg_env_queue_t* msgQ)
{
    if (raw_data == NULL)
        return ERROR_NULL_ARG;
    int * data = (int *) raw_data;
    int num_processes = *data++;
    int i;
    RTX_printf(send_env, msgQ, "PID | STATUS                | PRIORITY\n");
    for (i=0;i<num_processes;i++)
    {
        RTX_printf(send_env, msgQ, "  %d   ",*data++);
        switch(*data)
        {
            case P_READY:
                RTX_printf(send_env, msgQ, "ready                  ");
                break;
            case P_EXECUTING:
                RTX_printf(send_env, msgQ, "executing              ");
                break;
            case P_BLOCKED_ON_ENV_REQUEST:
                RTX_printf(send_env, msgQ, "blocked on env request ");
                break;
            case P_BLOCKED_ON_RECEIVE:
                RTX_printf(send_env, msgQ, "blocked on receive     ");
                break;
            default :
                RTX_printf(send_env, msgQ, "                       ");
                break;
        }
        data++;
        RTX_printf(send_env, msgQ, " %d\n",*data++);
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
 
    RTX_printf(send_env, msgQ,     "MESSAGE TRACE BUFFERS\n\n"
                   " Send Trace                   || Receive Trace\n"
                   " Dest |Sender|Message|  Time  || Dest |Sender|Message|  Time\n"
                   " PID  |PID   |Type   |        || PID  |PID   |Type   |\n"
                   "--------------------------------------------------------------\n");
    for (i=0;i<IPC_MESSAGE_TRACE_HISTORY_SIZE;i++)
    {
        if (send_dump[i].time_stamp != MAX_UINT32)
        {
            RTX_printf(send_env, msgQ, "   %2u |   %2u |   %3d | %6u ||",
                       send_dump[i].dest_pid, send_dump[i].send_pid,
                       send_dump[i].msg_type, send_dump[i].time_stamp);
        }
        else if (recv_dump[i].time_stamp != MAX_UINT32)
        {
            RTX_printf(send_env, msgQ, "      |      |       |       ||");
        }
        else
        {
            break;
        }
        
        if (recv_dump[i].time_stamp != 0)
        {
            RTX_printf(send_env, msgQ, "   %2u |   %2u |   %3d | %6u\n",send_dump[i].dest_pid,
                       recv_dump[i].send_pid, recv_dump[i].msg_type,
                       recv_dump[i].time_stamp);
        }
        else
        {
            RTX_printf(send_env, msgQ, "      |      |       |       \n");
        }
    }        
    
    RTX_printf(send_env, msgQ, "\n");

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


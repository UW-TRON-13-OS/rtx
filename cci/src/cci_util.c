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
        if (send_dump[i].time_stamp != 0)
        {
            RTX_printf(send_env, msgQ, "   %2u |   %2u |   %3d | %6lu ||",
                       send_dump[i].dest_pid, send_dump[i].send_pid,
                       send_dump[i].msg_type, send_dump[i].time_stamp);
        }
        else if (recv_dump[i].time_stamp != 0)
        {
            RTX_printf(send_env, msgQ, "      |      |       |       ||");
        }
        else
        {
            break;
        }
        
        if (recv_dump[i].time_stamp != 0)
        {
            RTX_printf(send_env, msgQ, "   %2u |   %2u |   %3d | %6lu\n",send_dump[i].dest_pid,
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


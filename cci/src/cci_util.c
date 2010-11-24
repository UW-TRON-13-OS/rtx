#include "cci_util.h"
#include "rtx.h"
#include "processes.h"
#include <stdlib.h>

//like CCI_printf, except for the CCI. and possibly broken.
int CCI_printf (const char* format, ...);

//prints process statuses on console given the envelope message data
int CCI_printProcessStatuses (char* raw_data)
{
    if (raw_data == NULL)
        return ERROR_NULL_ARG;
    int * data = (int *) raw_data;
    int num_processes = *data++;
    int i;
    CCI_printf ("PID | STATUS                | PRIORITY\n");
    for (i=0;i<num_processes;i++)
    {
        CCI_printf("  %d   ",*data++);
        switch(*data)
        {
            case P_READY:
                CCI_printf("ready                  ");
                break;
            case P_EXECUTING:
                CCI_printf("executing              ");
                break;
            case P_BLOCKED_ON_ENV_REQUEST:
                CCI_printf("blocked on env request ");
                break;
            case P_BLOCKED_ON_RECEIVE:
                CCI_printf("blocked on receive     ");
                break;
            default :
                CCI_printf("                       ");
                break;
        }
        data++;
        CCI_printf(" %d\n",*data++);
    }
    return CODE_SUCCESS;
}

//prints trace buffers on console given the envelope message data
int CCI_printTraceBuffers (char* data)
{
    if (data == NULL)
        return ERROR_NULL_ARG;
    int i;
    ipc_trace_t *send_dump = (ipc_trace_t *) data;
    ipc_trace_t *recv_dump = send_dump + IPC_MESSAGE_TRACE_HISTORY_SIZE; 
 
    CCI_printf(    "MESSAGE TRACE BUFFERS\n\n"
                   " Send Trace                   || Receive Trace\n"
                   " Dest |Sender|Message|  Time  || Dest |Sender|Message|  Time\n"
                   " PID  |PID   |Type   |        || PID  |PID   |Type   |\n"
                   "--------------------------------------------------------------\n");
    for (i=0;i<IPC_MESSAGE_TRACE_HISTORY_SIZE;i++)
    {
        if (send_dump[i].time_stamp != 0)
        {
            CCI_printf("   %2u |   %2u |   %3d | %6lu ||",
                       send_dump[i].dest_pid, send_dump[i].send_pid,
                       send_dump[i].msg_type, send_dump[i].time_stamp);
        }
        else if (recv_dump[i].time_stamp != 0)
        {
            CCI_printf("      |      |       |       ||");
        }
        else
        {
            break;
        }
        
        if (recv_dump[i].time_stamp != 0)
        {
            CCI_printf("   %2u |   %2u |   %3d | %6lu\n",send_dump[i].dest_pid,
                       recv_dump[i].send_pid, recv_dump[i].msg_type,
                       recv_dump[i].time_stamp);
        }
        else
        {
            CCI_printf("      |      |       |       \n");
        }
    }        
    
    CCI_printf("\n");

    return CODE_SUCCESS;
}


#include "cci_util.h"
#include "rtx.h"
#include "k_primitives.h"
#include "user_processes.h"
#include "utils.h"
#include "wallclock.h"
#include "k_globals.h"
#include "trace.h"

void CCI_print(char * msg)
{
    MsgEnv *env = request_msg_env();
    rtx_strcpy(env->msg, msg, IPC_MSG_ENV_MSG_SIZE);
    send_console_chars(env);
}

//prints process statuses on console given the envelope message data
int CCI_printProcessStatuses (char* raw_data)
{
    if (raw_data == NULL)
    {
        return ERROR_NULL_ARG;
    }
    int * data = (int *) raw_data;
    int num_processes = *data++;
    int i;
    CHAR str[100]; 
    void * params [11];

	CCI_print( "PID | STATUS                | PRIORITY\r\n" );
    for (i=0;i<num_processes;i++)
    {
        params[0] = &(*data++);
        params[1] = NULL;
        rtx_sprintf(str, "%4d   ", params);
        CCI_print(str);
        switch(*data)
        {
            case P_READY:
                CCI_print("ready                  ");
                break;
            case P_EXECUTING:
                CCI_print("executing              ");
                break;
            case P_BLOCKED_ON_ENV_REQUEST:
                CCI_print("blocked on env request ");
                break;
            case P_BLOCKED_ON_RECEIVE:
                CCI_print("blocked on receive     ");
                break;
            default :
                CCI_print("                       ");
                break;
        }
        data++;

        params[0] = &(*data++);
        params[1] = NULL;
        rtx_sprintf(str, " %d\r\n", params);
        CCI_print(str);
    }
    return CODE_SUCCESS;
}

//prints trace buffers on console given the envelope message data
int CCI_printTraceBuffers (char* data)
{
    if (data == NULL)
        return ERROR_NULL_ARG;
    int i;
    CHAR str[100]; 
    void * params [11];

    ipc_trace_t *send_dump = (ipc_trace_t *) data;
    ipc_trace_t *recv_dump = send_dump + IPC_MESSAGE_TRACE_HISTORY_SIZE; 
 
    CCI_print("MESSAGE TRACE BUFFERS\r\n\r\n"
              " Send Trace                   || Receive Trace\r\n"
              " Dest |Sender|Message|  Time  || Dest |Sender|Message|  Time\r\n"
              " PID  |PID   |Type   |        || PID  |PID   |Type   |\r\n"
              "--------------------------------------------------------------\r\n");
    for (i=0;i<IPC_MESSAGE_TRACE_HISTORY_SIZE;i++)
    {
        if (send_dump[i].time_stamp != MAX_UINT32)
        {
            //TODO: Fix '%2u' and such in sprintf...

            params[0] = &(send_dump[i].dest_pid);
            params[1] = &(send_dump[i].send_pid);
            params[2] = &(send_dump[i].msg_type);
            params[3] = &(send_dump[i].time_stamp);
            params[4] = NULL;
            rtx_sprintf(str, "   %2u |   %2u |   %3d | %6u ||", params);
            CCI_print (str);
        }
        else if (recv_dump[i].time_stamp != MAX_UINT32)
        {
            CCI_print("      |      |       |       ||");
        }
        else
        {
            break;
        }
        
        if (recv_dump[i].time_stamp != MAX_UINT32)
        {
            //TODO: Fix '%2u' and such in sprintf...

            params[0] = &(recv_dump[i].dest_pid);
            params[1] = &(recv_dump[i].send_pid);
            params[2] = &(recv_dump[i].msg_type);
            params[3] = &(recv_dump[i].time_stamp);
            params[4] = NULL;
            rtx_sprintf(str, "   %2u |   %2u |   %3d | %6u\r\n", params);
            CCI_print (str);
        }
        else
        {
            CCI_print("      |      |       |       \r\n");
        }
    }        
    
    CCI_print("\r\n");

    return CODE_SUCCESS;
}

int CCI_setWallClock (MsgEnv *send_env, char* newTime)
{
    int status;
    rtx_strcpy(send_env->msg, newTime, 1024);
    send_env->msg_type = CLOCK_SET;
    send_message(WALLCLOCK_PID, send_env);

    return status;
}

void CCI_displayWallClock (MsgEnv *send_env, int disp_b)
{
    if (disp_b)
    {
        send_env->msg_type = CLOCK_ON;
    }
    else
    {
        send_env->msg_type = CLOCK_OFF;
    }

    send_message(WALLCLOCK_PID,send_env);
}


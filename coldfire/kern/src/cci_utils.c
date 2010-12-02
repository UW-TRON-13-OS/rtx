#include "cci_util.h"
#include "rtx.h"
#include "k_primitives.h"
#include "processes.h"
#include "utils.h"
#include "wallclock.h"
#include "k_globals.h"

void CCI_print(char * msg)
{
    MsgEnv *env = request_msg_env();
    rtx_strcpy(env->msg, msg, IPC_MSG_ENV_MSG_SIZE);
    send_console_chars(env);
}

//prints process statuses on console given the envelope message data
int CCI_printProcessStatuses (char* raw_data, MsgEnv* send_env)
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

	CCI_print( "PID | STATUS                | PRIORITY\n" );
    for (i=0;i<num_processes;i++)
    {
        params[0] = &(*data++);
        params[1] = NULL;
        rtx_sprintf(str, "  %d   ", params);
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
        rtx_sprintf(str, " %d\n", params);
        CCI_print(str);
    }
    return CODE_SUCCESS;
}

//prints trace buffers on console given the envelope message data
int CCI_printTraceBuffers (char* data, MsgEnv* send_env)
{
    if (data == NULL)
        return ERROR_NULL_ARG;
    int i;
    CHAR str[100]; 
    void * params [11];

    ipc_trace_t *send_dump = (ipc_trace_t *) data;
    ipc_trace_t *recv_dump = send_dump + IPC_MESSAGE_TRACE_HISTORY_SIZE; 
 
    CCI_print("MESSAGE TRACE BUFFERS\n\n"
              " Send Trace                   || Receive Trace\n"
              " Dest |Sender|Message|  Time  || Dest |Sender|Message|  Time\n"
              " PID  |PID   |Type   |        || PID  |PID   |Type   |\n"
              "--------------------------------------------------------------\n");
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
            rtx_sprintf(str, "   %2u |   %2u |   %3d | %6u\n", params);
            CCI_print (str);
        }
        else
        {
            CCI_print("      |      |       |       \n");
        }
    }        
    
    CCI_print("\n");

    return CODE_SUCCESS;
}

int CCI_setWallClock (MsgEnv *send_env, msg_env_queue_t *msgQ, char* newTime)
{
    int status;
    rtx_strcpy(send_env->msg, newTime, 1024);
    send_env->msg_type = CLOCK_SET;
    k_send_message(PROCESS_WALLCLOCK_PID, send_env);

    while (1)
    {
        MsgEnv *env = k_receive_message();
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

    k_send_message(PROCESS_WALLCLOCK_PID,send_env);

    while (1)
    {
        MsgEnv *env = k_receive_message();
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

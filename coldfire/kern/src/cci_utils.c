#include "cci_util.h"
#include "rtx.h"
#include "k_primitives.h"
#include "user_processes.h"
#include "utils.h"
#include "wallclock.h"
#include "k_globals.h"
#include "trace.h"
#include "msg_env_queue.h"

void wait_for_wallclock()
{
    while (1)
    {
        MsgEnv *env = receive_message();
        if (env->msg_type == CLOCK_RET)
        {
            return;
        }
        msg_env_queue_enqueue(msg_queue, env);
    }
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

	print_ack( "PID | STATUS                | PRIORITY\r\n" , print_env, msg_queue);
    for (i=0;i<num_processes;i++)
    {
        params[0] = &(*data++);
        params[1] = NULL;
        rtx_sprintf(str, "%4d   ", params);
        print_ack(str, print_env, msg_queue);
        switch(*data)
        {
            case P_READY:
                print_ack("ready                  ", print_env, msg_queue);
                break;
            case P_EXECUTING:
                print_ack("executing              ", print_env, msg_queue);
                break;
            case P_BLOCKED_ON_ENV_REQUEST:
                print_ack("blocked on env request ", print_env, msg_queue);
                break;
            case P_BLOCKED_ON_RECEIVE:
                print_ack("blocked on receive     ", print_env, msg_queue);
                break;
            default :
                print_ack("                       ", print_env, msg_queue);
                break;
        }
        data++;

        params[0] = &(*data++);
        params[1] = NULL;
        rtx_sprintf(str, "%2d\r\n", params);
        print_ack(str, print_env, msg_queue);
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
 
    print_ack("MESSAGE TRACE BUFFERS\r\n\r\n"
              " Send Trace                   || Receive Trace\r\n"
              " Dest |Sender|Message|  Time  || Dest |Sender|Message|  Time\r\n"
              " PID  |PID   |Type   |        || PID  |PID   |Type   |\r\n"
              "--------------------------------------------------------------\r\n",
              print_env, msg_queue);
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
            print_ack(str,print_env,msg_queue);
        }
        else if (recv_dump[i].time_stamp != MAX_UINT32)
        {
            print_ack("      |      |       |       ||", print_env, msg_queue);
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
            print_ack (str, print_env, msg_queue);
        }
        else
        {
            print_ack("      |      |       |       \r\n", print_env, msg_queue);
        }
    }        
    
    print_ack("\r\n", print_env, msg_queue);

    return CODE_SUCCESS;
}

int CCI_setWallClock (MsgEnv *send_env, char* newTime)
{
    rtx_strcpy(send_env->msg, newTime, 1024);
    send_env->msg_type = CLOCK_SET;
    int ret = send_message(WALLCLOCK_PID, send_env);
    if (ret == CODE_SUCCESS)
    {
        wait_for_wallclock();
    }
    return ret;
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

    int ret = send_message(WALLCLOCK_PID, send_env);
    if (ret == CODE_SUCCESS)
    {
        wait_for_wallclock();
    }
}


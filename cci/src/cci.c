#include "cci.h"
#include "cci_util.h"
#include "rtx.h"
#include "processes.h"
#include "msg_env_queue.h"
#include <string.h>
#include <stdio.h> 
#include <stdarg.h>

#define WAKEUP_CODE 123
#define ONE_SECOND_DELAY 10

#define SAVE_CURSOR "\033[s"
#define RESTORE_CURSOR "\033[u"
#define MOVE_CURSOR "\033[0;72H"
#define CLOCK_FORMAT "%02d:%02d:%02d"
//                    00:00:00
#define EMPTY_CLOCK  "        "

msg_env_queue_t *messageQueue;
MsgEnv *send_env, *timeout_env, *receive_env, *status_env, *proc_a_env;

/** Adaption of printf for the CCI console using variable arguments **/
int CCI_printf (const char* format, ...)
{
    if (format == NULL)
        return ERROR_NULL_ARG;
    va_list args;
    int status;

    va_start (args, format);
    vsprintf(send_env->msg, format, args);
    va_end (args);

    //Send formatted string to CRT
    status = send_console_chars(send_env);
    if (CODE_SUCCESS == status)
    {
        while (1)
        {
            MsgEnv *env = receive_message();
            if (env->msg_type == DISPLAY_ACK)
            {
                break;
            }
            //store envelopes not used in local queue for CCI main loop
            else
            {
                msg_env_queue_enqueue(messageQueue, env);
            }
        }
    }
    return status;    
}

/** CCI entry point and main loop **/
void start_cci()
{
    // initialise 
    uint32_t clock_time = 0;
    char clock_display_en = 0; //clock not displayed by default
    uint32_t status;

    messageQueue = msg_env_queue_create();
    send_env = request_msg_env();
    timeout_env = request_msg_env();
    receive_env = request_msg_env();
    status_env = request_msg_env();
    proc_a_env = request_msg_env();

    status = get_console_chars(receive_env);
    if (status != CODE_SUCCESS)
    {
        CCI_printf("get_console_chars failed with status %d\n",status);
    }
    status = request_delay ( ONE_SECOND_DELAY, WAKEUP_CODE, timeout_env); 
    if (status != CODE_SUCCESS)
    {
        CCI_printf("request_delay failed with status %d\n",status);
    }

    //print CCI prompt
    CCI_printf("CCI: ");

    while (1)
    {
        MsgEnv* env;
        //First check for messages received but not processed by CCI_printf
        if( msg_env_queue_is_empty(messageQueue))
        {
            env = receive_message(); 
        }
        else
        {
            env = msg_env_queue_dequeue(messageQueue);
        }

        //envelope from timing services
        if (env->msg_type == WAKEUP_CODE)
        {
            status = request_delay ( ONE_SECOND_DELAY, WAKEUP_CODE, timeout_env);
            if (status != CODE_SUCCESS)
            {
                CCI_printf("request_delay failed with status %d\n",status);
            }
            clock_time = (clock_time+1)%86400; //86400 = 24hrs in secs
            if (clock_display_en)
            {
                CCI_printf( SAVE_CURSOR MOVE_CURSOR CLOCK_FORMAT RESTORE_CURSOR,
                        clock_time/3600,(clock_time%3600)/60, clock_time%60);
            }
        }
        //envelope with characters from console
        else if (env->msg_type == CONSOLE_INPUT)
        {
            char cmd [3];
            if (sscanf(env->msg,"%s", cmd)==1)
            {
                //send empty envelope to process A. should only do so once.
                if (strcmp(cmd,"s") == 0) 
                {
                    if ( proc_a_env != NULL )
                    {
                        status = send_message (PROCESS_A_PID, proc_a_env);
                        if (status != CODE_SUCCESS)
                        {
                            CCI_printf("send_message failed with status %d\n",status);
                        }
                    }
                    else
                    {
                        CCI_printf("Process A has already been started.\n");
                    }
                }
                //displays process statuses
                else if (strcmp(cmd,"ps") == 0) 
                {
                    status = request_process_status(status_env);
                    if (status != CODE_SUCCESS)
                    {
                        CCI_printf("request_process_status failed with status %d\n",status);
                    }
                    else
                    {
                        status = CCI_printProcessStatuses(status_env->msg);
                        if (status != CODE_SUCCESS)
                            CCI_printf("CCI_printProcessStatuses failed with status %d\n",status);
                    }
                }
                //show clock
                else if (strcmp(cmd,"cd") == 0) 
                {
                    clock_display_en = 1;
                }
                //hide clock
                else if (strcmp(cmd,"ct") == 0)  
                {
                    CCI_printf( SAVE_CURSOR MOVE_CURSOR EMPTY_CLOCK RESTORE_CURSOR);
                    clock_display_en = 0;
                }
                //show send/receive trace buffers
                else if (strcmp(cmd,"b") == 0) 
                {
                    status = get_trace_buffers (status_env);
                    if (status != CODE_SUCCESS)
                        CCI_printf("get_trace_buffers failed with status %d\n",status);
                    status = CCI_printTraceBuffers (status_env->msg);
                    if (status != CODE_SUCCESS)
                        CCI_printf("CCI_printTraceBuffers failed with status %d\n",status);
                }
                //terminate RTX
                else if (strcmp(cmd,"t") == 0) 
                {
                    terminate();
                }
                //change process priority
                else if (strcmp(cmd,"n") == 0) 
                {
                    int priority, pid; 
                    if (sscanf(env->msg, "%*s %d %d", &priority, &pid)!=2)
                    {
                        CCI_printf("Usageasd: n <priority> <processID>\n");
                    }
                    else
                    {
                        CCI_printf("The priority submitted is %d and the pid is %d \n", priority, pid);
                        status = change_priority(priority, pid);
                        if (status == ERROR_ILLEGAL_ARG)
                            CCI_printf("Usage: n <priority> <processID>\n");
                        if (status != CODE_SUCCESS)
                            CCI_printf("CCI_setNewPriority failed with status %d\n",status);
                    }
                }
                //set clock
                else if (strcmp(cmd,"c") == 0) 
                {
                    char newTime [9];
                    if (sscanf(env->msg, "%*s %s",newTime) != 1)
                    {
                        CCI_printf("c\n"
                                   "Sets the console clock.\n"
                                   "Usage: c <hh:mm:ss>\n");
                    }
                    else
                    {
                        status = CCI_setClock(newTime, &clock_time);
                        if (status == ERROR_ILLEGAL_ARG)
                        {
                            CCI_printf("c\n"
                                       "Sets the console clock.\n"
                                       "Usage: c <hh:mm:ss>\n");
                        }
                        else if (status != CODE_SUCCESS)
                        {
                            CCI_printf("CCI_setClock failed with status %d\n",status);
                        }
                    }
                }
                else
                {
                    CCI_printf("Invalid command '%s'\n", cmd);
                }
            }//end if (sscanf(env->msg,"%s",cmd)==1)
            else
            {
                CCI_printf("Please enter a command.\n");
            }

            CCI_printf("CCI: ");
            get_console_chars(receive_env);
        }//end if env->msg_type == CONSOLE_INPUT
    }
}


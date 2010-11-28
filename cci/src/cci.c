#include "cci.h"
#include "cci_util.h"
#include "wallclock.h"
#include "rtx.h"
#include "processes.h"
#include "msg_env_queue.h"
#include "rtx_util.h"
#include <string.h>
#include <stdio.h> 

/** CCI entry point and main loop **/
void start_cci()
{
    // initialise 
    uint32_t status;

    msg_env_queue_t *msgQ = msg_env_queue_create();
    MsgEnv *send_env, *receive_env, *status_env, *proc_a_env;
    send_env = request_msg_env();
    receive_env = request_msg_env();
    status_env = request_msg_env();
    proc_a_env = request_msg_env();

    status = get_console_chars(receive_env);
    if (status != CODE_SUCCESS)
    {
        RTX_printf(send_env, msgQ, "get_console_chars failed with status %d\n",status);
    }

    //print CCI prompt
    RTX_printf(send_env, msgQ, "CCI: ");

    while (1)
    {
        MsgEnv* env;
        //First check for messages received but not processed by RTX_printf
        if( msg_env_queue_is_empty(msgQ))
        {
            env = receive_message(); 
        }
        else
        {
            env = msg_env_queue_dequeue(msgQ);
        }

        //envelope with characters from console
        if (env->msg_type == CONSOLE_INPUT)
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
                            RTX_printf(send_env, msgQ, "send_message failed with status %d\n",status);
                        }
                        proc_a_env = NULL;
                    }
                    else
                    {
                        RTX_printf(send_env, msgQ, "Process A has already been started.\n");
                    }
                }
                //displays process statuses
                else if (strcmp(cmd,"ps") == 0) 
                {
                    status = request_process_status(status_env);
                    if (status != CODE_SUCCESS)
                    {
                        RTX_printf(send_env, msgQ, "request_process_status failed with status %d\n",status);
                    }
                    else
                    {
                        status = CCI_printProcessStatuses(status_env->msg,
                                 send_env, msgQ);
                        if (status != CODE_SUCCESS)
                            RTX_printf(send_env, msgQ, "CCI_printProcessStatuses failed with status %d\n",status);
                    }
                }
                //show clock
                else if (strcmp(cmd,"cd") == 0) 
                {
                    displayWallClock (1);
                }
                //hide clock
                else if (strcmp(cmd,"ct") == 0)  
                {
                    displayWallClock (0);
                }
                //show send/receive trace buffers
                else if (strcmp(cmd,"b") == 0) 
                {
                    status = get_trace_buffers (status_env);
                    if (status != CODE_SUCCESS)
                        RTX_printf(send_env, msgQ, "get_trace_buffers failed with status %d\n",status);
                    status = CCI_printTraceBuffers (status_env->msg, send_env,
                             msgQ);
                    if (status != CODE_SUCCESS)
                        RTX_printf(send_env, msgQ, "CCI_printTraceBuffers failed with status %d\n",status);
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
                        RTX_printf(send_env, msgQ, "Usageasd: n <priority> <processID>\n");
                    }
                    else
                    {
                        status = change_priority(priority, pid);
                        if (status == ERROR_ILLEGAL_ARG)
                            RTX_printf(send_env, msgQ, "Usage: n <priority> <processID>\n");
                        if (status != CODE_SUCCESS)
                            RTX_printf(send_env, msgQ, "CCI_setNewPriority failed with status %d\n",status);
                    }
                }
                //set clock
                else if (strcmp(cmd,"c") == 0) 
                {
                    char newTime [9];
                    if (sscanf(env->msg, "%*s %s",newTime) != 1)
                    {
                        RTX_printf(send_env, msgQ, "c\n"
                                   "Sets the console clock.\n"
                                   "Usage: c <hh:mm:ss>\n");
                    }
                    else
                    {
                        status = setWallClock (newTime);
                        if (status == ERROR_ILLEGAL_ARG)
                        {
                            RTX_printf(send_env, msgQ, "c\n"
                                       "Sets the console clock.\n"
                                       "Usage: c <hh:mm:ss>\n");
                        }
                        else if (status != CODE_SUCCESS)
                        {
                            RTX_printf(send_env, msgQ, "CCI_setClock failed with status %d\n",status);
                        }
                    }
                }
                else
                {
                    RTX_printf(send_env, msgQ, "Invalid command '%s'\n", cmd);
                }
            }//end if (sscanf(env->msg,"%s",cmd)==1)
            else
            {
                RTX_printf(send_env, msgQ, "Please enter a command.\n");
            }

            RTX_printf(send_env, msgQ, "CCI: ");
            get_console_chars(receive_env);
        }//end if env->msg_type == CONSOLE_INPUT
    }
}


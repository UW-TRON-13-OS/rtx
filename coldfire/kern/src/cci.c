#include "cci.h"
#include "cci_util.h"
#include "rtx.h"
#include "k_primitives.h"
#include "processes.h"
#include "msg_env_queue.h"
#include "utils.h"

void cci_intro(MsgEnv *send_env)
{
	send_env->msg = "Welcome to AwesomeOS\n"
                    "==========================\n";
    k_send_console_chars(send_env);
}

/** CCI entry point and main loop **/
void start_cci()
{
    // initialise 
    uint32_t status;

    msg_env_queue_t *msgQ = msg_env_queue_create();
    MsgEnv *send_env, *status_env, *proc_a_env;
    send_env = k_request_msg_env();
    status_env = k_request_msg_env();
    proc_a_env = k_request_msg_env();
    char str_a[100], format_a[100];
    char *str, *format;
    str = (char *)str_a;
    format = (char *) format_a;
    void * params [11];

    // Print the introduction
    cci_intro(send_env);

    //print CCI prompt
	send_env->msg = "CCI: ";
    k_send_console_chars(send_env);

    while (1)
    {
        MsgEnv* env;
        //First check for messages received but processed by clock fcns 
        if( msg_env_queue_is_empty(msgQ))
        {
            env = k_receive_message(); 
        }
        else
        {
            env = msg_env_queue_dequeue(msgQ);
        }

        //envelope with characters from console
        if (env->msg_type == CONSOLE_INPUT)
        {
            char cmd [3];
            rtx_strtok (env->msg, cmd, " \t");
            if ( *cmd != '\0')
            {
                //send empty envelope to process A. should only do so once.
                if (rtx_strcmp(cmd,"s") == 0) 
                {
                    if ( proc_a_env != NULL )
                    {
                        status = k_send_message (PROCESS_A_PID, proc_a_env);
                        if (status != CODE_SUCCESS)
                        {
							format = "Error: could not jumpstart process A."
                                     "errno %d\n";
							params[0] = &status;
                            params[1] = NULL;
							send_env->msg = rtx_sprintf(str, format, params);
                            k_send_console_chars(send_env);
                        }
                        proc_a_env = NULL;
                    }
                    else
                    {
						send_env->msg = "Process A has already been started.\n";
                        k_send_console_chars(send_env);
                    }
                }
                //displays process statuses
                else if (rtx_strcmp(cmd,"ps") == 0) 
                {
                    status = request_process_status(status_env);
                    if (status != CODE_SUCCESS)
                    {
						format = 	"Error: could not get the process statues. "
										"errno %d\n";
						params[0] = &status;
                        params[1] = NULL;
						send_env->msg = rtx_sprintf(str, format, params);
                        k_send_console_chars(send_env);
                    }
                    else
                    {
                        status = CCI_printProcessStatuses(status_env->msg, send_env);
                        if (status != CODE_SUCCESS)
						{
							format = 	"CCI_printProcessStatuses failed with "
											"status %d\n";
                            params[0] = &status;
                            params[1] = NULL;
							send_env->msg = rtx_sprintf(str, format, params);
                            k_send_console_chars(send_env);
                        }
                    }
                }
                //show clock
                else if (rtx_strcmp(cmd,"cd") == 0) 
                {
                    CCI_displayWallClock (send_env, msgQ, 1);
                }
                //hide clock
                else if (rtx_strcmp(cmd,"ct") == 0)  
                {
                    CCI_displayWallClock (send_env, msgQ, 0);
                }
                //show send/receive trace buffers
                else if (rtx_strcmp(cmd,"b") == 0) 
                {
                    status = get_trace_buffers (status_env);
                    if (status != CODE_SUCCESS)
					{
						format = "get_trace_buffers failed with status %d\n";
                        params[0] = &status;
                        params[1] = NULL;
						send_env->msg = rtx_sprintf(str, format, params);
                        k_send_console_chars(send_env);
                    }
                    status = CCI_printTraceBuffers (status_env->msg, send_env);
                    if (status != CODE_SUCCESS)
					{					
						format = "CCI_printTraceBuffers failed with status %d\n";
                        params[0] = &status;
                        params[1] = NULL;
						send_env->msg = rtx_sprintf(str, format, params);
                        k_send_console_chars(send_env);
                    }
                }
                //terminate RTX
                else if (rtx_strcmp(cmd,"t") == 0)
                {
                    send_env->msg = "Sayonara\n";
                    k_send_console_chars(send_env);
                    terminate();
                }
                //change process priority
                else if (rtx_strcmp(cmd,"n") == 0) 
                {
                    int priority, pid, ret; 
                    char priorityStr[6], pidStr[6];
                    rtx_strtok ( NULL, priorityStr, " \t" );
                    rtx_strtok ( NULL, pidStr, " \t" );
                    ret = rtx_atoi ( priorityStr, &priority );
                    ret += rtx_atoi ( pidStr, &pid );

                    if ( ret < 2 )
                    {
						send_env->msg = "Error Bad command format: "
                                        "Usage: n <priority> <processID>\n";
                        k_send_console_chars(send_env);
                    }
                    else
                    {
                        status = change_priority(priority, pid);
                        if (status == ERROR_ILLEGAL_ARG)
						{
							send_env->msg = "Usage: n <priority> <processID>\n"
											"priority must be 0-2 and "
											"processID must be a user process other "
											"than the null process\n";
                            k_send_console_chars(send_env);
                        }
                        else if (status != CODE_SUCCESS)
						{
							format = "CCI_setNewPriority failed with status %d\n";
                            params[0] = &status;
                            params[1] = NULL;
							send_env->msg = rtx_sprintf(str, format, params);
                            k_send_console_chars(send_env);
                        }
                    }
                }
                //set clock
                else if (rtx_strcmp(cmd,"c") == 0) 
                {
                    char newTime [9];
                    rtx_strtok (NULL, newTime, " \t");
                    if ( *newTime == '\0' )
                    {
						send_env->msg = "c\n"
									"Sets the console clock.\n"
									"Usage: c <hh:mm:ss>\n";
                        k_send_console_chars(send_env);
                    }
                    else
                    {
                        status = CCI_setWallClock (send_env, msgQ,newTime);
                        if (status == ERROR_ILLEGAL_ARG)
                        {
                            send_env->msg = "c\n"
                                       "Sets the console clock (24h).\n"
                                       "Usage: c <hh:mm:ss>\n"
                                       "hh must be 00-23\n"
                                       "mm must be 00-59\n"
                                       "ss must be 00-59\n";
                            k_send_console_chars(send_env);
                        }
                        else if (status != CODE_SUCCESS)
                        {
							format = "CCI_setClock failed with status %d\n";
                            params[0] = &status;
                            params[1] = NULL;
							send_env->msg = rtx_sprintf(str, format, params);
                            k_send_console_chars(send_env);
                        }
                    }
                }
                else
                {
					format = "Invalid command '%s'\n";
					params[0] = &cmd;
                    params[1] = NULL;
					send_env->msg = rtx_sprintf(str, format, params);
                    k_send_console_chars(send_env);
                }
            }//end if (*cmd != '\0')
            else
            {
				send_env->msg = "Please enter a command.\n";
                k_send_console_chars(send_env);
            }
			
			send_env->msg = "CCI: ";
            k_send_console_chars(send_env);
        }//end if env->msg_type == CONSOLE_INPUT
    }
}

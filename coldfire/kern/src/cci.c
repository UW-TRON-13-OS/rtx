#include "cci.h"
#include "cci_util.h"
#include "rtx.h"
#include "k_primitives.h"
#include "processes.h"
#include "msg_env_queue.h"
#include "utils.h"

void cci_intro(MsgEnv *send_env)
{
	CCI_print( "Welcome to P.OS.\r\n"
               "==========================\r\n");
}

/** CCI entry point and main loop **/
void start_cci()
{
    // initialise 
    uint32_t status;

    msg_env_queue_t *msgQ = msg_env_queue_create();
    MsgEnv *send_env, *status_env, *proc_a_env;
    status_env = request_msg_env();
    proc_a_env = request_msg_env();
    char str[100];
    void * params [11];

    // Print the introduction
    cci_intro(send_env);

    //print CCI prompt
    CCI_print("CCI: ");

    while (1)
    {
        MsgEnv* env;
        //First check for messages received but processed by clock fcns 
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
            char cmd [100];
            rtx_strtok (env->msg, cmd, " \t");
            if ( *cmd != '\0')
            {
                //send empty envelope to process A. should only do so once.
                if (rtx_strcmp(cmd,"s") == 0) 
                {
                    if ( proc_a_env != NULL )
                    {
                        status = send_message (PROCESS_A_PID, proc_a_env);
                        if (status != CODE_SUCCESS)
                        {
							params[0] = &status;
                            params[1] = NULL;
							rtx_sprintf(str, "Error: could not jumpstart process A."
                                              "errno %d\r\n", params);
                            CCI_print(str);
                        }
                        proc_a_env = NULL;
                    }
                    else
                    {
						CCI_print("Process A has already been started.\r\n");
                    }
                }
                //displays process statuses
                else if (rtx_strcmp(cmd,"ps") == 0) 
                {
                    status = request_process_status(status_env);
                    if (status != CODE_SUCCESS)
                    {
						params[0] = &status;
                        params[1] = NULL;
                        rtx_sprintf(str, "Error: could not get the process "
                                         "statues. errno %d\r\n", params);
                        CCI_print(str);
                    }
                    else
                    {
                        status = CCI_printProcessStatuses(status_env->msg,
                                                          send_env);
                        if (status != CODE_SUCCESS)
						{
                            params[0] = &status;
                            params[1] = NULL;
                            rtx_sprintf(str,"CCI_printProcessStatuses failed "
                                            "with status %d\r\n", params);
                            CCI_print(str);
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
                        params[0] = &status;
                        params[1] = NULL;
                        rtx_sprintf(str, "get_trace_buffers failed with status %d\r\n", params);
                        CCI_print(str);
                    }
                    status = CCI_printTraceBuffers (status_env->msg, send_env);
                    if (status != CODE_SUCCESS)
					{					
                        params[0] = &status;
                        params[1] = NULL;
                        rtx_sprintf(str, "CCI_printTraceBuffers failed with status %d\r\n", params);
                        CCI_print(str);
                    }
                }
                //terminate RTX
                else if (rtx_strcmp(cmd,"t") == 0)
                {
                    CCI_print("Bye");
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
						CCI_print ("Error Bad command format: "
                                   "Usage: n <priority> <processID>\r\n");
                    }
                    else
                    {
                        status = change_priority(priority, pid);
                        if (status == ERROR_ILLEGAL_ARG)
						{
                            CCI_print("Usage: n <priority> <processID>\r\n"
                                      "priority must be 0-2 and "
                                      "processID must be a user process other "
                                      "than the null process\r\n");
                        }
                        else if (status != CODE_SUCCESS)
						{
                            params[0] = &status;
                            params[1] = NULL;
							rtx_sprintf(str, "CCI_setNewPriority failed with status %d\r\n", params);
                            CCI_print(str);
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
						CCI_print( "c\r\n"
								   "Sets the console clock.\r\n"
								   "Usage: c <hh:mm:ss>\r\n");
                    }
                    else
                    {
                        status = CCI_setWallClock (send_env, msgQ,newTime);
                        if (status == ERROR_ILLEGAL_ARG)
                        {
                            CCI_print( "c\r\n"
                                       "Sets the console clock (24h).\r\n"
                                       "Usage: c <hh:mm:ss>\r\n"
                                       "hh must be 00-23\r\n"
                                       "mm must be 00-59\r\n"
                                       "ss must be 00-59\r\n" );
                        }
                        else if (status != CODE_SUCCESS)
                        {
                            params[0] = &status;
                            params[1] = NULL;
							rtx_sprintf(str, "CCI_setClock failed with status %d\r\n", params);
                            CCI_print(str);
                        }
                    }
                }
                else
                {
					params[0] = &cmd;
                    params[1] = NULL;
                    rtx_sprintf(str, "Invalid command '%s'\r\n", params);
                    CCI_print(str);
                }
            }//end if (*cmd != '\0')
            else
            {
				CCI_print( "Please enter a command.\r\n" );
            }
			
			CCI_print( "CCI: " );
        }//end if env->msg_type == CONSOLE_INPUT
    }
}

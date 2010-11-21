#include "cci.h"
#include "cci_util.h"
#include "rtx.h"
#include "processes.h"
#include <string.h>
#include <stdio.h> 
#include <stdarg.h>

#define WAKEUP_CODE 123

#define SAVE_CURSOR "\033[s"
#define RESTORE_CURSOR "\033[u"
#define MOVE_CURSOR "\033[0;72H"
#define CLOCK_FORMAT "%02d:%02d:%02d"
//                    00:00:00
#define EMPTY_CLOCK  "        "

MsgEnv *send_env, *timeout_env, *receive_env, *status_env;
int CCI_printf (const char* format, ...)
{
    if (format == NULL)
        return ERROR_NULL_ARG;
    va_list args;
    int status;

    va_start (args, format);
    vsprintf(send_env->msg, format, args);
    va_end (args);
    status = send_console_chars(send_env);
    while (1)
    {
        MsgEnv *env = receive_message();
        if (env->msg_type == DISPLAY_ACK)
        {
            break;
        }
        else
        {
            printf("Found message type %d when waiting for DISPLAY ACK\n", env->msg_type);
            //send_message(
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

    send_env = request_msg_env();
    timeout_env = request_msg_env();
    receive_env = request_msg_env();
    status_env = request_msg_env();

    status = get_console_chars(receive_env);
    if (status != CODE_SUCCESS)
        CCI_printf("get_console_chars failed with status %d\n",status);
    //we can add a periodic delay fcn instead
    status = request_delay ( 10, WAKEUP_CODE, timeout_env); //one second delay 
    if (status != CODE_SUCCESS)
        CCI_printf("request_delay failed with status %d\n",status);

    //print CCI prompt
    CCI_printf("CCI: ");

    while (1)
    {
        MsgEnv *env = receive_message(); 
        //envelope from timing services
        if (env->msg_type == WAKEUP_CODE)
        {
            status = request_delay ( 10, WAKEUP_CODE, timeout_env);
            if (status != CODE_SUCCESS)
                CCI_printf("request_delay failed with status %d\n",status);
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
            //Handle the different CCI commands
            char* cmd = strtok(env->msg," \t");
            //send empty envelope to process A
            if (strcmp(cmd,"s") == 0) 
            {
                    // TODO keep track of whether we have already started process A
                    MsgEnv* tmpEnv = request_msg_env();
                    status = send_message (PROCESS_A_PID, tmpEnv);
                    if (status != CODE_SUCCESS)
                        CCI_printf("send_message failed with status %d\n",status);
            }
            //displays process statuses
            else if (strcmp(cmd,"ps") == 0) 
            {
                status = request_process_status(status_env);
                if (status != CODE_SUCCESS)
                    CCI_printf("request_process_status failed with status %d\n",status);
                status = CCI_printProcessStatuses(status_env->msg);
                if (status != CODE_SUCCESS)
                    CCI_printf("CCI_printProcessStatuses failed with status %d\n",status);
            }
            //set clock
            else if (strcmp(cmd,"c") == 0) 
            {
                char* param = strtok(NULL," \t");
                if (strtok(NULL," \t") != NULL)
                    CCI_printf("c\n"
                           "Sets the console clock.\n"
                           "Usage: c <hh:mm:ss>\n");
                status = CCI_setClock(param, &clock_time);
                if (status == ERROR_ILLEGAL_ARG)
                    CCI_printf("c\n"
                           "Sets the console clock.\n"
                           "Usage: c <hh:mm:ss>\n");
                else if (status != CODE_SUCCESS)
                    CCI_printf("CCI_setClock failed with status %d\n",status);
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
                char* rem = strtok(NULL,"");
                status = CCI_setNewPriority(rem);
                if (status == ERROR_ILLEGAL_ARG)
                    CCI_printf("Usage: n <priority> <processID>\n");
                if (status != CODE_SUCCESS)
                    CCI_printf("CCI_setNewPriority failed with status %d\n",status);
            }
            else
            {
                    CCI_printf("Invalid command '%s'\n", cmd);
            }
            CCI_printf("CCI: ");
            get_console_chars(receive_env);
        }
    }
}


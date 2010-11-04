#include "cci.h"
#include "cci_util.h"
#include "rtx.h"
#include "processes.h"
#include <stdio.h> //for printf. TODO: rmv later
#include <string.h> //for strcmp. TODO: rmv later
#include <stdlib.h> //for atoi. TODO: rmv later

#define WAKEUP_CODE 123

/** CCI entry point and main loop **/
void start_cci()
{
    // initialise 
    uint32_t clock_time = 0;
    char clock_display_en = 0; //clock not displayed by default
    uint32_t status;
    MsgEnv* env = request_msg_env();

    status = get_console_chars(env);
    if (status != CODE_SUCCESS)
        printf("get_console_chars failed with status %d\n",status);
    env = request_msg_env();
    //we can add a periodic delay fcn instead
    //status = request_delay ( 10, WAKEUP_CODE, env); //one second delay 
    //if (status != CODE_SUCCESS)
     //   printf("request_delay failed with status %d\n",status);
    //print CCI prompt
    CCI_print("CCI: ");
    fflush(stdout);

    while (1)
    {
        env = receive_message(); 
        //envelope from timing services
        if (env->msg_type == WAKEUP_CODE)
        {
            status = request_delay ( 10, WAKEUP_CODE, env);
            if (status != CODE_SUCCESS)
                printf("request_delay failed with status %d\n",status);
            clock_time = (clock_time+1)%86400; //86400 = 24hrs in secs
            if (clock_display_en)
            {
                //show clock somehow...TODO? change this later
                printf("%d:%d:%d",clock_time/3600,(clock_time%3600)/60,
                                  clock_time%60);
            }
            else
            {
                //don't show clock somehow 
            }
        }
        //envelope with characters from console
        else if (env->msg_type == CONSOLE_INPUT)
        {
            //Handle the different CCI commands
            char cmd [3];
            char rem [20];
            splitFirstWord (env->msg, cmd, rem);
            //send empty envelope to process A
            if (strcmp(cmd,"s") == 0) //TODO rmv strcmp
            {
                    MsgEnv* tmpEnv = request_msg_env();
                    status = send_message (PROCESS_A_PID, tmpEnv);
                    if (status != CODE_SUCCESS)
                        printf("send_message failed with status %d\n",status);
            }
            //displays process statuses
            else if (strcmp(cmd,"ps") == 0) //TODO rmv strcmp
            {
                MsgEnv* tmpEnv = request_msg_env();
                status = request_process_status(tmpEnv);
                if (status != CODE_SUCCESS)
                    printf("request_process_status failed with status %d\n",status);
                status = CCI_printProcessStatuses(tmpEnv->msg);
                if (status != CODE_SUCCESS)
                    printf("CCI_printProcessStatuses failed with status %d\n",status);
                status = release_msg_env(tmpEnv);
                if (status != CODE_SUCCESS)
                    printf("release_msg_env failed with status %d\n",status);
            }
            //set clock
            else if (strcmp(cmd,"c") == 0) //TODO rmv strcmp
            {
                status = CCI_setClock(rem, &clock_time);
                if (status == ERROR_ILLEGAL_ARG)
                    printf("c\n"
                           "Sets the console clock.\n"
                           "Usage: c <hh:mm:ss>\n");
                else if (status != CODE_SUCCESS)
                    printf("CCI_setClock failed with status %d\n",status);
            }
            //show clock
            else if (strcmp(cmd,"cd") == 0) //TODO rmv strcmp
            {
                clock_display_en = 1;
            }
            //hide clock
            else if (strcmp(cmd,"ct") == 0) //TODO rmv strcmp 
            {
                clock_display_en = 0;
            }
            //show send/receive trace buffers
            else if (strcmp(cmd,"b") == 0) //TODO rmv strcmp
            {
                MsgEnv* tmpEnv = request_msg_env();
                status = get_trace_buffers (tmpEnv);
                if (status != CODE_SUCCESS)
                    printf("get_trace_buffers failed with status %d\n",status);
                status = CCI_printTraceBuffers (tmpEnv->msg);
                if (status != CODE_SUCCESS)
                    printf("CCI_printTraceBuffers failed with status %d\n",status);
                status = release_msg_env(tmpEnv);
                if (status != CODE_SUCCESS)
                    printf("release_msg_env failed with status %d\n",status);
            }
            //terminate RTX
            else if (strcmp(cmd,"t") == 0) //TODO rmv strcmp
            {
                terminate();
            }
            //change process priority
            else if (strcmp(cmd,"n") == 0) //TODO rmv strcmp
            {
                status = CCI_setNewPriority(rem);
                if (status == ERROR_ILLEGAL_ARG)
                    printf("n\n"
                           "Sets priority of a user process.\n"
                           "Usage: n <priority> <processID>\n");
                if (status != CODE_SUCCESS)
                    printf("CCI_setNewPriority failed with status %d\n",status);
            }
            else
            {
                    printf("Invalid command '%s'\n", cmd);
            }
            printf("CCI: ");
            fflush(stdout);
            get_console_chars(env);
        }
        //envelope from something else. discard
        else
        {
            release_msg_env (env);
        }
    }
}


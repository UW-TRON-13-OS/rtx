#include "cci.h"
#include "rtx.h"
#include "processes.h"
#include <stdio.h>
#include <string.h> //for strcmp. TODO: rmv later
#include <stdlib.h> //for atoi. TODO: rmv later

#define WAKEUP_CODE 123

/** FUNCTION DECLARATIONS **/
//gets and prints process statuses
int CCI_printProcessStatuses (char* data);

//sets the clock 
int CCI_setClock (char* timeParam, uint32_t* time);

//prints trace buffers on console given the envelope message data
int CCI_printTraceBuffers (char* data);

//set process priority based on params given provided
int CCI_setNewPriority (char* param);

//strcmp
int CCI_strcmp (char* str1, char* str2);

//converts string into int
int CCI_atoi (char* str);

//Splits input into the first word (retStr1) and the remainder (retStr2)
int splitFirstWord (char* input, char* retStr1, char* retStr2);

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
    printf("CCI: ");

    while (1)
    {
        printf("cci receiving a message\n");
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
            get_console_chars(env);
        }
        //envelope from something else. discard
        else
        {
            release_msg_env (env);
        }
    }
}

//prints process statuses on console given the envelope message data
int CCI_printProcessStatuses (char* raw_data)
{
    if (raw_data == NULL)
        return ERROR_NULL_ARG;
    int * data = (int *) raw_data;
    int num_processes = *data++;
    int i;
    printf ("PID | STATUS                | PRIORITY\n");
    for (i=0;i<num_processes;i++)
    {
        printf("  %d   ",*data++);
        switch(*data)
        {
            case P_READY:
                printf("ready                  ");
                break;
            case P_EXECUTING:
                printf("executing              ");
                break;
            case P_BLOCKED_ON_ENV_REQUEST:
                printf("blocked on env request ");
                break;
            case P_BLOCKED_ON_RECEIVE:
                printf("blocked on receive     ");
                break;
            default :
                printf("                       ");
                break;
        }
        *data++;
        printf(" %d\n",*data++);
    }
    return CODE_SUCCESS;
}

//sets the clock 
int CCI_setClock (char* timeParam, uint32_t* time)
{
    if (timeParam == NULL)
        return ERROR_NULL_ARG;
    if (timeParam[2] != ':' || timeParam[5] != ':' || timeParam[8] != ':')
        return ERROR_ILLEGAL_ARG;

    char hr_s [3];
    char min_s [3];
    char sec_s [3];
    int i, hr, min, sec;
    for (i=0;i<2;i++)
    {
        hr_s[i] =timeParam[i];
        min_s[i]=timeParam[3+i];
        sec_s[i]=timeParam[6+i];
    }
    hr_s[2]='\0';
    min_s[2]='\0';
    sec_s[2]='\0';

    hr = atoi(hr_s); // TODO rmv atoi
    min = atoi(min_s);
    sec = atoi(sec_s);
    if (hr>23 || min>59 || sec > 59)
        return ERROR_ILLEGAL_ARG;
    *time = hr*3600 + min*60 + sec;
    return CODE_SUCCESS;
}

//prints trace buffers on console given the envelope message data
int CCI_printTraceBuffers (char* data)
{
    if (data == NULL)
        return ERROR_NULL_ARG;
    int i;
    ipc_trace_t *send_dump = (ipc_trace_t *) data;
    ipc_trace_t *recv_dump = (ipc_trace_t *) &data[sizeof(ipc_trace_t) * 
                             IPC_MESSAGE_TRACE_HISTORY_SIZE];
 
    printf("MESSAGE TRACE BUFFERS\n"
            "-----------------------------\n"
            "Send trace buffer:\n");
    for (i=0;i<IPC_MESSAGE_TRACE_HISTORY_SIZE 
             && send_dump[i].time_stamp != 0;i++)
    {
        printf("  %2u | %u | %u | %2u | %llu\n",i+1,send_dump[i].dest_pid,
               send_dump[i].send_pid, send_dump[i].msg_type,
               send_dump[i].time_stamp);
    }        
    
    printf("\nReceive trace buffer:\n");
    for (i=0;i<IPC_MESSAGE_TRACE_HISTORY_SIZE 
             && recv_dump[i].time_stamp != 0;i++)
    {
        printf("  %2u | %u | %u | %2u | %llu\n",i+1,recv_dump[i].dest_pid,
               recv_dump[i].send_pid, recv_dump[i].msg_type,
               recv_dump[i].time_stamp);
    } 
    printf("\n");

    return CODE_SUCCESS;
}

//set process priority based on params given provided
int CCI_setNewPriority (char* param)
{
    if (param == NULL)
        return ERROR_NULL_ARG;

    char priorityStr [3];
    char pidStr [3];
    int priority, pid;
    splitFirstWord (param, priorityStr, pidStr);
    priority = atoi(priorityStr); //TODO rmv atoi
    pid = atoi(pidStr); //TODO rmv atoi
    printf("%s=%d %s=%d\n", priorityStr, priority, pidStr, pid);
    return change_priority(priority, pid);
}

//strcmp
int CCI_strcmp (char* str1, char* str2)
{
    int i, diff;
    do
    {
        diff = str1[i]-str2[i];
    }
    while (str1[i] != '\0' && str2[i] != '\0');

    return diff;
}

//converts string into int
int CCI_atoi (char* str)
{
    int sign = 1;
    int value = 0;

    if (*str == '+')
    {
        str++;
    }
    else if (*str == '-')
    {
        sign = -1;
        str++
    }

    while (*str != '\0')
    {
        value *= 10;
        value += (int)*str++ - (int)'0';
    }
    
    return value*sign;
}

//Splits input into the first word (retStr1) and the remainder (retStr2)
int splitFirstWord (char* input, char* retStr1, char* retStr2)
{
    if (input == NULL || retStr1 == NULL || retStr2 == NULL)
        return ERROR_NULL_ARG;
    else
    {
        int i = 0;
        char ch = input[0];
        //ignore leading whitespace
        while (ch == ' ' || ch == '\t')
        {
            i++;
            ch = input [i];
        }

        //get first word
        while (ch != '\0' && ch != '\t' && ch != ' ')
        {
            retStr1[i]=ch;
            i++;
            ch = input[i];
        }
        retStr1[i] = '\0';

        //get remainder
        int j = 0;
        if (ch == '\0')
            retStr2[0]='\0';
        else
        {
            while (ch == ' ' || ch == '\t')
            {
                i++;
                ch = input [i];
            }

            while (ch != '\0')
            {
                retStr2[j]=ch;
                i++;
                j++;
                ch = input[i];
            }
            retStr2[j]='\0';
        }
    }
    return CODE_SUCCESS;
}


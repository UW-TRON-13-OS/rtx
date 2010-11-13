#include "cci_util.h"
#include "rtx.h"
#include <stdio.h> //for printf. TODO: rmv later
#include <string.h>
#include <stdlib.h>

#include <stdlib.h>

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
        data++;
        printf(" %d\n",*data++);
    }
    return CODE_SUCCESS;
}

//sets the clock 
int CCI_setClock (char* timeParam, uint32_t* time)
{
    if (timeParam == NULL)
        return ERROR_NULL_ARG;
    if (timeParam[2] != ':' || timeParam[5] != ':')
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

    hr = atoi(hr_s); 
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
    ipc_trace_t *recv_dump = send_dump + IPC_MESSAGE_TRACE_HISTORY_SIZE; 
 
    printf("MESSAGE TRACE BUFFERS\n"
            "-----------------------------\n"
            "Send trace buffer:\n");
    for (i=0;i<IPC_MESSAGE_TRACE_HISTORY_SIZE 
             && send_dump[i].time_stamp != 0;i++)
    {
        printf("  %2u | %u | %u | %3d | %6llu\n",i+1,send_dump[i].dest_pid,
               send_dump[i].send_pid, send_dump[i].msg_type,
               send_dump[i].time_stamp);
    }        
    
    printf("\nReceive trace buffer:\n");
    for (i=0;i<IPC_MESSAGE_TRACE_HISTORY_SIZE 
             && recv_dump[i].time_stamp != 0;i++)
    {
        printf("  %2u | %u | %u | %3d | %6llu\n",i+1,recv_dump[i].dest_pid,
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

    char* priorityStr;
    char* pidStr;
    int priority, pid;
    //splitFirstWord (param, priorityStr, pidStr);
    priorityStr = strtok (param," \t");
    pidStr = strtok(NULL," \t");
    priority = atoi(priorityStr);
    pid = atoi(pidStr);
    return change_priority(priority, pid);
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


#include "cci.h"
#include "rtx.h"

#define WAKEUP_CODE 123

void start_cci()
{
    // initialise 
    uint32_t clock_time = 0;
    bool clock_display_en = 0 //clock not displayed by default
    uint32_t status;
    MsgEnv* env = request_msg_env();

    status = get_console_chars(env);
    if (status != CODE_SUCCESS)
        printf("get_console_chars failed with status %d\n",status);
    env = request_msg_env();
    //we can add a periodic delay fcn instead
    status = request_delay ( 10, WAKEUP_CODE, env); //one second delay 
    if (status != CODE_SUCCESS)
        printf("request_delay failed with status %d\n",status);
    //print CCI prompt
    print("CCI: ");

    while (1)
    {
        env = receive_message(); 
        //envelope from timing services
        if (env.msg_type == WAKEUP_CODE)
        {
            status = request_delay ( 10, WAKEUP_CODE, env);
            if (status != CODE_SUCCESS)
                printf("request_delay failed with status %d\n",status);
            clock_time++;
            if (clock_display_en)
            {
                //show clock somehow
            }
            else
            {
                //don't show clock somehow 
            }
        }
        //envelope with characters from console
        else if (env.msg_type == CONSOLE_INPUT)
        {
            //Handle the different CCI commands
            char cmd [3];
            char rem [20];
            splitFirstWord (&env.msg_data, &cmd, &rem);
            switch(cmd)
            {
                //send empty envelope to process A
                case "s":
                    MsgEnv* tmpEnv = request_msg_env();
                    status = send_message (PROCESS_A_PID, tmpEnv);
                    if (status != CODE_SUCCESS)
                        printf("send_message failed with status %d\n",status);
                    break;
                //displays process statuses
                case "ps":
                    MsgEnv* tmpEnv = request_msg_env();
                    status = request_process_status(tmpEnv);
                    if (status != CODE_SUCCESS)
                        printf("request_process_status failed with status %d\n",status);
                    status = CCI_printProcessStatuses(tmpEnv.msg_data);
                    if (status != CODE_SUCCESS)
                        printf("CCI_printProcessStatuses failed with status %d\n",status);
                    break;
                //set clock
                case "c":
                    status = CCI_setClock(&rem);
                    if (status == ERROR_ILLEGAL_ARG)
                        printf("c\n
                                Sets the console clock.\n
                                Usage: c <hh:mm:ss>\n");
                    else if (status != CODE_SUCCESS)
                        printf("CCI_setClock failed with status %d\n",status
                    break;
                //show clock
                case "cd":
                    clock_display_en = 1;
                    break;
                //hide clock
                case "ct":
                    clock_display_en = 0;
                    break;
                //show send/receive trace buffers
                case "b":
                    MsgEnv* tmpEnv = request_msg_env();
                    status = get_trace_buffers (tempEnv);
                    if (status != CODE_SUCCESS)
                        printf("get_trace_buffers failed with status %d\n",status);
                    status = CCI_printTraceBuffers (tempEnv.msg_data);
                    if (status != CODE_SUCCESS)
                        printf("CCI_printTraceBuffers failed with status %d\n",status);
                    break;
                //terminate RTX
                case "t":
                    terminate();
                    break;
                //change process priority
                case "n":
                    status = CCI_setNewPriority(&rem);
                    if (status == ERROR_ILLEGAL_ARG)
                        printf("n\n
                                Sets priority of a user process.\n
                                Usage: n <priority> <processID>\n");
                    if (status != CODE_SUCCESS)
                        printf("CCI_setNewPriority failed with status %d\n",status);
                    break;
                default :
                    printf("Invalid command.\n");
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
int CCI_printProcessStatuses (char* data)
{
}

//sets the clock 
int CCI_setClock (char* timeParam)
{
    if (timeParam == NULL)
        return ERROR_NULL_ARG;
    if (timeParam[2] != ':' || timeParam[5] != ':' || timeParam[8] != ':')
        return ERROR_ILLEGAL_ARG;

    char hr [3];
    char min [3];
    char sec [3];
    int i;
    for (i=0;i<2;i++)
    {
        hr[i] =timeParam[i];
        min[i]=timeParam[3+i];
        sec[i]=timeParam[6+i];
    }
    hr[2]='\0';
    min[2]='\0';
    sec[2]='\0';

    clock_time = atoi(hr)*3600 + atoi(min)*60 + atoi(sec);
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
 
    printf("MESSAGE TRACE BUFFERS\n
            -----------------------------\n
            Send trace buffer:\n");
    for (i=0;i<IPC_MESSAGE_TRACE_HISTORY_SIZE 
             && send_dump[i].time_stamp != 0;i++)
    {
        printf("  %2d | %d | %d | %2d | %d\n",i+1,send_dump[i].dest_pid,
               send_dump[i].send_pid, send_dump[i].msg_type,
               send_dump[i].time_stamp);
    }        
    
    printf("\nReceive trace buffer:\n");
    for (i=0;i<IPC_MESSAGE_TRACE_HISTORY_SIZE 
             && recv_dump[i].time_stamp != 0;i++)
    {
        printf("  %2d | %d | %d | %2d | %d\n",i+1,recv_dump[i].dest_pid,
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
        if (ch == '\0')
            retStr2[0]=='\0';
        else
        {
            while (ch == ' ' || ch == '\t')
            {
                i++;
                ch = input [i];
            }

            while (ch != '\0')
            {
                retStr2[i]=ch;
                i++;
                ch = input[i];
            }
            retStr2[i]='\0';
        }
    }
    return CODE_SUCCESS;
}


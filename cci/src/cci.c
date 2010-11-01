#include "cci.h"
#include "rtx.h"

#define WAKEUP_CODE 123

void start_cci()
{
    // initialise 
    uint32_t time = 0;
    bool clock_display_en = 0 //clock not displayed by default
    uint32_t status;
    MsgEnv* env = request_msg_env();

    get_console_chars(env);
    env = request_msg_env();
    //we can add a periodic delay fcn instead
    request_delay ( 10, WAKEUP_CODE, env); //one second delay 
    //print CCI prompt
    print("CCI: ");

    while (1)
    {
        env = receive_message(); 
        //envelope from timing services
        if (env.msg_type == WAKEUP_CODE)
        {
            request_delay ( 10, WAKEUP_CODE, env);
            time++;
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
                    send_message (
                    break;
                //displays process statuses
                case "ps":
                    MsgEnv* tmpEnv = request_msg_env();
                    request_process_status(tmpEnv);
                    CCI_printProcessStatuses(tmpEnv.msg_data);
                    break;
                //set clock
                case "c":
                    CCI_setClock(&rem); 
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
                    get_trace_buffers (tempEnv);
                    CCI_printTraceBuffers (tempEnv.msg_data);
                    break;
                //terminate RTX
                case "t":
                    terminate();
                    break;
                //change process priority
                case "n":
                    CCI_setNewPriority(&rem);
                    break;
                default :
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
int CCI_setClock (char* time)
{
}

//prints trace buffers on console given the envelope message data
int CCI_printTraceBuffers (char* data)
{
}

//set process priority based on params given provided
int CCI_setNewPriority (char* param)
{
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


#include "wallclock.h"
#include "rtx.h"
#include "rtx_util.h"
#include "msg_env_queue.h"
#include <stdlib.h>

#define SAVE_CURSOR "\033[s"
#define RESTORE_CURSOR "\033[u"
#define MOVE_CURSOR "\033[0;72H"
#define CLOCK_FORMAT "%02d:%02d:%02d"
//                    00:00:00
#define EMPTY_CLOCK  "        "

#define WAKEUP_CODE 123
#define ONE_SECOND_DELAY 9 
#define SEC_IN_HR 86400

uint32_t offset;
uint64_t ref;
char clock_display_en;

MsgEnv *timeout_env, *send_env;
msg_env_queue_t *msgQ;

void start_wallclock()
{
    uint32_t status;
    //initialise
    msgQ = msg_env_queue_create();
    offset = 0; 
    ref = 0;
    clock_display_en = 0; //clock not displayed by default
    timeout_env = request_msg_env();
    send_env = request_msg_env();

    status = request_delay ( ONE_SECOND_DELAY, WAKEUP_CODE, timeout_env); 
    if (status != CODE_SUCCESS)
    {
        RTX_printf(send_env, msgQ, "request_delay failed with status %d\n",status);
    }
    
    while (1)
    {
        MsgEnv* env;
        //First check for messages received but not processed by CCI_printf
        if( msg_env_queue_is_empty(msgQ))
        {
            env = receive_message(); 
        }
        else
        {
            env = msg_env_queue_dequeue(msgQ);
        }

        //envelope from timing services
        if (env->msg_type == WAKEUP_CODE)
        {
            status = request_delay( ONE_SECOND_DELAY, WAKEUP_CODE, timeout_env);
            if (status != CODE_SUCCESS)
            {
                RTX_printf(send_env, msgQ, "request_delay failed with status %d\n",status);
            }
            //86400 = 24hrs in secs
            int32_t clock_time = (int32_t)((clock_get_system_time()-ref)/10+offset)%SEC_IN_HR;
            if (clock_display_en)
            {
                RTX_printf(send_env, msgQ,  SAVE_CURSOR MOVE_CURSOR CLOCK_FORMAT RESTORE_CURSOR,
                        clock_time/3600,(clock_time%3600)/60, clock_time%60);
            }
        }
    }
}

//sets the clock
int setWallClock (char* timeParam)
{
    if (timeParam == NULL)
        return ERROR_NULL_ARG;
    if (timeParam[2] != ':' || timeParam[5] != ':')
        return ERROR_ILLEGAL_ARG;

    //parse timeParam string
    char hr_s [3];
    char min_s [3];
    char sec_s [3];
    int32_t i, hr, min, sec;
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
    offset = (hr*3600 + min*60 + sec);
    ref = clock_get_system_time(); 
    return CODE_SUCCESS;
}

//set whether clock is displayed or not
void displayWallClock (int disp_b)
{
    if (disp_b)
    {
        clock_display_en = 1;
    }
    else
    {
        RTX_printf(send_env, msgQ, SAVE_CURSOR MOVE_CURSOR EMPTY_CLOCK RESTORE_CURSOR);
        clock_display_en = 0;
    }
}

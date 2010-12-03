#include "wallclock.h"
#include "rtx.h"
#include "utils.h"
#include "processes.h"
#include "msg_env_queue.h"

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
uint32_t ref;
char clock_display_en;

MsgEnv *timeout_env, *send_env;

int _setWallClock (char* timeParam);
void _displayWallClock (int disp_b);

char str[100];
void * params [11];

void start_wallclock()
{
    uint32_t status;
    //initialise
    offset = 0; 
    ref = 0;
    clock_display_en = 0; //clock not displayed by default
    timeout_env = request_msg_env();
    send_env = request_msg_env();

    status = request_delay ( ONE_SECOND_DELAY, WAKEUP_CODE, timeout_env); 
    if (status != CODE_SUCCESS)
    {
        params[0] = &status;
        params[1] = NULL;
        rtx_sprintf(send_env->msg, "request_delay failed with status %d\n", params);
        send_console_chars(send_env);       
    }
    
    while (1)
    {
        MsgEnv* env = receive_message(); 

        //envelope from timing services
        if (env->msg_type == WAKEUP_CODE)
        {
            status = request_delay( ONE_SECOND_DELAY, WAKEUP_CODE, timeout_env);
            if (status != CODE_SUCCESS)
            {
                params[0] = &status;
                params[1] = NULL;
                rtx_sprintf(send_env->msg, "request_delay failed with status %d\n", params);
                send_console_chars(send_env);       
            }
            //86400 = 24hrs in secs
            int32_t clock_time = (int32_t)((get_system_time()-ref)/10
                                 +offset)%SEC_IN_HR;
            if (clock_display_en)
            {
                int32_t hr, min, sec;
                hr = clock_time/3600;
                min = (clock_time%3600)/60;
                sec = clock_time%60;

                params[0] = &hr;
                params[1] = &min;
                params[2] = &sec;
                params[3] = NULL;
                rtx_sprintf(send_env->msg, SAVE_CURSOR MOVE_CURSOR CLOCK_FORMAT
                            RESTORE_CURSOR, params);
                send_console_chars(send_env);       
            }
        }
        else if (env->msg_type == CLOCK_ON)
        {
            _displayWallClock (1);
            env->msg_type = CLOCK_RET;
            send_message(PROCESS_CCI_PID,env);
        }
        else if (env->msg_type == CLOCK_OFF)
        {
            _displayWallClock (0);
            env->msg_type = CLOCK_RET;
            send_message(PROCESS_CCI_PID,env);
        }
        else if (env->msg_type == CLOCK_SET) 
        {
            int status = _setWallClock (env->msg);
            *((int *)env->msg) = status; 
            env->msg_type = CLOCK_RET;
            send_message(PROCESS_CCI_PID,env);
        }
    }
}

//sets the clock
int _setWallClock (char* timeParam)
{
    if (timeParam == NULL)
    {
        return ERROR_NULL_ARG;
    }
    if (timeParam[2] != ':' || timeParam[5] != ':' || timeParam[8] != '\0')
    {
        return ERROR_ILLEGAL_ARG;
    }

    //parse timeParam string
    char hr_s [3];
    char min_s [3];
    char sec_s [3];
    int i, hr, min, sec, ret;
    for (i=0;i<2;i++)
    {
        hr_s[i] =timeParam[i];
        min_s[i]=timeParam[3+i];
        sec_s[i]=timeParam[6+i];
    }
    hr_s[2]='\0';
    min_s[2]='\0';
    sec_s[2]='\0';
    ret = rtx_atoi(hr_s, &hr); 
    ret += rtx_atoi(min_s, &min);
    ret += rtx_atoi(sec_s, &sec);

    if (hr>23 || min>59 || sec > 59 || ret < 3)
        return ERROR_ILLEGAL_ARG;
    offset = (hr*3600 + min*60 + sec);
    ref = get_system_time(); 
    return CODE_SUCCESS;
}

//set whether clock is displayed or not
void _displayWallClock (int disp_b)
{
    if (disp_b)
    {
        clock_display_en = 1;
    }
    else
    {
        params[0] = NULL;
        rtx_sprintf(send_env->msg, SAVE_CURSOR MOVE_CURSOR EMPTY_CLOCK RESTORE_CURSOR, params);
        send_console_chars(send_env);       
        clock_display_en = 0;
    }
}

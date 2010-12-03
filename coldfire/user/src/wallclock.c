#include "wallclock.h"
#include "rtx.h"
#include "utils.h"
#include "msg_env_queue.h"
#include "trace.h"

#define SAVE_CURSOR "\033[s"
#define RESTORE_CURSOR "\033[u"
#define MOVE_CURSOR "\033[0;72H"
#define CLOCK_FORMAT "%02d:%02d:%02d"
//                    00:00:00
#define EMPTY_CLOCK  "        "

#define WAKEUP_CODE 123
#define ONE_SECOND_DELAY 100 
#define SEC_IN_HR 86400

uint32_t offset;
uint32_t ref;
char clock_display_en;

MsgEnv *timeout_env, *send_env;
msg_env_queue_t *msg_q;

int _setWallClock (char* timeParam);
void _displayWallClock (int disp_b);

char str[1024];
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
    msg_q = msg_env_queue_create(); 

    status = request_delay ( ONE_SECOND_DELAY, WAKEUP_CODE, timeout_env); 
    if (status != CODE_SUCCESS)
    {
        params[0] = &status;
        params[1] = NULL;
        rtx_sprintf(str, "request_delay failed with status %d\r\n", params);
        print_ack(str, send_env, msg_q);
    }
    
    MsgEnv* env;

    while (1)
    {
        if (msg_env_queue_is_empty(msg_q))
        {
            env = receive_message();
        }
        else
        {
            env = msg_env_queue_dequeue(msg_q);
        }

        //envelope from timing services
        if (env->msg_type == WAKEUP_CODE)
        {
            status = request_delay( ONE_SECOND_DELAY, WAKEUP_CODE, timeout_env);
            if (status != CODE_SUCCESS)
            {
                params[0] = &status;
                params[1] = NULL;
                rtx_sprintf(str, "request_delay failed with status %d\r\n", params);
                print_ack(str, send_env, msg_q);
            }
            //86400 = 24hrs in secs
            int32_t clock_time = (int32_t)((get_system_time()-ref)/100
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
                rtx_sprintf(str, SAVE_CURSOR MOVE_CURSOR CLOCK_FORMAT
                            RESTORE_CURSOR, params);
                print_ack(str, send_env, msg_q);
            }
        }
        else if (env->msg_type == CLOCK_ON)
        {
            _displayWallClock (1);
            env->msg_type = CLOCK_RET;
            send_message(env->send_pid,env);
        }
        else if (env->msg_type == CLOCK_OFF)
        {
            _displayWallClock (0);
            env->msg_type = CLOCK_RET;
            send_message(env->send_pid,env);
        }
        else if (env->msg_type == CLOCK_SET) 
        {
            int status = _setWallClock (env->msg);
            if (status == ERROR_ILLEGAL_ARG)
            {
                params[0] = NULL;
                rtx_sprintf( str, "c\r\n"
                           "Sets the console clock (24h).\r\n"
                           "Usage: c <hh:mm:ss>\r\n"
                           "hh must be 00-23\r\n"
                           "mm must be 00-59\r\n"
                           "ss must be 00-59\r\n", params );
                print_ack(str, send_env, msg_q);
            }
            else if (status != CODE_SUCCESS)
            {
                params[0] = &status;
                params[1] = NULL;
                rtx_sprintf( str, "CCI_setClock failed with status %d\r\n", params);
                print_ack(str, send_env, msg_q);
            }
            env->msg_type = CLOCK_RET;
            send_message(env->send_pid,env);
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

    if (rtx_strlen(timeParam) != 8 || timeParam[2] != ':' || timeParam[5] != ':' || timeParam[8] != '\0')
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
        rtx_sprintf(str, SAVE_CURSOR MOVE_CURSOR EMPTY_CLOCK RESTORE_CURSOR, params);
        print_ack(str, send_env, msg_q);
        clock_display_en = 0;
    }
}

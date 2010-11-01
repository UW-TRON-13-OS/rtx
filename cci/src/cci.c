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
    //we can add a periodic delay fcn
    request_delay ( 10, WAKEUP_CODE, env); //one second delay 

    while (1)
    {
        env = receive_message(); 
        if (env.msg_type == 
    }
}

#include "rtx.h"
#include "trace.h"
#include "timer_i_process.h"
#include "timeout_queue.h"
#include "k_primitives.h"
#include "k_globals.h"
#include "k_init.h"
#include "null_process.h"


// We need this function because of gcc
int __main(void)
{
    return 0;
}

void fake_cci ()
{   
    trace(ALWAYS,"request envelopes and stuff.");
    MsgEnv *env = k_request_msg_env ();

    while (1)
    {
        trace(ALWAYS,"request delay of 5 sec.");
        k_request_delay( 500, 123 , env);
        k_receive_message ();
        
        trace(ALWAYS,"request delay of 2 sec.");
        k_request_delay( 200, 123 , env);
        k_receive_message ();

        trace(ALWAYS,"request delay of 1 sec.");
        k_request_delay( 100, 123 , env);
        k_receive_message ();
        
        trace(ALWAYS,"request delay of 0.5 sec.");
        k_request_delay( 50, 123 , env);
        k_receive_message ();
        
        trace(ALWAYS,"done");
    }
}

int main()
{
    timeout_queue = NULL; 
    pcb_init_t itable[3];

    itable[0].pid = TIMER_I_PROCESS_PID; 
    itable[0].name = "timer_i_process";
    itable[0].priority = 0;
    itable[0].start = timer_i_process;
    itable[0].stack_size = 4096;
    itable[0].is_i_process = 1;
    itable[0].is_sys_process = 0;

    itable[1].pid = CCI_PID;
    itable[1].name = "fake_cci";
    itable[1].priority = 0;
    itable[1].start = fake_cci;
    itable[1].stack_size = 4096;
    itable[1].is_i_process = 0;
    itable[1].is_sys_process = 1;

    itable[2].pid = NULL_PID;
    itable[2].name = "null";
    itable[2].priority = 3;
    itable[2].start = null_process;
    itable[2].stack_size = 4096;
    itable[2].is_i_process = 0;
    itable[2].is_sys_process = 1;
   
    trace(ALWAYS,"Starting initialization");
    
    k_init(itable, 3, FALSE, TRUE);
  
    return 0;
}


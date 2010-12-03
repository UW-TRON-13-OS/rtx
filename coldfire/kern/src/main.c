#include "rtx.h"
#include "k_globals.h"
#include "user_processes.h"
#include "abc.h"
#include "null_process.h"
#include "cci.h"
#include "wallclock.h"
#include "uart_i_process.h"
#include "timer_i_process.h"
#include "k_init.h"

#define NUM_PROCESSES 8

// We need this function because of gcc
int __main(void)
{
    return 0;
}

int main()
{
    pcb_init_t itable[NUM_PROCESSES];

    itable[0].pid = TIMER_I_PROCESS_PID;
    itable[0].name = "timer_i_process";
    itable[0].priority = 0;
    itable[0].start = timer_i_process;
    itable[0].stack_size = 4096;
    itable[0].is_i_process = 1;
    itable[0].is_sys_process = 0;

    itable[1].pid = UART_I_PROCESS_PID;
    itable[1].name = "uart_i_process";
    itable[1].priority = 0;
    itable[1].start = uart_i_process;
    itable[1].stack_size = 4096;
    itable[1].is_i_process = 1;
    itable[1].is_sys_process = 0;

    itable[2].pid = CCI_PID;
    itable[2].name = "cci";
    itable[2].priority = 0;
    itable[2].start = start_cci;
    itable[2].stack_size = 4096;
    itable[2].is_i_process = 0;
    itable[2].is_sys_process = 1;

    itable[3].pid = NULL_PID;
    itable[3].name = "null";
    itable[3].priority = NULL_PRIORITY;
    itable[3].start = null_process;
    itable[3].stack_size = 4096;
    itable[3].is_i_process = 0;
    itable[3].is_sys_process = 0;
    
    itable[4].pid = WALLCLOCK_PID;
    itable[4].name = "wallclock";
    itable[4].priority = 2;
    itable[4].start = start_wallclock;
    itable[4].stack_size = 4096;
    itable[4].is_i_process = 0;
    itable[4].is_sys_process = 0;
    
    itable[5].pid = PROC_A_PID;
    itable[5].name = "process a";
    itable[5].priority = 2;
    itable[5].start = process_A;
    itable[5].stack_size = 4096;
    itable[5].is_i_process = 0;
    itable[5].is_sys_process = 0;
    
    itable[6].pid = PROC_B_PID;
    itable[6].name = "process b";
    itable[6].priority = 1;
    itable[6].start = process_B;
    itable[6].stack_size = 4096;
    itable[6].is_i_process = 0;
    itable[6].is_sys_process = 0;
    
    itable[7].pid = PROC_C_PID;
    itable[7].name = "process c";
    itable[7].priority = 0;
    itable[7].start = process_C;
    itable[7].stack_size = 4096;
    itable[7].is_i_process = 0;
    itable[7].is_sys_process = 0;
    
    k_init(itable, NUM_PROCESSES, TRUE, TRUE);

    return 0;
}

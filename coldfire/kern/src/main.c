#include "rtx.h"
#include "k_globals.h"
#include "null_process.h"
#include "cci.h"
#include "uart_i_process.h"
#include "k_init.h"

// We need this function because of gcc
int __main(void)
{
    return 0;
}

int main()
{
    pcb_init_t itable[4];

    itable[0].pid = NULL_PID;
    itable[0].name = "null";
    itable[0].priority = NULL_PRIORITY;
    itable[0].start = null_process;
    itable[0].stack_size = 4096;
    itable[0].is_i_process = 0;
    itable[0].is_sys_process = 0;

    itable[1].pid = UART_I_PROCESS_PID;
    itable[1].name = "uart_i_process";
    itable[1].priority = 0;
    itable[1].start = uart_i_process;
    itable[1].stack_size = 4096;
    itable[1].is_i_process = 1;
    itable[1].is_sys_process = 0;

    itable[1].pid = TIMER_I_PROCESS_PID;
    itable[1].name = "timer_i_process";
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

    
    k_init(itable, 4);

    return 0;
}

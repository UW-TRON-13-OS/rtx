#include "k_globals.h"
#include "trace.h"

#define TIMER_ISR 1
#define UART_ISR 2

pcb_t * interrupted_process;

void isr_handler(uint32_t int_source)
{
    interrupted_process = current_process;
    current_process = NULL;
    switch (int_source)
    {
        case TIMER_ISR:
            current_process = k_get_process(TIMER_I_PROCESS_PID);
            break;
        case UART_ISR:
            current_process = k_get_process(UART_I_PROCESS_PID);
            break;
    }

    if (current_process)
    {
        current_process->start();
    }
    else
    {
        trace_uint(ALWAYS, "ISR Error: could not find int_source for ", int_source);
    }

    current_process = interrupted_process;
    interrupted_process = NULL;
}

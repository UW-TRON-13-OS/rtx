#include <k_globals.h>
#include <k_signal_handler.h>
#include <k_atomic.h>
#include "stdio.h"

pcb_t* interrupted_process;

void handle_signal(int sig_num)
{
    atomic(ON);

    interrupted_process = current_process;
    switch (sig_num)
    {
        //TODO handle any other specific signals?
        case SIGALRM: 
            k_i_process_enter(&p_table[TIMEOUT_I_PROCESS_PID]);
        case SIGUSR1: 
            k_i_process_enter(&p_table[KB_I_PROCESS_PID]);
        case SIGUSR2: 
            k_i_process_enter(&p_table[CRT_I_PROCESS_PID]);
        default:
            printf("Unknown signal: %d/n",sig_num);
            k_terminate();
    }

    atomic(OFF);
}

void k_i_process_enter (pcb_t* i_process)
{
    //TODO check if provided is i_process
}

void k_i_process_exit ()
{
    //TODO implement me
}

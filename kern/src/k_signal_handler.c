#include <k_globals.h>
#include <k_signal_handler.h>
#include <k_atomic.h>
#include <k_scheduler.h>
#include "stdio.h"

static pcb_t* interrupted_process;

void handle_signal(int sig_num)
{
    atomic(ON);

    printf("Got signal %d\n", sig_num);

    switch (sig_num)
    {
        case SIGINT: 
        case SIGBUS: 
        case SIGHUP: 
        case SIGILL: 
        case SIGQUIT: 
        case SIGSEGV: 
        case SIGTERM: 
        case SIGABRT: 
            k_terminate();
            break;
        case SIGALRM: 
            k_i_process_enter(&p_table[TIMEOUT_I_PROCESS_PID]);
            break;
        case SIGUSR1: 
            k_i_process_enter(&p_table[KB_I_PROCESS_PID]);
            break;
        case SIGUSR2: 
            k_i_process_enter(&p_table[CRT_I_PROCESS_PID]);
            break;
        default:
            printf("Unknown signal: %d/n",sig_num);
            k_terminate();
    }

    atomic(OFF);
}

int flag = 0;
int k_i_process_enter (pcb_t* i_process)
{
    assert(flag == 0);
    flag = 1;
    assert(i_process != NULL);
    if (!i_process->is_i_process)
    {
#ifdef DEBUG_KERN
        printf("i_process %s is not an i process!\n", i_process->name);
#endif
        assert(i_process->is_i_process);
        return ERROR_ILLEGAL_ARG;
    }
#ifdef DEBUG_KERN
        printf("entering i_process %s\n", i_process->name);
#endif
    interrupted_process = current_process;
    interrupted_process->status = P_INTERRUPTED;
    current_process = i_process;
    current_process->status = P_EXECUTING;
    assert(interrupted_process);
    k_context_switch(&interrupted_process->context,
                     &current_process->context);
    return CODE_SUCCESS;
}

void k_i_process_exit ()
{
    assert(flag == 1);
    flag = 0;
#ifdef DEBUG_KERN
        printf("exiting i_process %s\n", current_process->name);
#endif
    pcb_t* i_process = current_process;
    i_process->status = P_READY;
    assert(interrupted_process);
    current_process = interrupted_process;
    interrupted_process = NULL;
    current_process->status = P_EXECUTING;
#ifdef DEBUG_KERN
        printf("switching i_process %s to %s\n", i_process->name, current_process->name);
#endif
    k_context_switch(&i_process->context,
                     &current_process->context);
}

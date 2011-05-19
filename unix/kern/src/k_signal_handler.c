#include <k_globals.h>
#include <k_signal_handler.h>
#include <k_atomic.h>
#include <k_scheduler.h>
#include "msg_env_queue.h"

#include <stdio.h>
#include <stdlib.h>

static pcb_t* interrupted_process;

#ifdef DEBUG_KERN
void print_status_info()
{
    MsgEnv env;
    env.msg = malloc(1024);
    k_request_process_status(&env);
    int * data = (int *) env.msg;
    int num_processes = *data++;
    int i;
    printf ("PID | STATUS                | PRIORITY\n");
    for (i=0;i<num_processes;i++)
    {
        printf("  %d   ",*data++);
        switch(*data)
        {
            case P_READY:
                printf("ready                  ");
                break;
            case P_EXECUTING:
                printf("executing              ");
                break;
            case P_BLOCKED_ON_ENV_REQUEST:
                printf("blocked on env request ");
                break;
            case P_BLOCKED_ON_RECEIVE:
                printf("blocked on receive     ");
                break;
            default :
                printf("                       ");
                break;
        }
        data++;
        printf(" %d\n",*data++);
    }
}
#endif // DEBUG_KERN

void handle_signal(int sig_num)
{
    atomic(ON);

#ifdef DEBUG_KERN
    printf("Got signal %d\n", sig_num);
#endif

    switch (sig_num)
    {
        case SIGINT: 
#ifdef DEBUG_KERN
            printf("Interrupted in process %s\n", current_process->name);
            printf("Num free envelopes: %d\n", msg_env_queue_size(free_env_q));
            print_status_info();
#endif
        case SIGBUS: 
        case SIGHUP: 
        case SIGILL: 
        case SIGQUIT: 
        case SIGSEGV: 
        case SIGTERM: 
        case SIGABRT: 
#ifdef DEBUG_KERN
            printf("Terminating with signal %d\n", sig_num);
#endif
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
#ifdef DEBUG_KERN
            printf("Unknown signal: %d/n",sig_num);
#endif
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
        printf("\tentering i_process %s\n", i_process->name);
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
#ifdef DEBUG_KERN
        printf("\texiting i_process %s\n", current_process->name);
#endif
    assert(flag == 1);
    flag = 0;
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

#include "k_globals.h"

proc_pq_t *ready_pq;
proc_pq_t *blocked_request_env_pq;

void k_process_switch(enum process_state transition_to)
{
    pcb_t *old_proc = current_process;
    old_proc->state = transition_to;

    current_process = proc_pq_dequeue(ready_pq);
    current_process->state = P_EXECUTING;
    k_context_switch(old_proc, current_process);
}

void k_context_switch(pcb_t * prev, pcb_t * next)
{
    // Save the current stack
    asm("move.l %%a7, %0": "=m" (prev->context.stack_ptr) : );
    // Restore the next stack
    asm("move.l %0, %%a7":  : "m" (next->context.stack_ptr) );  
}

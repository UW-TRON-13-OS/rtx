#include "k_scheduler.h"
#include "k_process.h"
#include "k_globals.h"
#include "proc_pq.h"
#include "k_atomic.h"
#include "processes.h"

#define RESUME_PROCESS 1

#ifdef DEBUG_KERN
#include <stdio.h>
#include <string.h>
#endif

proc_pq_t * ready_pq;

void k_process_switch(p_status_t next_status)
{
    pcb_t *old_proc = current_process;
    old_proc->status = next_status;

    current_process = proc_pq_dequeue(ready_pq);
#ifdef DEBUG_KERN
    assert(current_process != NULL);
    if (current_process->pid != PROCESS_NULL_PID)
        printf("%s now entering executing\n", current_process->name);
#endif
    current_process->status = P_EXECUTING;
    k_context_switch(&old_proc->context, &current_process->context);
}

void k_context_switch(jmp_buf *old_context, jmp_buf *new_context)
{

#ifdef DEBUG_KERN
    assert(old_context != NULL);
    assert(new_context != NULL);
#endif
    if (setjmp(*old_context) == 0)
    {
        longjmp(*new_context, RESUME_PROCESS);
    }
#ifdef DEBUG_KERN
    if (current_process->pid != PROCESS_NULL_PID)
        printf("%s context now loaded\n", current_process->name);
#endif
}

void k_enter_scheduler()
{
    pcb_t * first_pcb = proc_pq_dequeue(ready_pq);
    current_process = first_pcb;
    first_pcb->status = P_EXECUTING;
    longjmp(first_pcb->context, RESUME_PROCESS);
}

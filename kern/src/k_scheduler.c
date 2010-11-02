#include "k_scheduler.h"
#include "k_process.h"
#include "k_globals.h"
#include "proc_pq.h"

#define RESUME_PROCESS 1

// global variables
proc_pq_t * ready_pq;

void k_process_switch(p_status_t next_status)
{
    pcb_t *old_proc = current_process;
    old_proc->status = next_status;

    current_process = proc_pq_dequeue(ready_pq);
    current_process->status = P_EXECUTING;
    k_context_switch(&old_proc->context, &current_process->context);
}

void k_context_switch(jmp_buf *old_context, jmp_buf *new_context)
{
    if (setjmp(*old_context) == 0)
    {
        longjmp(*new_context, RESUME_PROCESS);
    }
}

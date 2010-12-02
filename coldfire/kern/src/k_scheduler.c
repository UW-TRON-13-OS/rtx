#include "k_globals.h"
#include "dbug.h"

proc_pq_t *ready_pq;
proc_pq_t *blocked_request_env_pq;

void k_process_switch(enum process_state transition_to)
{
    pcb_t *old_proc = current_process;
    old_proc->state = transition_to;

    proc_pq_print(ready_pq);
    current_process = proc_pq_dequeue(ready_pq);
    current_process->state = P_EXECUTING;
    k_context_switch(&old_proc->context, &current_process->context);
}

uint32_t temp;
void k_context_switch(pcb_context_t * old_context, pcb_context_t * new_context)
{
    // Save the current registers onto the stack
    dbug("Enter context switch");

    if (old_context)
    {
        dbug("    Saving registers");
        asm("move.l %d0, -(%a7)");
        asm("move.l %d1, -(%a7)");
        asm("move.l %d2, -(%a7)");
        asm("move.l %d3, -(%a7)");
        asm("move.l %d4, -(%a7)");
        asm("move.l %d5, -(%a7)");
        asm("move.l %d6, -(%a7)");
        asm("move.l %d7, -(%a7)");

        asm("move.l %a0, -(%a7)");
        asm("move.l %a1, -(%a7)");
        asm("move.l %a2, -(%a7)");
        asm("move.l %a3, -(%a7)");
        asm("move.l %a4, -(%a7)");
        asm("move.l %a5, -(%a7)");
        asm("move.l %a6, -(%a7)");
    }

    dbug_hex("    Saving to ", old_context->stack_ptr);
    dbug_hex("    Restoring from ", new_context->stack_ptr);
    dbug_ptr("    Current process ptr : ", current_process);
    rtx_dbug_outs("    Current process : ");
    dbug(current_process->name);

    // Save the current stack
    asm("move.l %%a7, %0": "=m" (old_context->stack_ptr) : );
    // Restore the next stack
    asm("move.l %0, %%a7":  : "m" (new_context->stack_ptr) );  

    // Restore the registers from the stack
    asm("move.l (%a7)+, %a6");
    asm("move.l (%a7)+, %a5");
    asm("move.l (%a7)+, %a4");
    asm("move.l (%a7)+, %a3");
    asm("move.l (%a7)+, %a2");
    asm("move.l (%a7)+, %a1");
    asm("move.l (%a7)+, %a0");

    asm("move.l (%a7)+, %d7");
    asm("move.l (%a7)+, %d6");
    asm("move.l (%a7)+, %d5");
    asm("move.l (%a7)+, %d4");
    asm("move.l (%a7)+, %d3");
    asm("move.l (%a7)+, %d2");
    asm("move.l (%a7)+, %d1");
    asm("move.l (%a7)+, %d0");

    if (current_process->first_time)
    {
        dbug("   First time ");
        asm("move.l %%a7, %0": "=m" (temp));
        dbug_hex("    rte from ", temp);
        dbug_hex("      ->pc is ", ((uint32_t*)temp)[1]);
        //asm("move.l #0x45672000, (%a7)");
        //asm("move.l (%a7), %a1");
        //asm("move.l 4(%a7), %a2");
        current_process->first_time = 0;
        asm("rte");
    }
    dbug("  ->End context switch");
}

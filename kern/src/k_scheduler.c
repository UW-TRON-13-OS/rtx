#include <k_scheduler.h>

#define RESUME_PROCESS 1

// global variables
pcb_t *current_process;

void process_switch()
{
    // choose the next process to switch into
    
    //k_context_switch(next_process);
}

void k_context_switch(pcb_t *next_process)
{
    // Save the current context
    if (setjmp(current_process->context) == 0)
    {
        // Switch the current process to the next process
        current_process = next_process;
        longjmp(next_process->context, RESUME_PROCESS);
    }

    // The context has now switched back to this process
}

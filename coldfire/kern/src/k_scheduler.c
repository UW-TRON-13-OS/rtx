#include "k_globals.h"

proc_pq_t *ready_pq;
proc_pq_t *blocked_request_env_pq;

void k_process_switch(enum process_state transition_to)
{
}

void k_context_switch(pcb_t * prev, pcb_t * next)
{
    
}

#include "k_process.h"
#include "k_config.h"
#include "k_globals.h"
#include "k_scheduler.h"

pcb_t * current_process;
pcb_t   p_table[NUM_PROCESSES];

int k_release_processor()
{
    proc_pq_enqueue(ready_pq, current_process);
    k_process_switch(P_READY);
    return CODE_SUCCESS;
}

int k_request_process_status(MsgEnv *msg_env)
{
    uint32_t * data = (uint32_t *) msg_env->msg;
    int i;
    *data++ = NUM_PROCESSES;
    for (i = 0; i < NUM_PROCESSES; i++)
    {
        *data++ = p_table[i].pid;
        *data++ = p_table[i].status;
        *data++ = p_table[i].priority;
    }
    return CODE_SUCCESS;
}

int k_terminate()
{
    assert(0);
    return -1;
}

int k_change_priority(int new_priority, int target_process_id)
{
    return -1;
}

void k_init_processes(proc_cfg_t init_table[])
{
    int i;
    for (i = 0; i < NUM_PROCESSES; i++)
    {
        pcb_t *pcb = &p_table[i];
        proc_cfg_t *cfg = &init_table[i];
        pcb->pid =      cfg->pid;
        pcb->priority = cfg->priority;
        pcb->name =     cfg->name;
        pcb->is_i_process =    cfg->is_i_process;
        pcb->start =    cfg->start;

        // TODO stack stuff and rpq
    }
}

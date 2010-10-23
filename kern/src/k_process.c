#include "k_process.h"
#include "k_config.h"

pcb_t * current_process;
pcb_t   p_table[NUM_PROCESSES];

int k_request_process_status(MsgEnv *msg_env_ptr)
{
    return -1;
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

#include "k_process.h"
#include "k_config.h"
#include "k_globals.h"
#include "k_scheduler.h"
#include "k_atomic.h"

#include "processes.h"

#include <stdlib.h>
#include <setjmp.h>

#define NULL_PROCESS 1

static int _num_processes;
pcb_t * current_process;
pcb_t   p_table[MAX_NUM_PROCESSES];

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
    *data++ = _num_processes;
    for (i = 0; i < _num_processes; i++)
    {
        *data++ = p_table[i].pid;
        *data++ = p_table[i].status;
        *data++ = p_table[i].priority;
    }
    return CODE_SUCCESS;
}

int k_change_priority(int new_priority, int target_process_id)
{
    if (new_priority < 0 || new_priority >= NUM_PRIORITIES ||
            target_process_id < 0 || target_process_id >= _num_processes)
    {
        return ERROR_ILLEGAL_ARG;
    }

    pcb_t *pcb = &p_table[target_process_id];
    if (pcb->is_i_process || pcb->pid == PROCESS_NULL_PID)
    {
        return ERROR_ILLEGAL_ARG;
    }
    switch (pcb->status)
    {
        case P_READY:
            proc_pq_remove(ready_pq, pcb);
            pcb->priority = new_priority;
            proc_pq_enqueue(ready_pq, pcb);
            break;

        case P_BLOCKED_ON_ENV_REQUEST:
            proc_pq_remove(env_blocked_pq, pcb);
            pcb->priority = new_priority;
            proc_pq_enqueue(env_blocked_pq, pcb);
            break;

        default:
            pcb->priority = new_priority;
            break;
    }

    return CODE_SUCCESS;
}

void k_init_processes(int num_processes, proc_cfg_t init_table[])
{
    jmp_buf init_buf;
    int i;
    _num_processes = num_processes;
    ready_pq = proc_pq_create(NUM_PRIORITIES+NULL_PROCESS);
    for (i = 0; i < num_processes; i++)
    {
        pcb_t *pcb = &p_table[i];
        proc_cfg_t *cfg = &init_table[i];
        pcb->pid = cfg->pid;
        pcb->priority = cfg->priority;
        pcb->name = cfg->name;
        pcb->is_i_process = cfg->is_i_process;
        pcb->start = cfg->start;
        pcb->recv_msgs = msg_env_queue_create();
        pcb->next = NULL;
        pcb->status = P_READY;
        pcb->stack_end = malloc(STACK_SIZE);
        if (pcb->is_i_process)
        {
            pcb->atomic_count = 0;
        }
        else
        {
            pcb->atomic_count = 1;
        }

        // Initialize the stack and start pc
        if (setjmp(init_buf) ==  0)
        {
            char * stack_top = pcb->stack_end + STACK_SIZE - STACK_OFFSET;
            __asm__("movl %0, %%esp":"=g" (stack_top));
            if (setjmp(pcb->context) == 0)
            {
                longjmp(init_buf, 1);
            }
            else
            {
                if (!current_process->is_i_process)
                    atomic(OFF);
                current_process->start();
            }
        }

        // If the process is not an i process place it on the ready queue
        if (!pcb->is_i_process)
        {
            assert(proc_pq_enqueue(ready_pq, pcb) == CODE_SUCCESS);
        }
    }
}

int k_get_num_processes()
{
    return _num_processes;
}

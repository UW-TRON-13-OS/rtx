#include "proc_pq.h"
#include "proc_queue.h"
#include "k_globals.h"
#include "dbug.h"

struct proc_pq {
    uint32_t num_priorities;
    proc_queue_t **priority_queues;
};

proc_pq_t * proc_pq_create(uint32_t num_priorities)
{
    if (num_priorities == 0)
    {
        return NULL;
    }

    proc_pq_t * ppq = k_malloc(sizeof(*ppq));
    ppq->priority_queues = k_malloc(sizeof(*ppq->priority_queues) * num_priorities);
    ppq->num_priorities = num_priorities;
    int i;
    for (i = 0; i < num_priorities; i++)
    {
        ppq->priority_queues[i] = proc_queue_create();
    }
    return ppq;
}

pcb_t * proc_pq_dequeue(proc_pq_t *ppq)
{
    if (ppq != NULL)
    {
        int priority;
        for (priority = 0; priority < ppq->num_priorities; priority++)
        {
            if (!proc_queue_is_empty(ppq->priority_queues[priority]))
            {
                return proc_queue_dequeue(ppq->priority_queues[priority]);
            }
        }
    }

    return NULL;
}

int proc_pq_enqueue(proc_pq_t *ppq, pcb_t * pcb)
{
    if (ppq == NULL || pcb == NULL)
    {
        return ERROR_NULL_ARG;
    }

    if (pcb->priority < 0 || pcb->priority >= ppq->num_priorities)
    {
        return ERROR_ILLEGAL_ARG;
    }

    return proc_queue_enqueue(ppq->priority_queues[pcb->priority], pcb);
}

uint32_t proc_pq_get_num_prorities(proc_pq_t *ppq)
{
    return ppq ? ppq->num_priorities : -1;
}

pcb_t * proc_pq_remove(proc_pq_t * ppq, pcb_t *pcb)
{
    if (ppq != NULL && pcb != NULL && 
        pcb->priority >= 0 && pcb->priority < ppq->num_priorities)
    {
        return proc_queue_remove(ppq->priority_queues[pcb->priority], pcb);
    }

    return NULL;
}

void proc_pq_print(proc_pq_t* ppq)
{
    dbug_uint("ppq # priorities ", ppq->num_priorities);
    int i;
    for (i = 0; i < ppq->num_priorities; i++)
    {
        rtx_dbug_outs("     priority ");
        rtx_dbug_uint(i);
        proc_queue_print(ppq->priority_queues[i]);
    }
}


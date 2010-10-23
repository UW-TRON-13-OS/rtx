#include "proc_pq.h"
#include "proc_queue.h"

#include <stdlib.h>

struct proc_pq {
    uint32_t num_priorities;
    proc_queue_t **priority_queues;
};

proc_pq_t * proc_pq_create(uint32_t num_priorities)
{
    proc_pq_t * ppq = malloc(sizeof(*ppq));
    if (ppq)
    {
        ppq->priority_queues = malloc(sizeof(*ppq->priority_queues) * num_priorities);
        if (ppq->priority_queues)
        {
            int priority;
            for (priority = 0; priority < num_priorities; priority++)
            {
                ppq->priority_queues[priority] = proc_queue_create();
                // TODO error handling
            }
        }
        else
        {
            free(ppq);
            ppq = NULL;
        }
    }
    return ppq;
}

void proc_pq_destroy(proc_pq_t * ppq)
{
    if (ppq)
    {
        if (ppq->priority_queues)
        {
            int priority;
            for (priority = 0; priority < ppq->num_priorities; priority++)
            {
                 proc_queue_destroy(ppq->priority_queues[priority]);
            }
            free(ppq->priority_queues);
        }
        free(ppq);
    }
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

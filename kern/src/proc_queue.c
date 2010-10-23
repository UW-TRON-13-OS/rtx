#include "proc_queue.h"

#include <stdlib.h>

struct proc_queue {
    pcb_t *head;
    pcb_t *tail;
};

proc_queue_t * proc_queue_create()
{
    proc_queue_t * queue = malloc(sizeof(*queue));
    if (queue)
    {
        queue->head = queue-> tail = NULL;
    }
    return queue;
}

void proc_queue_destroy(proc_queue_t * queue)
{
    free(queue);
}

int proc_queue_is_empty(proc_queue_t* queue)
{
    if (queue == NULL)
    {
        return -1;
    }
    
    return queue->head == NULL;
}

pcb_t * proc_queue_dequeue(proc_queue_t* queue)
{
    if (queue == NULL || proc_queue_is_empty(queue))
    {
        return NULL;
    }

    pcb_t * pcb = queue->head;
    queue->head = queue->head->next;
    if (queue->head == NULL)
    {
        queue->tail = NULL;
    }
    pcb->next = NULL;
    return pcb;
}

int proc_queue_enqueue(proc_queue_t* queue, pcb_t * pcb)
{
    if (queue == NULL || pcb == NULL)
    {
        return ERROR_NULL_ARG;
    }

    assert(pcb->next == NULL);
    if (proc_queue_is_empty(queue))
    {
        queue->head = pcb;
    }
    queue->tail->next = pcb;
    queue->tail = pcb;
    queue->tail = NULL;

    return CODE_SUCCESS;
}


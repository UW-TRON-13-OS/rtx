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
    if (queue->head == NULL)
    {
        queue->tail = NULL;
    }
    else
    {
        queue->head = queue->head->next;
    }
    pcb->next = NULL;
    return pcb;
}

int proc_queue_enqueue(proc_queue_t * queue, pcb_t * pcb)
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
    else
    {
        queue->tail->next = pcb;
    }
    queue->tail = pcb;
    queue->tail->next = NULL;

    return CODE_SUCCESS;
}

pcb_t * proc_queue_remove(proc_queue_t * queue, pcb_t *pcb)
{
    if (queue == NULL || pcb == NULL || proc_queue_is_empty(queue))
    {
        return NULL;
    }

    if (queue->head == pcb)
    {
        return proc_queue_dequeue(queue);
    }

    pcb_t *prev_node = queue->head;
    pcb_t *curr_node = queue->head->next;
    while (curr_node)
    {
        if (curr_node == pcb)
        {
            prev_node->next = curr_node->next;
            curr_node->next = NULL;
            return curr_node;
        }
        prev_node = curr_node;
        curr_node = curr_node->next;
    }

    return NULL;
}

#ifndef _PROC_QUEUE_H_
#define _PROC_QUEUE_H_

#include "k_process.h"

typedef struct proc_queue proc_queue_t;

proc_queue_t * proc_queue_create();
void proc_queue_destroy(proc_queue_t * queue);

int proc_queue_is_empty(proc_queue_t* queue);
pcb_t * proc_queue_dequeue(proc_queue_t* queue);
int proc_queue_enqueue(proc_queue_t* queue, pcb_t * pcb);

#endif

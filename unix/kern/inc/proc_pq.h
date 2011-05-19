#ifndef _PROC_PQ_H_
#define _PROC_PQ_H_

#include <rtx.h>
#include "k_process.h"

typedef struct proc_pq proc_pq_t;

proc_pq_t * proc_pq_create(uint32_t num_priorities);
void proc_pq_destroy(proc_pq_t *ppq);

pcb_t * proc_pq_dequeue(proc_pq_t *ppq);
int proc_pq_enqueue(proc_pq_t *ppq, pcb_t * pcb);

uint32_t proc_pq_get_num_prorities(proc_pq_t *ppq);

pcb_t * proc_pq_remove(proc_pq_t * ppq, pcb_t *pcb);

#endif

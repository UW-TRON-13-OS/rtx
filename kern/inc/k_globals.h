#ifndef _KERN_GLOBALS_H_
#define _KERN_GLOBALS_H_

#include <rtx.h>
#include "k_process.h"
#include "proc_pq.h"

extern pcb_t *  current_process;
extern pcb_t    p_table[]; 

extern proc_pq_t * ready_pq;

#endif

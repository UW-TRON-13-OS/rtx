#ifndef _KERN_GLOBALS_H_
#define _KERN_GLOBALS_H_

#include <rtx.h>
#include <sys/types.h>
#include "k_process.h"
#include "proc_pq.h"

#define KB_I_PROCESS_PID        0
#define CRT_I_PROCESS_PID       1
#define TIMEOUT_I_PROCESS_PID   2

extern pcb_t *  current_process;
extern pcb_t    p_table[]; 

extern proc_pq_t * ready_pq;
extern proc_pq_t * env_blocked_pq;

extern MsgEnv * env_pool;

#endif

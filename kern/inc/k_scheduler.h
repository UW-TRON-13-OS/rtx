#ifndef _KERN_SCHEDULER_H_
#define _KERN_SCHEDULER_H_

#include "rtx_thread.h"

// unix context switch simulation
#include <setjmp.h>

typedef enum p_status {
    P_READY, P_EXECUTING, P_BLOCKED, P_SUSPENDED;
} p_status_t;

typedef struct pcb {
    uint32_t    pid;
    uint32_t    priority;
    p_status_t  status;
    jmp_buf     context;
    queue_t     recv_msgs;
    uint32_t    is_i_process;
    char *      name;
} pcb_t;

extern pcb_t *current_process;

void k_context_switch(pcb_t *next_process);

#endif

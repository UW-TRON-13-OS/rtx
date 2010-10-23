#ifndef _KERN_PROCESS_H_
#define _KERN_PROCESS_H_

#include <setjmp.h>

#include <rtx.h>
#include "msg_env_queue.h"

typedef enum p_status {
    P_READY, P_EXECUTING, P_BLOCKED, P_SUSPENDED
} p_status_t;

typedef void (*start_pc)();
typedef struct pcb {
    struct pcb *        next;
    uint32_t            pid;
    char *              name;
    uint32_t            priority;
    p_status_t          status;
    jmp_buf             context;
    msg_env_queue_t *   recv_msgs;
    uint32_t            is_i_process;
    start_pc            start;
} pcb_t;

typedef struct proc_cfg {
    uint32_t            pid;
    char *              name;
    uint32_t            priority;
    uint32_t            is_i_process;
    start_pc            start;
} proc_cfg_t;

/** 5.3 Processor Management **/
int k_request_process_status(MsgEnv *msg_env_ptr);
int k_terminate();
int k_change_priority(int new_priority, int target_process_id);

/** Kernel Process Functions **/
void k_init_processes(proc_cfg_t init_table[]);

#endif

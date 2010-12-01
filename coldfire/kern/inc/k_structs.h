#ifndef _KERN_STRUCTS_H_
#define _KERN_STRUCTS_H_

#include "rtx.h"
#include "msg_env_queue.h"

typedef struct pcb_context {
    uint32_t stack_ptr;
} pcb_context_t;

typedef struct pcb {
    struct pcb *        next;
    int32_t             pid;
    char *              name;
    uint32_t            priority;
    enum process_state  state;
    pcb_context_t       context;
    bool                is_i_process;
    msg_env_queue_t *   recv_msgs;
    start_pc            start;
    byte *              stack_begin;
    uint32_t *          stack_bottom;
    bool                first_time;
} pcb_t;

typedef struct pcb_init {
    int32_t             pid;
    char *              name;
    uint32_t            priority;
    start_pc            start;
    uint32_t            stack_size;
    bool                is_i_process;
    bool                is_sys_process;
} pcb_init_t;

typedef struct trace_circle_buf {
    uint32_t tail;
    ipc_trace_t *buf;
} trace_circle_buf_t;

#endif

#ifndef _KERN_STRUCTS_H_
#define _KERN_STRUCTS_H_

#include "rtx.h"
#include "msg_env_queue.h"

typedef struct pcb_context {
    int32_t stack_ptr;
} pcb_context_t;

typedef struct pcb {
    pcb_context_t       context;
    struct pcb *        next;
    int32_t             pid;
    char *              name;
    uint32_t            priority;
    enum process_state  state;
    bool                is_i_process;
    //msg_env_queue_t     recv_msgs;
    start_pc            start;
    byte *              stack_end;
} pcb_t;

typedef struct pcb_init {
    int32_t             pid;
    char *              name;
    uint32_t            priority;
    bool                is_i_process;
    start_pc            start;
} pcb_init_t;

#endif

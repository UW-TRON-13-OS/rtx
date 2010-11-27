#ifndef _KERN_GLOBALS_H_
#define _KERN_GLOBALS_H_

#include "rtx.h"
#include "k_structs.h"
#include "proc_pq.h"
#include "msg_env_queue.h"

// process pids
#define NULL_PID 0
#define CCI_PID -1
#define UART_I_PROCESS_PID -2

// Process priorities
#define NUM_PRIORITIES 3
#define ADDITIONAL_NULL_PRIORITY 1
#define HIGHEST_PRIORITY 0
#define LOWEST_PRIORITY  2
#define NULL_PRIORITY    3

// Processes
extern pcb_t * current_process;
int32_t k_get_num_processes();
pcb_t * k_get_process(int32_t pid);

// Scheduling
extern proc_pq_t *ready_pq;
extern proc_pq_t *blocked_request_env_pq;

void k_process_switch(enum process_state transition_to);
void k_context_switch(pcb_t * prev, pcb_t * next);

// Memory allocation
void * k_malloc(uint32_t size);

// Msg Resources
extern msg_env_queue_t * free_env_q;

#endif

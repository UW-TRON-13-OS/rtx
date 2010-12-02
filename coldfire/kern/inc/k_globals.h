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
#define TIMER_I_PROCESS_PID -3

// Process priorities
#define NUM_PRIORITIES 3
#define ADDITIONAL_NULL_PRIORITY 1
#define HIGHEST_PRIORITY 0
#define LOWEST_PRIORITY  2
#define NULL_PRIORITY    3

// Msg
#define IPC_MESSAGE_TRACE_HISTORY_SIZE 16
#define IPC_MSG_ENV_MSG_SIZE 1024
#define IPC_NUM_FREE_MSG_ENVS 32

// Processes
extern pcb_t *      current_process;
extern pcb_t *      interrupted_process;
extern int32_t *    pid_table;

int32_t k_get_num_processes();
pcb_t * k_get_process(int32_t pid);

// Scheduling
extern proc_pq_t *ready_pq;
extern proc_pq_t *blocked_request_env_pq;

void k_process_switch(enum process_state transition_to);
void k_context_switch(pcb_context_t * old_context, pcb_context_t * new_context);

// Memory allocation
void * k_malloc(uint32_t size);

// Msg Resources
extern msg_env_queue_t * free_env_q;
extern trace_circle_buf_t send_trace_buf;
extern trace_circle_buf_t recv_trace_buf;

#endif

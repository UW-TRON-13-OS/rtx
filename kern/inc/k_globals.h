#ifndef _KERN_GLOBALS_H_
#define _KERN_GLOBALS_H_

#include <rtx.h>
#include <sys/types.h>
#include "k_process.h"
#include "proc_pq.h"

#include "keyboard_shmem.h"
#include "crt_shmem.h"

#define KB_I_PROCESS_PID        4
#define CRT_I_PROCESS_PID       5
#define TIMEOUT_I_PROCESS_PID   6

extern pcb_t *  current_process;
extern pcb_t    p_table[]; 

extern proc_pq_t * ready_pq;
extern proc_pq_t * env_blocked_pq;

extern recv_buf_t * kb_buf;
extern send_buf_t * crt_buf;

extern pid_t rtx_pid;
extern pid_t kb_child_pid;
extern pid_t crt_child_pid;

#endif

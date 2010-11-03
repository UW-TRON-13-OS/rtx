#ifndef _KERN_UART_H_
#define _KERN_UART_H_

#include "keyboard_shmem.h"
#include "crt_shmem.h"

extern recv_buf_t * kb_buf;
extern send_buf_t * crt_buf;

extern pid_t rtx_pid;
extern pid_t kb_child_pid;
extern pid_t crt_child_pid;

#endif

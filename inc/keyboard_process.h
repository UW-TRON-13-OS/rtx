#ifndef _KEYBOARD_PROCESS_H_
#define _KEYBOARD_PROCESS_H_

#include <sys/types.h>
#include "keyboard_shmem.h"

void start_keyboard_process(pid_t parent_pid, recv_buf_t * kb_buffer);

#endif

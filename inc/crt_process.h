#ifndef _CRT_PROCESS_H_
#define _CRT_PROCESS_H_

#include <sys/types.h>
#include "crt_shmem.h"

void start_crt_process(pid_t parent_pid, send_buf_t * crt_buffer);

#endif

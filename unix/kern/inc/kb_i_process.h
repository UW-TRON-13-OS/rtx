#ifndef _KB_I_PROCESS_H_
#define _KB_I_PROCESS_H_

#include "keyboard_shmem.h"

void kb_register_shmem(recv_buf_t * kb_buf);
void start_kb_i_process();

#endif

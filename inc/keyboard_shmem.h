#ifndef _KEYBOARD_SHMEM_H_
#define _KEYBOARD_SHMEM_H_

#define KEYBOARD_BUF_SIZE 80

typedef struct recv_buf {
    char kb_wait_flag; // 1 when kb helper process is waiting for i process to set to 0
    char data[KEYBOARD_BUF_SIZE];
} recv_buf_t;

#endif

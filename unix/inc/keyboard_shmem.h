#ifndef _KEYBOARD_SHMEM_H_
#define _KEYBOARD_SHMEM_H_

#define KEYBOARD_BUF_SIZE 1024
#define KB_SHMEM_FILENAME "keyboard_shmem.dat"
#define KB_FLAG_WAIT 1
#define KB_FLAG_FREE 0

typedef struct recv_buf {
    char kb_wait_flag;
    char data[KEYBOARD_BUF_SIZE];
    int length;
} recv_buf_t;

#endif

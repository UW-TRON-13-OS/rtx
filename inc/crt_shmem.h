#ifndef _CRT_SHMEM_H_
#define _CRT_SHMEM_H_

#define CRT_BUF_SIZE 80
#define CRT_SHMEM_FILENAME "crt_shmem.dat"
#define CRT_FLAG_WAIT 1
#define CRT_FLAG_FREE 0

typedef struct send_buf {
    char i_process_wait_flag;
    char data[CRT_BUF_SIZE];
} send_buf_t;

#endif

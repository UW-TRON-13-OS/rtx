#define _XOPEN_SOURCE 500
#include "crt_shmem.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <signal.h>

#define TIME_1MS 1000
#define FAIL -1

send_buf_t * crt_buffer = NULL;

void die(int sig_num)
{
    if (crt_buffer)
    {
        int status = munmap(crt_buffer, sizeof(*crt_buffer));
        if (status == FAIL)
        {
            printf("Error: Unmapping shmem from crt helper process failed\n");
        }
    }
    exit(0);
}

int main(int argc, char *argv[])
{
    int fid; 
    pid_t parent_pid;
    void * mmap_ptr;
    int i;

    if (argc < 3)
    {
        printf("Usage: crt <parent_pid> <shmem_fid>\n");
        exit(1);
    }
    sigset(SIGINT, die); 

    sscanf(argv[1], "%d", &parent_pid);
    sscanf(argv[2], "%d", &fid);
    
    mmap_ptr = mmap(NULL, sizeof(send_buf_t), PROT_READ | PROT_WRITE, 
                    MAP_SHARED, fid, (off_t) 0);
    if (mmap_ptr == MAP_FAILED)
    {
        printf("Could not create mmap pointer to data");
        return 0;
    }
    
    crt_buffer = (send_buf_t *) mmap_ptr;
    
    while (1)
    {
        if (crt_buffer->i_process_wait_flag == CRT_FLAG_WAIT)
        {
            i = 0;
            while (crt_buffer->data[i] != '\0')
            {
                putchar(crt_buffer->data[i]);   // make it print better later
                i++;
            }
            fflush(stdout);
            crt_buffer->i_process_wait_flag = CRT_FLAG_FREE;
            kill(parent_pid, SIGUSR2);
        }
        usleep(TIME_1MS);
    }
    
    return 0;
}

#define _XOPEN_SOURCE 500
#include "keyboard_shmem.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <signal.h>

#define FAIL -1

recv_buf_t * kb_buffer = NULL;

void die(int sig_num)
{
    if (kb_buffer)
    {
        int status = munmap(kb_buffer, sizeof(*kb_buffer));
        if (status == FAIL)
        {
            printf("Error: Unmapping shmem from keyboard helper process "
                   "failed\n");
        }
    }
    exit(0);
}

int main(int argc, char *argv[])
{
    int fid; 
    pid_t parent_pid;
    char * mmap_ptr;
    char c;

    if (argc < 2)
    {
        printf("Usage: crt <parent_pid> <shmem_fid>\n");
        exit(1);
    }
    sigset(SIGINT, die); 
    
    sscanf(argv[1], "%d", &parent_pid);
    sscanf(argv[2], "%d", &fid);
    
    mmap_ptr = (char *) mmap(NULL, sizeof(recv_buf_t), PROT_READ | PROT_WRITE, MAP_SHARED, fid, (off_t) 0);
    if (mmap_ptr == MAP_FAILED)
    {
        printf("Could not create mmap pointer to data");
        return 0;
    }
    
    kb_buffer = (recv_buf_t *) mmap_ptr;
    
    kb_buffer->kb_wait_flag = '0';
    kb_buffer->length = 0;
    
    while (1)
    {
        c = getchar();
		if (c == '\n')
        {
            kill(parent_pid, SIGUSR1); // send a signal to RTX
            kb_buffer->kb_wait_flag = '1';
        }
        else if (kb_buffer->length < KEYBOARD_BUF_SIZE)
        {
            kb_buffer->data[kb_buffer->length] = c;
            kb_buffer->length++;
        }
    }
}

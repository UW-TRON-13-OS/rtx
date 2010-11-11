#include "crt_process.h"
#include "crt_shmem.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define TIME_100MS 100000

int main(int argc, char *argv[])
{
    int fid; 
    pid_t parent_pid;
    caddr_t mmap_ptr;
    send_buf_t * crt_buffer;
    int i;
    
    sscanf(argv[1], "%d", &parent_pid);
    sscanf(argv[2], "%d", &fid);
    
    mmap_ptr = mmap((caddr_t) 0, sizeof(send_buf_t), PROT_READ | PROT_WRITE, MAP_SHARED, fid, (off_t) 0);
    if (mmap_ptr == MAP_FAILED)
    {
        printf("Could not create mmap pointer to data");
        return 0;
    }
    
    kb_buffer = (recv_buf_t *) mmap_ptr;
    
    while (1)
    {
        if (crt_buffer->i_process_wait_flag == '1')
        {
            i = 0;
            while (crt_buffer->data[i] != '\0')
            {
                putchar(crt_buffer->data[i]);   // make it print better later
                i++;
            }
            fflush(stdout);
            crt_buffer->i_process_wait_flag = '0';
        }
        usleep(TIME_100MS);
    }
    
    return 0;
}

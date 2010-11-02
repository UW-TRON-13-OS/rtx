#include "crt_process.h"
#include "crt_shmem.h"

#include <stdio.h>

#define TIME_100MS 100000

void start_crt_process(pid_t parent_pid, send_buf_t * crt_buffer)
{
    int i;
    
    while (1)
    {
        if (crt_buffer->i_process_wait_flag == '1')
        {
            for (i = 0; i < crt_buffer->length; i++)
            {
                putchar(crt_buffer->data[i]);   // make it print better later
            }
            crt_buffer->length = 0;
            crt_buffer->i_process_wait_flag = '0';
        }
        usleep(TIME_100MS);
    }
}

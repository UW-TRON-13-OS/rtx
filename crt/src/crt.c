#include "crt_process.h"
#include "crt_shmem.h"

#include <stdio.h>
#include <unistd.h>

#define TIME_100MS 100000

void start_crt_process(pid_t parent_pid, send_buf_t * crt_buffer)
{
    int i;
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
}

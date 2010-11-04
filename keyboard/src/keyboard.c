#include "keyboard_process.h"
#include "keyboard_shmem.h"

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void start_keyboard_process(pid_t parent_pid, recv_buf_t * kb_buffer)
{
    char c;
    
    kb_buffer->kb_wait_flag = '0';
    kb_buffer->length = 0;

    printf("parent %u\n", parent_pid);
    
    while (1)
    {
        c = getchar();
		if (c == '\n')
        {
            kill(parent_pid, SIGUSR1); // send a signal to RTX
            kb_buffer->kb_wait_flag = '1';
            printf("Sent signal to rtx\n");
        }
        else if (kb_buffer->length < KEYBOARD_BUF_SIZE)
        {
            kb_buffer->data[kb_buffer->length] = c;
            kb_buffer->length++;
        }
    }
}

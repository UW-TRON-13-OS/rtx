#include "keyboard_process.h"
#include "keyboard_shmem.h"
#define SECONDARY_BUFFER_SIZE 128

char secondaryBuffer[SECONDARY_BUFFER_SIZE];
int secBufIndex;

void start_keyboard_process(pid_t parent_pid, recv_buf_t * kb_buffer)
{
    char c;
    int i;
    secBufIndex = 0;
    
    kb_buffer->kb_wait_flag = '0';
    kb_buffer->length = 0;
    
    while (1)
    {
        if (kb_buffer->kb_wait_flag == '1')
        {
            secondaryBuffer[secBufIndex] = c;
            secBufIndex++;
        }
        else
        {
            for (i = 0; i < secBufIndex; i++)
            {
                kb_buffer->data[kb_buffer->length] = secondaryBuffer[i];
                kb_buffer->length++;
            }
            secBufIndex = 0;
        }
        c = getchar();
		if (c == '\n')
        {
            kill(parent_pid, SIGUSR1); // send a signal to RTX
            kb_buffer->kb_wait_flag = '1';
            while (kb_buffer->kb_wait_flag == '1')
            {
                usleep(100000);
            }
        }
        else
        {
            kb_buffer->data[kb_buffer->length] = c;
            kb_buffer->length++;
        }
    }
}

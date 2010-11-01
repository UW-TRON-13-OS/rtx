#include "keyboard_process.h"
#include "keyboard_shmem.h"
#define SECONDARY_BUFFER_SIZE 128

char secondaryBuffer[SECONDARY_BUFFER_SIZE];
int secBufIndex;
//Shared RX Memory

void start_keyboard_process(pid_t parent_pid, recv_buf_t * kb_buffer)
{
    char c;
    int i;
    //int kbBufIndex = 0;
    secBufIndex = 0;
    
    char* mmap_ptr;
    int fid = open(KB_SHMEM_FILENAME, O_RDWR | O_CREAT | O_EXCL, (mode_t) 0755);
    if (fid < 0)
    {
        printf("Bad open of a file");
    }
    mmap_ptr = mmap((char*) 0, KEYBOARD_BUF_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fid, (off_t) 0);
    kb_buffer = (recv_buf*) mmap_ptr;
    
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

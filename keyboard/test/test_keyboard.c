#include "keyboard_shmem.h"
#include "keyboard_process.h"
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

recv_buf_t buffer;

void print_buf()
{
    printf("Buffer length: %d\n", buffer.length);
    printf("Buffer   data: ");
    int i;
    for (i = 0; i < buffer.length; i++)
    {
        putchar(buffer.data[i]);
    }
    printf("\n");
    buffer.length = 0;
}

int main(int argc, char *argv[])
{
    //recv_buf_t buffer;
    int self_pid = getpid();
    sigset(SIGUSR1, print_buf);
    
    start_keyboard_process(self_pid, &buffer);
    return 0;
}

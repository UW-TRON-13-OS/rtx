#include "keyboard_process.h"
#define SECONDARY_BUFFER_SIZE 128

char secondaryBuffer[SECONDARY_BUFFER_SIZE];
//Shared RX Memory
char* kb_buffer;
int* kb_wait_for_read;

void start_keyboard_process(pid_t parent_pid)
{
	char c;
	int secBufIndex = 0;
	int kbBufIndex = 0;
	while (1)
	{
		c = getchar();
		if (kb_wait_for_read[0] == 1)
		{
			secondaryBuffer[secBufIndex] = c;
			secBufIndex++;
		}
		else
		{
			move the characters from the secondary buffer into the shared RX memory
			add the characters into the shared RX memory
		}
		check if any of the inputted characters are a carriage return, if so then
		{
			move anything after the carriage return into the secondary buffer as
				long as there is still space left in the buffer
			send a signal to RTX
			kb_wait_for_read[0] = 1;
		}
		usleep(100000);
	}
}
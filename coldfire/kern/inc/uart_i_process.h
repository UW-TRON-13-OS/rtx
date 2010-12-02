#ifndef _UART_I_PROCESS_H_
#define _UART_I_PROCESS_H_

#include "coldfire.h"

extern uint32_t inputIndex;
extern uint32_t outputIndex;
extern bool output_print_char;


void uart_i_process();

#endif

#ifndef _KERN_SIGNAL_HANDLER_H_
#define _KERN_SIGNAL_HANDLER_H_

#include <signal.h>

void handle_signal(int sig_num);

void k_i_process_enter (pcb_t* i_process);
void k_i_process_exit ();

#endif


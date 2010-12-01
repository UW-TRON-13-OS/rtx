#ifndef _KERN_INIT_H_
#define _KERN_INIT_H_

#include "coldfire.h"
#include "k_structs.h"

SINT32 coldfire_vbr_init( VOID );
void init_uart();
void init_ipc();
void init_timer();
void init_processes(pcb_init_t processes[], uint32_t num_processes);
int k_init(pcb_init_t processes[], uint32_t num_processes);

#endif

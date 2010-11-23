#ifndef _KERN_SCHEDULER_H_
#define _KERN_SCHEDULER_H_

// unix context switch simulation
#include <setjmp.h>

#include <rtx.h>
#include "k_process.h"

void k_enter_scheduler();
void k_process_switch(p_status_t next_status);
void k_context_switch(jmp_buf *old_context, jmp_buf *new_context);

#endif

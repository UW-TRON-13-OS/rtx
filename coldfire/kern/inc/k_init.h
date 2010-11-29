#ifndef _KERN_INIT_H_
#define _KERN_INIT_H_

#include "coldfire.h"

SINT32 coldfire_vbr_init( VOID );
void init_uart();
void init_msg_envs();
void init_timer();
int k_init();

#endif

#ifndef _KERN_INIT_H_
#define _KERN_INIT_H_

SINT32 coldfire_vbr_init( VOID );
void init_uart();
void init_msg_envs();
void init_timer( void );
int k_init();

#endif

#ifndef _KERN_CONFIG_H_
#define _KERN_CONFIG_H_

/**
 * This header file contains various configurable values which directly
 * affect kernel internal behaviour or data structures such as the 
 * size of memory buffers etc.
 */

#define IPC_NUM_FREE_MSG_ENVS   6

#define MAX_NUM_PROCESSES       9
#define NUM_PRIORITIES          3

#define STACK_SIZE              (4*4096)
#define STACK_OFFSET            16

#define USEC_TO_MS              1000
#define DELAY_TIME              (500*USEC_TO_MS)
#define TIMEOUT_100MS           (100*USEC_TO_MS)

#define MSG_ENV_MSG_SIZE        1024

#endif

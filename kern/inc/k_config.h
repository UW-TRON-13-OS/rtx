#ifndef _KERN_K_CONFIG_
#define _KERN_K_CONFIG_

/**
 * This header file contains various configurable values which directly
 * affect kernel internal behaviour or data structures such as the 
 * size of memory buffers etc.
 */

#define IPC_NUM_FREE_MSG_ENVS   128

#define MAX_NUM_PROCESSES       8
#define NUM_PRIORITIES          3

#define STACK_SIZE              4096
#define STACK_OFFSET            4

#define USEC_TO_MS              1000
#define DELAY_TIME              (500*USEC_TO_MS)
#define TIMEOUT_100MS           (100*USEC_TO_MS)

#endif

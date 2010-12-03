#ifndef _KERN_STATS_H_
#define _KERN_STATS_H_

#include "coldfire.h"

uint32_t k_stats_mem_available();
uint32_t k_stats_mem_used();
uint32_t k_stats_stack_space_left(int32_t pid);

#endif

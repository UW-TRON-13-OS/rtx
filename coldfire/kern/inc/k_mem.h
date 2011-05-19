#ifndef _KERN_MEM_H_
#define _KERN_MEM_H_

#include "coldfire.h"

uint32_t k_mem_get_available();
uint32_t k_mem_get_used();
int32_t k_mem_get_stack_left(int32_t pid);

#endif

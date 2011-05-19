#ifndef _KERN_CLOCK_H_
#define _KERN_CLOCK_H_

#include "rtx.h"

// gets the system time in timer ticks
uint32_t k_clock_get_system_time();
void k_clock_incr_system_time();

#endif

#ifndef _KERN_CLOCK_H_
#define _KERN_CLOCK_H_

#include <rtx.h>

// gets the system time in timer ticks
uint64_t k_clock_get_system_time();
uint64_t k_clock_incr_system_time();

#endif

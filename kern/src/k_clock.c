#include "k_clock.h"

static uint64_t clock = 0;

uint64_t k_clock_get_system_time()
{
    return clock;
}

uint64_t k_clock_incr_system_time()
{
    clock++;
}

#include "k_clock.h"
#include "rtx.h"

static uint32_t clock = 1;

uint32_t k_clock_get_system_time()
{
    return clock;
}

void k_clock_incr_system_time()
{
    clock++;
}

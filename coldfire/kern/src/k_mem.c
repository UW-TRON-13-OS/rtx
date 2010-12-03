#include "coldfire.h"
#include "k_globals.h"
#include "k_init.h"
#include "trace.h"

static uint32_t origin;

void init_mem()
{
    origin = HEAP_START;
}

void * k_malloc(uint32_t size)
{
    //align to even boundary
    size = (size+1) & 0xfffffffe;
    if ( size == 0 || (origin + size) > MEMORY_END )
    {
        return NULL;
    }
    uint32_t ret = origin;
    origin += size;
    if (ret % 2 != 0)
    {
        trace(ALWAYS, "k_malloc: foul!!!");
    }
    return (void *) ret;
}

uint32_t k_mem_get_available()
{
    return MEMORY_END - origin;
}

uint32_t k_mem_get_used()
{
    return origin - HEAP_START;
}

int32_t k_mem_get_stack_left(int32_t pid)
{
    pcb_t * pcb = k_get_process(pid);
    uint32_t temp;
    if (pcb == NULL)
    {
        return 0;
    }

    if (pcb->state == P_EXECUTING)
    {
       asm("move.l %a7, -8(%a6)"); 
       return temp - (uint32_t) pcb->stack_begin;
    }

    return pcb->context.stack_ptr - (uint32_t) pcb->stack_begin;
}

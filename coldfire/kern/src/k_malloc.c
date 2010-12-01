#include "memory_layout.h"
#include "k_globals.h"
#define MEMORY_START 0x10100000
#define MEMORY_END   0x10200000

void * k_malloc(uint32_t size)
{
    static uint32_t origin = MEMORY_START;

    //align to even boundary
    size = (size+1) & 0x00000001 
    if ( size == 0 || (origin + size) > MEMORY_END )
    {
        return NULL;
    }
    uint32_t ret = origin;
    origin += size;
    return (void *) ret;
}

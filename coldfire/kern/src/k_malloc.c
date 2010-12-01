#include "coldfire.h"
#include "k_globals.h"
#include "dbug.h"

void * k_malloc(uint32_t size)
{
    static uint32_t origin = HEAP_START;

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
        dbug("k_malloc: foul!!!");
    }
    return (void *) ret;
}

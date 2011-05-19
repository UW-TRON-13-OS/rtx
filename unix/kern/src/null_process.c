#include "null_process.h"
#include <rtx.h>

void start_null()
{
    // loop forever... and ever...
    while (1)
    {
        release_processor();
    }
}

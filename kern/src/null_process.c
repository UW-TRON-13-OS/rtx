#include "null_process.h"
#include <rtx.h>

void start_null()
{
    while (1)
    {
        release_processor();
    }
}

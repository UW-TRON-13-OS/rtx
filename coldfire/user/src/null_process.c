#include "null_process.h"
#include "rtx.h"

void null_process()
{
    while (1)
    {
        release_processor();
    }
}

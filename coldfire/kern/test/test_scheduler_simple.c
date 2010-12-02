#include "dbug.h"
#include "rtx.h"
#include "k_init.h"

// We need this function because of gcc
int __main(void)
{
    return 0;
}

void process1()
{
    uint32_t count = 0;
    while (1)
    {
        bool debug = enable_debug;
        enable_debug = 1;
        dbug("Hello from process 1");
        count++;
        if (count == 1000)
        {
            count = 0;
        }
        dbug_uint("count proc 1 ", count);
        enable_debug = debug;
        release_processor();
    }
}

void process2()
{
    uint32_t count = 1000;
    while (1)
    {
        bool debug = enable_debug;
        enable_debug = 1;
        dbug("Hello from process 2");
        count--;
        if (count == 0)
        {
            count = 1000;
        }
        dbug_uint("count proc 2 ", count);
        enable_debug = debug;
        release_processor();
    }
}

void null_process()
{
    while (1)
    {
        release_processor();
    }
}

int main()
{
    enable_debug = 1;

    pcb_init_t itable[3];
    itable[0].pid = 0;
    itable[0].name = "null";
    itable[0].priority = 3;
    itable[0].start = null_process;
    itable[0].stack_size = 4096;
    itable[0].is_i_process = 0;
    itable[0].is_sys_process = 0;

    itable[1].pid = 1;
    itable[1].name = "1";
    itable[1].priority = 0;
    itable[1].start = process1;
    itable[1].stack_size = 4096;
    itable[1].is_i_process = 0;
    itable[1].is_sys_process = 0;

    itable[2].pid = 2;
    itable[2].name = "2";
    itable[2].priority = 0;
    itable[2].start = process2;
    itable[2].stack_size = 4096;
    itable[2].is_i_process = 0;
    itable[2].is_sys_process = 0;
   
    dbug("Starting initialization");
    
    k_init(itable, 3);
    return 0;
}

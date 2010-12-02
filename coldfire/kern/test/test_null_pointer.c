#include "trace.h"

// We need this function because of gcc
int __main(void)
{
    return 0;
}

int main()
{
    trace(ALWAYS, "Hello world");
    int * foo =  NULL;
    *foo = 10;
    trace(ALWAYS, "After null pointer derefernce");

    return 0;
}

#include "dbug.h"

// We need this function because of gcc
int __main(void)
{
    return 0;
}

int main()
{
    dbug("Hello world");
    int * foo =  NULL;
    *foo = 10;
    dbug("After null pointer derefernce");

    return 0;
}

#include "dbug.h"

// We need this function because of gcc
int __main(void)
{
    return 0;
}

int main()
{
    rtx_dbug_outs("Hello world\n");
    return 0;
}

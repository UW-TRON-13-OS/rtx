#include "dbug.h"
#include "trace.h"

// We need this function because of gcc
int __main(void)
{
    return 0;
}

int main()
{
    trace_uint(ALWAYS, "testhing chars", '\r');
    trace_uint(ALWAYS, "testhing chars", '\0');
    rtx_dbug_outs("Hello world\n");
    return 0;
}

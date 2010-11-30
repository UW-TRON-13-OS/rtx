#include "dbug.h"
#include "rtx.h"
#include "k_init.h"

// We need this function because of gcc
int __main(void)
{
    return 0;
}

int main()
{
    MsgEnv msg_env;
    msg_env.msg = NULL;
    k_init();
    rtx_dbug_uint((uint32_t)&msg_env);
    rtx_dbug_out_char('\n');
    rtx_dbug_uint((uint32_t)msg_env.msg);
    rtx_dbug_out_char('\n');
    int retcode = get_trace_buffers(&msg_env);
    rtx_dbug_outs("Hello world\n");
    if (retcode == 0)
    {
        rtx_dbug_outs("Success\n");
        if (msg_env.msg == NULL)
        {
            rtx_dbug_outs("Oh noes the msg is still null\n");
        }
        else
        {
            rtx_dbug_outs(msg_env.msg);
        }
    }
    else
    {
        rtx_dbug_uint(-1);
        rtx_dbug_out_char('\n');
        rtx_dbug_uint(123);
        rtx_dbug_out_char('\n');
        if (retcode == 1)
        {
            rtx_dbug_outs("got 1\n");
        }
        rtx_dbug_outs("Failure\n");
    }
    return 0;
}

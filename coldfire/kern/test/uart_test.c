#include "trace.h"
#include "uart_i_process.h"
#include "k_globals.h"
#include "rtx.h"
#include "k_primitives.h"
#include "k_init.h"
#include "null_process.h"

// We need this function because of gcc
int __main(void)
{
    return 0;
}

void fake_cci ()
{
    while (1)
    {
        MsgEnv* message = k_receive_message(); // sent from uart to "cci" after enter key
        enable_debug = 1;
        trace(ALWAYS,"CCI check");
        if (message != NULL)
        {
            trace(ALWAYS,"Message was sent to CCI");
            if (message->msg[0] == '\0' || message->msg[1] == '\0')
            {
                trace(ALWAYS,"Short message given in");
                message->msg[2] = '\0';
            }
            message->msg[0] = '@';
            message->msg[1] = '#';
            send_console_chars(message, 0);
        }
        enable_debug = 0;
    }
}

int main()
{
    trace(ALWAYS,"Beginning of test");
    trace(ALWAYS,"Messages will be changed so 1st and 2nd char are @ and #\n");
     
    enable_debug = 0;
    
    pcb_init_t itable[3];

    itable[0].pid = UART_I_PROCESS_PID;
    itable[0].name = "uart_i_process";
    itable[0].priority = 0;
    itable[0].start = uart_i_process;
    itable[0].stack_size = 4096;
    itable[0].is_i_process = 1;
    itable[0].is_sys_process = 0;

    itable[1].pid = CCI_PID;
    itable[1].name = "fake_cci";
    itable[1].priority = 0;
    itable[1].start = fake_cci;
    itable[1].stack_size = 4096;
    itable[1].is_i_process = 0;
    itable[1].is_sys_process = 1;

    itable[2].pid = NULL_PID;
    itable[2].name = "null";
    itable[2].priority = 3;
    itable[2].start = null_process;
    itable[2].stack_size = 4096;
    itable[2].is_i_process = 0;
    itable[2].is_sys_process = 0;
   
    trace(ALWAYS,"Starting initialization");
    
    k_init(itable, 3, TRUE, FALSE);
    return 0;
}

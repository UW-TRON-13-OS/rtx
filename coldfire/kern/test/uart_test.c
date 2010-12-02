#include "dbug.h"
#include "uart_i_process.h"
#include "crt_process.h"
#include "k_globals.h"
#include "rtx.h"
#include "k_primitives.h"
#include "k_init.h"

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
        if (message != NULL)
        {
            rtx_dbug_outs("Message was sent to CCI\n");
            if (message->msg[0] == '\0' || message->msg[1] == '\0')
            {
                rtx_dbug_outs("Short message given in");
                message->msg[2] = '\0';
            }
            message->msg[0] = '@';
            message->msg[1] = '#';
            k_send_message(CRT_PID, message);
        }
    }
}

int main()
{
    // You need this?
    asm( "move.l #asm_serial_entry,%d0" );
    asm( "move.l %d0,0x10000100" );
    
    rtx_dbug_outs("Beginning of test/n");
    rtx_dbug_outs("Messages will be changed so 1st and 2nd char are @ and #\n");
     
    enable_debug = 0;
    
    pcb_init_t itable[3];
    itable[0].pid = CRT_PID;
    itable[0].name = "crt_process";
    itable[0].priority = 3;
    itable[0].start = start_crt_process;
    itable[0].stack_size = 4096;
    itable[0].is_i_process = 0;
    itable[0].is_sys_process = 0;

    itable[1].pid = UART_I_PROCESS_PID;
    itable[1].name = "uart_i_process";
    itable[1].priority = 0;
    itable[1].start = uart_i_process;
    itable[1].stack_size = 4096;
    itable[1].is_i_process = 1;
    itable[1].is_sys_process = 0;

    itable[2].pid = CCI_PID;
    itable[2].name = "fake_cci";
    itable[2].priority = 0;
    itable[2].start = fake_cci;
    itable[2].stack_size = 4096;
    itable[2].is_i_process = 0;
    itable[2].is_sys_process = 0;
   
    dbug("Starting initialization");
    
    k_init(itable, 3);
    return 0;
}

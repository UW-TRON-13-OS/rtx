#include "dbug.h"
#include "uart_i_process.h"
#include "crt_process.h"
#include "k_globals.h"
#include "rtx.h"
#include "k_primitives.h"

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
//    asm( "move.l #asm_serial_entry,%d0" );
 //   asm( "move.l %d0,0x10000100" );
    
    rtx_dbug_outs("Beginning of test/n");
    rtx_dbug_outs("Messages will be changed so 1st and 2nd char are @ and #\n");
        
    return 0;
}

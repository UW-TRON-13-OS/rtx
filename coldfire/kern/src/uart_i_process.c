#include "uart_i_process.h"
#include "rtx.h"
#include "k_primitives.h"
#include "msg_env_queue.h"
#include "dbug.h"

volatile BYTE CharIn = ' ';
volatile BYTE CharOut = '\0';
volatile BOOLEAN Caught = TRUE;
CHAR buffer[100] = {0};

/*
 * This function is called by the assembly STUB function
 */
VOID c_serial_handler( VOID )
{
    BYTE temp;
    temp = SERIAL1_UCSR;    /* Ack the interrupt */

    /* See if data is waiting.... */
    if( temp & 1 )
    {
        CharIn = SERIAL1_RD;
        Caught = FALSE;
    }

    /* See if port is ready to accept data */
    else if ( temp & 4 )
    {
        SERIAL1_WD = CharOut;   /* Write data to port */
        SERIAL1_IMR = 2;        /* Disable tx Interupt */
    }
    return;
}

void start_uart_i_process()
{

}
